using System;
using System.IO;
using System.Text;
using System.Timers;
using System.Windows;
using System.Threading;
using System.Diagnostics;
using System.Net.Sockets;
using System.ComponentModel;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Collections.Concurrent;

namespace Smart_Home_GUI.Services
{
    public class CAMStream : INotifyPropertyChanged, IDisposable
    {
        private readonly object ffmpegLock = new();
        private Process ffmpegProcess;
        private Stream ffmpegStdin;
        private bool mp4Recording = false;
        private string ffmpegPath = "ffmpeg";
        private System.Timers.Timer rotationTimer;
        private int rotationSeconds = 0;
        private string currentRecordingPath = null;
        private bool useHardwareEncoder = false;
        private bool addTimestamp = false;
        private string timestampFontPath = @"C:\Windows\Fonts\arial.ttf";
        private BlockingCollection<byte[]> ffmpegFrameQueue;
        private Task ffmpegWriterTask;
        private CancellationTokenSource ffmpegWriterCts;
        private readonly int frameQueueCapacity = 200;
        private long totalFramesReceived = 0;
        private long totalFramesDropped = 0;
        private UdpClient UDPReceiver;
        private CancellationTokenSource cancellation;
        private bool Streaming = false;
        private int _port;

        public int Port
        {
            get => _port;
            set
            {
                if (_port != value)
                {
                    _port = value;
                    OnPropertyChanged(nameof(Port));
                    if (Streaming)
                        _ = Restart();
                }
            }
        }

        public CAMStream(int port)
        {
            _port = port;
        }

        private BitmapImage _image = OFFLine();
        public BitmapImage Image
        {
            get => _image;
            private set
            {
                _image = value;
                OnPropertyChanged(nameof(Image));
            }
        }

        public long FramesReceived => Interlocked.Read(ref totalFramesReceived);
        public long FramesDropped => Interlocked.Read(ref totalFramesDropped);

        private static BitmapImage OFFLine()
        {
            var uri = new Uri("pack://application:,,,/ImagesIcons/OFFLine.png", UriKind.Absolute);
            var bmp = new BitmapImage();
            bmp.BeginInit();
            bmp.UriSource = uri;
            bmp.CacheOption = BitmapCacheOption.OnLoad;
            bmp.EndInit();
            bmp.Freeze();
            return bmp;
        }

        private static BitmapImage NoSignal()
        {
            var uri = new Uri("pack://application:,,,/ImagesIcons/NoSignal.png", UriKind.Absolute);
            var bmp = new BitmapImage();
            bmp.BeginInit();
            bmp.UriSource = uri;
            bmp.CacheOption = BitmapCacheOption.OnLoad;
            bmp.EndInit();
            bmp.Freeze();
            return bmp;
        }

        public void Start()
        {
            cancellation = new CancellationTokenSource();
            _ = ReceiveAsync(cancellation.Token);
            Streaming = true;
        }

        public void Stop()
        {
            cancellation?.Cancel();
            UDPReceiver?.Close();
            UDPReceiver?.Dispose();
            UDPReceiver = null;
            Image = OFFLine();
            Streaming = false;
        }

        public void Clear()
        {
            cancellation?.Cancel();
            UDPReceiver?.Close();
            UDPReceiver?.Dispose();
            UDPReceiver = null;
        }

        private async Task Restart()
        {
            await Task.Run(() =>
            {
                cancellation?.Cancel();
                UDPReceiver?.Close();
                UDPReceiver?.Dispose();
                UDPReceiver = null;
                cancellation = new CancellationTokenSource();
                _ = ReceiveAsync(cancellation.Token);
            });
        }

        public void StartRecordingMP4(
            string filePathPattern,
            bool useHardwareEncoder = false,
            int rotationSeconds = 0,
            bool addTimestamp = true,
            string ffmpegExecutablePath = null,
            string timestampFont = null)
        {
            lock (ffmpegLock)
            {
                if (mp4Recording) return;

                if (!string.IsNullOrEmpty(ffmpegExecutablePath))
                    ffmpegPath = ffmpegExecutablePath;

                this.useHardwareEncoder = useHardwareEncoder;
                this.rotationSeconds = rotationSeconds;
                this.addTimestamp = addTimestamp;

                if (!string.IsNullOrEmpty(timestampFont))
                    timestampFontPath = timestampFont;

                currentRecordingPath = ResolvePath(filePathPattern);
                var dir = Path.GetDirectoryName(currentRecordingPath);
                if (!Directory.Exists(dir))
                    Directory.CreateDirectory(dir);

                StartFFmpegForFile(currentRecordingPath);

                ffmpegFrameQueue = new BlockingCollection<byte[]>(frameQueueCapacity);
                ffmpegWriterCts = new CancellationTokenSource();
                ffmpegWriterTask = Task.Run(() => FfmpegWriterLoop(ffmpegWriterCts.Token), ffmpegWriterCts.Token);

                if (rotationSeconds > 0)
                {
                    rotationTimer = new System.Timers.Timer(rotationSeconds * 1000);
                    rotationTimer.Elapsed += RotationTimer_Elapsed;
                    rotationTimer.AutoReset = true;
                    rotationTimer.Start();
                }

                mp4Recording = true;
                Console.WriteLine($"Started MP4 recording -> {currentRecordingPath}");
            }
        }

        public void StopRecordingMP4()
        {
            lock (ffmpegLock)
            {
                if (!mp4Recording) return;

                mp4Recording = false;
                rotationTimer?.Stop();
                rotationTimer?.Dispose();
                rotationTimer = null;

                // Stop accepting frames
                try
                {
                    if (ffmpegFrameQueue != null)
                    {
                        ffmpegFrameQueue.CompleteAdding();
                    }
                    ffmpegWriterCts?.Cancel();

                    ffmpegWriterTask?.Wait(5000);
                }
                catch (AggregateException ae) { Console.WriteLine(ae.Flatten().Message); }
                catch (Exception ex) { Console.WriteLine(ex.Message); }

                CloseAndDisposeFFmpeg();

                try { ffmpegFrameQueue?.Dispose(); } catch { }
                ffmpegFrameQueue = null;
                try { ffmpegWriterCts?.Dispose(); } catch { }
                ffmpegWriterCts = null;
                ffmpegWriterTask = null;

                Console.WriteLine("Stopped MP4 recording");
            }
        }

        private void RotationTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            lock (ffmpegLock)
            {
                if (!mp4Recording) return;

                var newPath = ResolvePath(currentRecordingPath);
                var dir = Path.GetDirectoryName(newPath);
                if (!Directory.Exists(dir))
                    Directory.CreateDirectory(dir);

                try
                {
                    if (ffmpegFrameQueue != null)
                        ffmpegFrameQueue.CompleteAdding();
                    ffmpegWriterCts?.Cancel();
                    ffmpegWriterTask?.Wait(3000);
                }
                catch { }

                CloseAndDisposeFFmpeg();

                StartFFmpegForFile(newPath);

                ffmpegFrameQueue = new BlockingCollection<byte[]>(frameQueueCapacity);
                ffmpegWriterCts = new CancellationTokenSource();
                ffmpegWriterTask = Task.Run(() => FfmpegWriterLoop(ffmpegWriterCts.Token), ffmpegWriterCts.Token);

                currentRecordingPath = newPath;
                Console.WriteLine($"Rotated recording -> {currentRecordingPath}");
            }
        }

        private string ResolvePath(string patternOrPath)
        {
            var p = patternOrPath;
            if (p.Contains("%") || p.Contains("yyyy") || p.Contains("{timestamp}"))
            {
                var now = DateTime.Now;
                p = p.Replace("%yyyy", now.ToString("yyyy"))
                     .Replace("%MM", now.ToString("MM"))
                     .Replace("%dd", now.ToString("dd"))
                     .Replace("%HH", now.ToString("HH"))
                     .Replace("%mm", now.ToString("mm"))
                     .Replace("%ss", now.ToString("ss"))
                     .Replace("yyyy", now.ToString("yyyy"))
                     .Replace("{timestamp}", now.ToString("yyyyMMdd-HHmmss"));
            }
            return p;
        }

        private void StartFFmpegForFile(string outputPath)
        {
            var encoderArgs = GetEncoderArguments();
            var vf = "";

            if (addTimestamp)
            {
                var drawtext = $"drawtext=fontfile='{timestampFontPath}':" +
                               "text='%{localtime\\:%Y-%m-%d %H\\\\:%M\\\\:%S}':" +
                               "x=10:y=10:fontsize=24:fontcolor=white:box=1:boxcolor=0x00000099";
                vf = $"-vf \"{drawtext}\"";
            }

            var args = new StringBuilder();
            args.Append("-y ");
            args.Append("-f image2pipe ");
            args.Append("-vcodec mjpeg ");
            args.Append("-i - ");

            if (!string.IsNullOrEmpty(vf))
                args.Append(vf).Append(' ');

            args.Append(encoderArgs).Append(' ');

            args.Append("-pix_fmt yuv420p ");
            args.Append("-movflags +frag_keyframe+empty_moov+default_base_moof ");
            args.Append("-frag_duration 1000000 ");
            args.Append("-reset_timestamps 1 ");
            args.Append("-g 30 ");
            args.Append("-f mp4 ");
            if (useHardwareEncoder)
            {
                args.Append("-rc vbr -b:v 4000k -maxrate 6000k -bufsize 4000k ");
            }
            else
            {
                args.Append("-crf 23 ");
            }
            args.Append($"\"{outputPath}\"");

            var startInfo = new ProcessStartInfo
            {
                FileName = ffmpegPath,
                Arguments = args.ToString(),
                UseShellExecute = false,
                RedirectStandardInput = true,
                RedirectStandardError = true,
                RedirectStandardOutput = true,
                CreateNoWindow = true,
            };

            ffmpegProcess = new Process { StartInfo = startInfo };

            ffmpegProcess.ErrorDataReceived += (sender, e) =>
            {
                if (!string.IsNullOrEmpty(e.Data))
                    Console.WriteLine($"FFmpeg: {e.Data}");
            };

            ffmpegProcess.Start();
            ffmpegProcess.BeginErrorReadLine();
            ffmpegStdin = ffmpegProcess.StandardInput.BaseStream;

            Console.WriteLine($"FFmpeg started with args: {args}");
        }

        private string GetEncoderArguments()
        {
            if (useHardwareEncoder)
            {
                return "-c:v h264_nvenc -preset fast -profile:v high";
            }
            return "-c:v libx264 -preset veryfast -profile:v high";
        }

        private async Task FfmpegWriterLoop(CancellationToken token)
        {
            try
            {
                while (!token.IsCancellationRequested && !ffmpegFrameQueue.IsCompleted)
                {
                    byte[] frame;
                    try
                    {
                        frame = ffmpegFrameQueue.Take(token);
                    }
                    catch (OperationCanceledException) { break; }
                    catch (InvalidOperationException) { break; }

                    if (frame == null || frame.Length == 0) continue;

                    try
                    {
                        if (ffmpegProcess == null || ffmpegProcess.HasExited || ffmpegStdin == null)
                        {
                            Console.WriteLine("FFmpeg process not available, dropping frame");
                            continue;
                        }

                        await ffmpegStdin.WriteAsync(frame, 0, frame.Length, token).ConfigureAwait(false);

                        if (ffmpegFrameQueue.Count == 0)
                        {
                            await ffmpegStdin.FlushAsync(token).ConfigureAwait(false);
                        }
                    }
                    catch (OperationCanceledException) { break; }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"FFmpeg writer error: {ex.Message}");
                        if (ffmpegProcess != null && ffmpegProcess.HasExited)
                            break;
                    }
                }
            }
            finally
            {
                Console.WriteLine("FFmpeg writer loop finished");
            }
        }

        private void CloseAndDisposeFFmpeg()
        {
            try
            {
                if (ffmpegStdin != null)
                {
                    try
                    {
                        ffmpegStdin.Flush();
                        ffmpegStdin.Close();
                    }
                    catch { }
                    try { ffmpegStdin.Dispose(); } catch { }
                    ffmpegStdin = null;
                }

                if (ffmpegProcess != null)
                {
                    try
                    {
                        if (!ffmpegProcess.HasExited)
                        {
                            if (!ffmpegProcess.WaitForExit(3000))
                            {
                                ffmpegProcess.Kill();
                            }
                        }
                    }
                    catch { }
                    try { ffmpegProcess.Dispose(); } catch { }
                    ffmpegProcess = null;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error closing ffmpeg: {ex.Message}");
            }
        }

        private async Task ReceiveAsync(CancellationToken token)
        {
            UDPReceiver = new UdpClient(Port);
            try
            {
                while (!token.IsCancellationRequested)
                {
                    var received = UDPReceiver.ReceiveAsync();
                    var timeoutTask = Task.Delay(2000, token);
                    var completed = await Task.WhenAny(received, timeoutTask).ConfigureAwait(false);

                    if (completed == timeoutTask)
                    {
                        Image = NoSignal();
                        continue;
                    }

                    var result = await received;
                    Interlocked.Increment(ref totalFramesReceived);

                    if (mp4Recording && ffmpegFrameQueue != null && !ffmpegFrameQueue.IsAddingCompleted)
                    {
                        var copy = new byte[result.Buffer.Length];
                        Buffer.BlockCopy(result.Buffer, 0, copy, 0, result.Buffer.Length);

                        if (!ffmpegFrameQueue.TryAdd(copy, 10))
                        {
                            Interlocked.Increment(ref totalFramesDropped);
                            if (FramesDropped % 30 == 0)
                                Console.WriteLine($"Frames dropped due to full queue: {FramesDropped}");
                        }
                    }

                    await Application.Current.Dispatcher.BeginInvoke((Action)(() =>
                    {
                        try
                        {
                            using var ms = new MemoryStream(result.Buffer);
                            var bitmapImage = new BitmapImage();
                            bitmapImage.BeginInit();
                            bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                            bitmapImage.StreamSource = ms;
                            bitmapImage.EndInit();
                            bitmapImage.Freeze();
                            Image = bitmapImage;
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine($"Error displaying image: {ex.Message}");
                        }
                    }));
                }
            }
            catch (ObjectDisposedException)
            {
              
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error in ReceiveAsync: {ex.Message}");
            }
        }

        public void SetPort(int port) => Port = port;
        public int GetPort() => Port;

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string propertyName)
            => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

        public void Dispose()
        {
            try
            {
                Stop();
                StopRecordingMP4();
            }
            catch { }
        }
    }
}