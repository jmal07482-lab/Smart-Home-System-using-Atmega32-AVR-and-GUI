using Renci.SshNet;
using Smart_Home_GUI.Model;
using Smart_Home_GUI.Services;
using System;
using System.Collections.Concurrent;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Net.Sockets;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using System.Windows.Threading;

namespace Smart_Home_GUI.ViewModel
{
    public class MainWindowViewModel : IDisposable,  INotifyPropertyChanged
    {
        #region Stream
        public ObservableCollection<string> Feed_Comb { get; }
        private readonly DispatcherTimer CAMConnectedTimer;
        public string UserName { get; set; } = "rov";
        public string Password { get; set; } = "rov2025";
        private string _localIp = "192.168.0.101";
        public string LocalIP
        {
            get => _localIp;
            set
            {
                if (_localIp != value)
                {
                    _localIp = value;
                    OnPropertyChanged(nameof(LocalIP));
                    ResetIP();
                }
            }
        }
        private string _remoteIp = "192.168.0.104";
        public string RemoteIP
        {
            get => _remoteIp;
            set
            {
                if (_remoteIp != value)
                {
                    _remoteIp = value;
                    OnPropertyChanged(nameof(RemoteIP));
                    ResetIP();
                }
            }
        }
        public int Cam1_Port
        {
            get => _cam1_port;
            set
            {
                if (_cam1_port != value)
                {
                    _cam1_port = value;
                    OnPropertyChanged(nameof(Cam1_Port));
                    Feed1?.SetPort(Cam1_Port);
                }
            }
        }
        private int _cam2_port = 6000;
        public int Cam2_Port
        {
            get => _cam2_port;
            set
            {
                if (_cam2_port != value)
                {
                    _cam2_port = value;
                    OnPropertyChanged(nameof(Cam2_Port));
                    Feed2?.SetPort(Cam2_Port);
                }
            }
        }
        private int _cam3_port = 7000;
        public int Cam3_Port
        {
            get => _cam3_port;
            set
            {
                if (_cam3_port != value)
                {
                    _cam3_port = value;
                    OnPropertyChanged(nameof(Cam3_Port));
                    Feed3?.SetPort(Cam3_Port);
                }
            }
        }
        private readonly CAMStream Feed1;
        private BitmapImage _image1;
        public BitmapImage Image1
        {
            get => _image1;
            set
            {
                _image1 = value;
                OnPropertyChanged(nameof(Image1));
            }
        }
        private readonly CAMStream Feed2;
        private BitmapImage _image2;
        public BitmapImage Image2
        {
            get => _image2;
            set
            {
                _image2 = value;
                OnPropertyChanged(nameof(Image2));
            }
        }
        private readonly CAMStream Feed3;
        private BitmapImage _image3;
        public BitmapImage Image3
        {
            get => _image3;
            set
            {
                _image3 = value;
                OnPropertyChanged(nameof(Image3));
            }
        }
        public int PIndex1;
        public int PIndex2;
        public int PIndex3;
        private int _index1 = 0;
        public int Index1
        {
            get => _index1;
            set
            {
                if (_index1 != value)
                {
                    _index1 = value;
                    OnPropertyChanged(nameof(Index1));
                    _ = FeedChange(1);
                }
            }
        }
        private int _index2 = 0;
        public int Index2
        {
            get => _index2;
            set
            {
                if (_index2 != value)
                {
                    _index2 = value;
                    OnPropertyChanged(nameof(Index2));
                    _ = FeedChange(2);
                }
            }
        }
        private int _index3 = 0;
        public int Index3
        {
            get => _index3;
            set
            {
                if (_index3 != value)
                {
                    _index3 = value;
                    OnPropertyChanged(nameof(Index3));
                    _ = FeedChange(3);
                }
            }
        }
        private bool isCAMConnect = false;
        public bool IsCAMConnect
        {
            get => isCAMConnect;
            set
            {
                if (isCAMConnect != value)
                {
                    isCAMConnect = value;
                    OnPropertyChanged(nameof(IsCAMConnect));
                }
            }
        }
        private bool _isCAMconnectedind = false;
        public bool IsCAMConnectedInd
        {
            get => _isCAMconnectedind;
            set
            {
                _isCAMconnectedind = value;
                OnPropertyChanged(nameof(IsCAMConnectedInd));
            }
        }
        private string _isCAMconnectedtext = "";
        public string CAMConnectedText
        {
            get => _isCAMconnectedtext;
            set
            {
                _isCAMconnectedtext = value;
                OnPropertyChanged(nameof(CAMConnectedText));
            }
        }
        private bool _isStream = false;
        public bool IsStream
        {
            get { return _isStream; }
            set
            {
                if (_isStream != value)
                {
                    _isStream = value;
                    OnPropertyChanged(nameof(IsStream));
                }
            }
        }
        private bool _isRecord = false;
        public bool IsRecord
        {
            get => _isRecord;
            set
            {
                if (_isRecord != value)
                {
                    _isRecord = value;
                    OnPropertyChanged(nameof(IsRecord));
                }
            }
        }
        private int _cam1_port = 5000;
        public ICommand CAMConnectCommand { get; }
        public ICommand StreamCommand { get; }
        public ICommand RecordCommand { get; }
        private async void ResetIP()
        {
            Feed1?.StopRecordingMP4();
            Feed2?.StopRecordingMP4();
            Feed3?.StopRecordingMP4();
            Feed1?.Stop();
            Feed2?.Stop();
            Feed3?.Stop();
            IsCAMConnectedInd = false;
            CAMConnectedText = "";
            CAMConnectedTimer.Stop();
            if (isCAMConnect)
            {
                IsCAMConnect = await ConnectionCheckAsync(RemoteIP);
                if (IsCAMConnect)
                {
                    if(IsStream)
                        _ = StartStreaming();
                    if(IsRecord)
                        _ = StartRecording();
                }
                else
                {
                    IsCAMConnectedInd = false;
                    CAMConnectedText = "";
                    CAMConnectedTimer.Stop();
                    Feed1?.StopRecordingMP4();
                    Feed2?.StopRecordingMP4();
                    Feed3?.StopRecordingMP4();
                    Feed1?.Stop();
                    Feed2?.Stop();
                    Feed3?.Stop();
                    IsRecord = false;
                    IsStream = false;
                }
            }
        }
        private async void ExecuteCAMConnectCommand()
        {
            try
            {
                if (!IsCAMConnect)
                {
                    IsCAMConnect = await ConnectionCheckAsync(RemoteIP);
                    if (!IsCAMConnect)
                    {
                        MessageBox.Show($"Failed to connect to {RemoteIP} !");
                        return;
                    }
                    CAMConnectedTimer.Start();
                }
                else
                {
                    CAMConnectedTimer.Stop();
                    if (IsRecord)
                        _ = StopRecording();
                    if (IsStream)
                        _ = StopStreaming();
                    IsRecord = false;
                    IsStream = false;
                    IsCAMConnect = false;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Camera connection error:\n{ex.Message}");
                IsCAMConnect = false;
            }
        }
        private void ExecuteStreamCommand()
        {
            if (IsCAMConnect)
            {
                IsStream = !IsStream;
                if (IsStream)
                    _ = StartStreaming();
                else
                    _ = StopStreaming();
            }
            else
                MessageBox.Show("No Connection.");
        }
        private void ExecuteRecordCommand()
        {
            if (IsCAMConnect && IsStream)
            {
                IsRecord = !IsRecord;
                if (IsRecord)
                    _ = StartRecording();
                else
                    _ = StopRecording();
            }
            else
                MessageBox.Show("No Stream.");
        }
        public async Task StartStreaming()
        {
            await Task.Run(() =>
            {
                Index1 = PIndex1;
                Index2 = PIndex2;
                Index3 = PIndex3;
                ;
                if (IsCAMConnect)
                {
                    using var ssh = new SshClient(RemoteIP, UserName, Password);
                    ssh.Connect();
                    ssh.RunCommand("nohup python3 /home/rov/main.py > /dev/null 2>&1 &");
                    ssh.Disconnect();
                }
                else
                {
                    MessageBox.Show("No Connection.");
                }
                Feed1?.Start();
                Feed2?.Start();
                Feed3?.Start();

            });
        }
        public async Task StopStreaming()
        {
            await Task.Run(() =>
            {
                if (IsCAMConnect)
                {
                    using var ssh = new SshClient(RemoteIP, UserName, Password);
                    ssh.Connect();
                    ssh.RunCommand("pkill -f rpicam-vid");
                    ssh.RunCommand("pkill -f main.py");
                    ssh.Disconnect();
                }
                else
                {
                    MessageBox.Show("No Connection.");
                }
                Feed1?.Stop();
                Feed2?.Stop();
                Feed3?.Stop();

            });
        }
        public async Task StartRecording()
        {
            await Task.Run(() =>
            {
                try
                {
                    var timestamp = DateTime.Now.ToString("yyyy-MM-dd_HH-mm-ss");
                    var file_1 = $@"C:\Smart_Home\Cam_1\cam-test-{timestamp}.mp4";
                    var file_2 = $@"C:\Smart_Home\Cam_2\cam-test-{timestamp}.mp4";
                    var file_3 = $@"C:\Smart_Home\Cam_3\cam-test-{timestamp}.mp4";

                    Directory.CreateDirectory(@"C:\Smart_Home\Cam_1");
                    Directory.CreateDirectory(@"C:\Smart_Home\Cam_2");
                    Directory.CreateDirectory(@"C:\Smart_Home\Cam_3");

                    Feed1?.StartRecordingMP4(
                        file_1,
                        useHardwareEncoder: false,
                        rotationSeconds: 0,
                        addTimestamp: false,
                        ffmpegExecutablePath: @"C:\ffmpeg\ffmpeg\bin\ffmpeg.exe");

                    Feed2?.StartRecordingMP4(
                        file_2,
                        useHardwareEncoder: false,
                        rotationSeconds: 0,
                        addTimestamp: false,
                        ffmpegExecutablePath: @"C:\ffmpeg\ffmpeg\bin\ffmpeg.exe");

                    Feed3?.StartRecordingMP4(
                        file_3,
                        useHardwareEncoder: false,
                        rotationSeconds: 0,
                        addTimestamp: false,
                        ffmpegExecutablePath: @"C:\ffmpeg\ffmpeg\bin\ffmpeg.exe");

                    Console.WriteLine($"Started recording at {timestamp}");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error starting recording: {ex.Message}");
                }
            });
        }
        public async Task StopRecording()
        {
            await Task.Run(() =>
            {
                Feed1?.StopRecordingMP4();
                Feed2?.StopRecordingMP4();
                Feed3?.StopRecordingMP4();
            });
        }
        private void IndexChange(ref int a, ref int b, ref int c, ref int d, int e)
        {
            d = b;
            int temp = b;
            b = a;
            if (c > 0) // index(X) should change last after (b) change so no two change happen at the same time
                switch (e)
                {
                    case 1:
                        Index1 = temp;
                        break;
                    case 2:
                        Index2 = temp;
                        break;
                    case 3:
                        Index3 = temp;
                        break;
                    default: break;
                }
        }
        private async Task FeedChange(int a)
        {
            await Task.Run(() =>
            {
                switch (a)
                {
                    case 1:
                        if (Index1 == 0)
                        {
                            if (IsStream)
                                PIndex1 = Index1;
                        }
                        else if (Index1 == Index2 || Index1 == PIndex2)
                        {
                            Feed1.Clear();
                            Feed2.Clear();
                            int p = Feed1.GetPort();
                            Feed1.SetPort(Feed2.GetPort());
                            Feed2.SetPort(p);
                            IndexChange(ref _index1, ref PIndex1, ref _index2, ref PIndex2, 2);
                        }
                        else if (_index1 == Index3 || _index1 == PIndex3)
                        {
                            Feed1.Clear();
                            Feed3.Clear();
                            int p = Feed1.GetPort();
                            Feed1.SetPort(Feed3.GetPort());
                            Feed3.SetPort(p);
                            IndexChange(ref _index1, ref PIndex1, ref _index3, ref PIndex3, 3);
                        }
                        else
                        {
                            PIndex1 = Index1;
                        }
                        break;
                    case 2:
                        if (Index2 == 0)
                        {
                            if (IsStream)
                                PIndex2 = Index2;
                        }
                        else if (_index2 == _index1 || _index2 == PIndex1)
                        {
                            int p = Feed2.GetPort();
                            Feed1.Clear();
                            Feed2.Clear();
                            Feed2.SetPort(Feed1.GetPort());
                            Feed1.SetPort(p);
                            IndexChange(ref _index2, ref PIndex2, ref _index1, ref PIndex1, 1);
                        }
                        else if (_index2 == _index3 || _index2 == PIndex3)
                        {
                            int p = Feed2.GetPort();
                            Feed2.Clear();
                            Feed3.Clear();
                            Feed2.SetPort(Feed3.GetPort());
                            Feed3.SetPort(p);
                            IndexChange(ref _index2, ref PIndex2, ref _index3, ref PIndex3, 3);
                        }
                        else
                        {
                            PIndex2 = Index2;
                        }
                        break;
                    case 3:
                        if (Index3 == 0)
                        {
                            if (IsStream)
                                PIndex3 = Index3;
                        }
                        else if (_index3 == _index1 || _index3 == PIndex1)
                        {
                            Feed1.Clear();
                            Feed3.Clear();
                            int p = Feed3.GetPort();
                            Feed3.SetPort(Feed1.GetPort());
                            Feed1.SetPort(p);
                            IndexChange(ref _index3, ref PIndex3, ref _index1, ref PIndex1, 1);
                        }
                        else if (_index3 == _index2 || _index3 == PIndex2)
                        {
                            Feed2.Clear();
                            Feed3.Clear();
                            int p = Feed3.GetPort();
                            Feed3.SetPort(Feed2.GetPort());
                            Feed2.SetPort(p);
                            IndexChange(ref _index3, ref PIndex3, ref _index2, ref PIndex2, 2);
                        }
                        else
                        {
                            PIndex3 = Index3;
                        }
                        break;
                    default: break;
                }
            });
        }
        private async void CAMConnected_Tick(object sender, EventArgs e)
        {
            IsCAMConnect = await ConnectionCheckAsync(RemoteIP);
            if (IsCAMConnect)
            {
                IsCAMConnectedInd = true;
                CAMConnectedText = $"Connected to {RemoteIP}";
                await Task.Delay(500);
                IsCAMConnectedInd = false;
                CAMConnectedText = "";
                await Task.Delay(500);
            }
            else
            {
                CAMConnectedTimer.Stop();
                IsCAMConnectedInd = false;
                CAMConnectedText = "";
                if (IsRecord)
                    _ = StopRecording();
                IsRecord = false;
                if (IsStream)
                    _ = StopStreaming();
                IsStream = false;
            }
        }
        public static async Task<bool> ConnectionCheckAsync(string remoteIP, int port = 22, int timeoutMs = 1000)
        {
            try
            {
                using var client = new TcpClient();
                var connectTask = client.ConnectAsync(remoteIP, port);
                var timeoutTask = Task.Delay(timeoutMs);
                var completedTask = await Task.WhenAny(connectTask, timeoutTask);
                return completedTask == connectTask && client.Connected;
            }
            catch
            {
                return false;
            }
        }
        #endregion


        #region UART
        public ObservableCollection<string> Baud_Rate { get; }
        public ObservableCollection<string> FilteredPorts { get; } = [];
        private readonly DispatcherTimer UARTConnectedTimer;
        readonly UARTCommunication uart;
        private string localPort = "COM3";
        public string LocalPort
        {
            get => localPort;
            set
            {
                if (localPort != value)
                {
                    localPort = value;
                    OnPropertyChanged();
                    uart?.SetUART(LocalPort, RemotePort, BaudRate);
                    _ = CheckRemoteConnection();
                }
            }
        }
        private string remotePort = "COM4";
        public string RemotePort
        {
            get => remotePort;
            set
            {
                if (remotePort != value)
                {
                    remotePort = value;
                    OnPropertyChanged();
                    uart?.SetUART(LocalPort, RemotePort, BaudRate);
                    _ = CheckRemoteConnection();
                }
            }
        }
        private int _index = 5;
        public int Index
        {
            get => _index;
            set
            {
                if (_index != value)
                {
                    _index = value;
                    BaudRate = int.Parse(Baud_Rate[_index]);
                    OnPropertyChanged(nameof(Index));
                }
            }
        }
        private int baudRate;
        public int BaudRate
        {
            get => baudRate;
            set
            {
                if (baudRate != value)
                {
                    baudRate = value;
                    OnPropertyChanged();
                    uart?.SetUART(LocalPort, RemotePort, BaudRate);
                }
            }
        }
        private bool isUARTConnect = false;
        public bool IsUARTConnect
        {
            get => isUARTConnect;
            set
            {
                if (isUARTConnect != value)
                {
                    isUARTConnect = value;
                    OnPropertyChanged(nameof(IsUARTConnect));
                }
            }
        }
        private bool _isUARTconnectedind = false;
        public bool IsUARTConnectedInd
        {
            get => _isUARTconnectedind;
            set
            {
                _isUARTconnectedind = value;
                OnPropertyChanged(nameof(IsUARTConnectedInd));
            }
        }
        private string _UARTconnectedtext = "";
        public string UARTConnectedText
        {
            get => _UARTconnectedtext;
            set
            {
                _UARTconnectedtext = value;
                OnPropertyChanged(nameof(UARTConnectedText));
            }
        }
        public ICommand UARTConnectCommand { get; }
        private async void ExecuteUARTConnectCommand()
        {
            if (!IsUARTConnect)
            {
                IsUARTConnect = await uart.IsRemoteConnected();
                if (!IsUARTConnect)
                {
                    MessageBox.Show($"Connect to {RemotePort}, {BaudRate}  faild !");
                }
                else
                {
                    uart?.Connect();
                    UARTConnectedTimer?.Start();
                }
            }
            else
            {
                UARTConnectedTimer.Stop();
                IsUARTConnectedInd = false;
                UARTConnectedText = "";
                uart?.Disconnect();
                IsUARTConnect = false;
            }
        }
        private async Task CheckRemoteConnection()
        {
            if (await uart.IsRemoteConnected())
            {
                uart?.Disconnect();
                UARTConnectedTimer?.Stop();
                IsUARTConnect = false;
            }
        }
        private async void UARTConnected_Tick(object sender, EventArgs e)
        {
            IsUARTConnect = await uart.IsRemoteConnected();
            if (IsUARTConnect)
            {
                IsUARTConnectedInd = true;
                UARTConnectedText = $"Connected to {RemotePort}";
                await Task.Delay(500);
                IsUARTConnectedInd = false;
                UARTConnectedText = "";
                await Task.Delay(500);
            }
            else
            {
                UARTConnectedTimer.Stop();
                IsUARTConnectedInd = false;
                UARTConnectedText = "";
                uart?.Disconnect();
            }
        }
        #endregion



        #region Keypad
        public RelayCommand PressCommand { get; }
        private string keyValue = "";
        public string KeyValue
        {
            get => keyValue;
            set
            {
                keyValue = value;
                var ts = $"{keyValue}";
                while (StatusLines0.Count > 0)
                    StatusLines0.RemoveAt(0);
                StatusLines0.Add(ts);
                OnPropertyChanged();
            }
        }
        #endregion


        #region Environment
        private readonly Temperature Room_Temperature_1 = new();
        private readonly Temperature Room_Temperature_2 = new();
        private readonly Temperature Room_Temperature_3 = new();
        public double Temperature1
        {
            get => Room_Temperature_1.Celsius;
            set
            {
                if (Room_Temperature_1.Celsius != value)
                {
                    Room_Temperature_1.Celsius = value;
                    OnPropertyChanged();
                }
            }
        }
        public double Temperature2
        {
            get => Room_Temperature_2.Celsius;
            set
            {
                if (Room_Temperature_2.Celsius != value)
                {
                    Room_Temperature_2.Celsius = value;
                    OnPropertyChanged();
                }
            }
        }
        public double Temperature3
        {
            get => Room_Temperature_3.Celsius;
            set
            {
                if (Room_Temperature_3.Celsius != value)
                {
                    Room_Temperature_3.Celsius = value;
                    OnPropertyChanged();
                }
            }
        }
        private readonly Humidity Room_Humidity_1 = new ();
        private readonly Humidity Room_Humidity_2 = new();
        private readonly Humidity Room_Humidity_3 = new();
        public string HumidityStatus1
        {
            get => Room_Humidity_1.Status;
        }
        public double Humidity1
        {
            get => Room_Humidity_1.RelativeHumidityPercentage;
            set
            {
                if (Room_Humidity_1.RelativeHumidityPercentage != value)
                {
                    Room_Humidity_1.RelativeHumidityPercentage = value;
                    OnPropertyChanged();
                    OnPropertyChanged(nameof(HumidityStatus1));
                }
            }
        }
        public string HumidityStatus2
        {
            get => Room_Humidity_2.Status;
        }
        public double Humidity2
        {
            get => Room_Humidity_2.RelativeHumidityPercentage;
            set
            {
                if (Room_Humidity_2.RelativeHumidityPercentage != value)
                {
                    Room_Humidity_2.RelativeHumidityPercentage = value;
                    OnPropertyChanged();
                    OnPropertyChanged(nameof(HumidityStatus2));
                }
            }
        }
        public string HumidityStatus3
        {
            get => Room_Humidity_3.Status;
        }
        public double Humidity3
        {
            get => Room_Humidity_3.RelativeHumidityPercentage;
            set
            {
                if (Room_Humidity_3.RelativeHumidityPercentage != value)
                {
                    Room_Humidity_3.RelativeHumidityPercentage = value;
                    OnPropertyChanged();
                    OnPropertyChanged(nameof(HumidityStatus3));
                }
            }
        }

        private bool _isLed1 = false;
        public bool IsLed1
        {
            get { return _isLed1; }
            set
            {
                if (_isLed1 != value)
                {
                    _isLed1 = value;
                    OnPropertyChanged(nameof(IsLed1));
                }
            }
        }
        private bool _isLed2 = false;
        public bool IsLed2
        {
            get { return _isLed2; }
            set
            {
                if (_isLed2 != value)
                {
                    _isLed2 = value;
                    OnPropertyChanged(nameof(IsLed2));
                }
            }
        }
        private bool _isLed3 = false;
        public bool IsLed3
        {
            get { return _isLed3; }
            set
            {
                if (_isLed3 != value)
                {
                    _isLed3 = value;
                    OnPropertyChanged(nameof(IsLed3));
                }
            }
        }
        private bool _isLed4 = false;
        public bool IsLed4
        {
            get { return _isLed4; }
            set
            {
                if (_isLed4 != value)
                {
                    _isLed4 = value;
                    OnPropertyChanged(nameof(IsLed4));
                }
            }
        }
        private readonly DispatcherTimer Air_timer1;
        private readonly DispatcherTimer Air_timer2;
        private readonly DispatcherTimer Air_timer3;
        private bool _isAir1 = false;
        public bool IsAir1
        {
            get { return _isAir1; }
            set
            {
                if (_isAir1 != value)
                {
                    _isAir1 = value;
                    OnPropertyChanged(nameof(IsAir1));
                    if(_isAir1)
                        Air_timer1.Start();
                    else
                        Air_timer1.Stop();
                }
            }
        }
        private bool _isAir2 = false;
        public bool IsAir2
        {
            get { return _isAir2; }
            set
            {
                if (_isAir2 != value)
                {
                    _isAir2 = value;
                    OnPropertyChanged(nameof(IsAir2));
                    if(_isAir2)
                        Air_timer2.Start();
                    else
                        Air_timer2.Stop();
                }
            }
        }
        private bool _isAir3 = false;
        public bool IsAir3
        {
            get { return _isAir3; }
            set
            {
                if (_isAir3 != value)
                {
                    _isAir3 = value;
                    OnPropertyChanged(nameof(IsAir3));
                    if(_isAir3)
                        Air_timer3.Start();
                    else
                        Air_timer3.Stop();
                }
            }
        }
        #endregion








        private double _fahrenheit;
        public double Fahrenheit
        {
            get => _fahrenheit;
            private set
            {
                if (_fahrenheit != value)
                {
                    _fahrenheit = value;
                    OnPropertyChanged();
                }
            }
        }

















        private readonly ConcurrentQueue<string> _statusQueue = [];
        public ObservableCollection<string> StatusLines { get; } = [];
        public ObservableCollection<string> StatusLines0 { get; } = [];
        private void Uart_DataReceived(object sender, SerialDataEventArgs e)
        {
            string data = e.RawData;
            try
            {
                string[] parts = data.Split(',');
                foreach (var part in parts)
                {
                    string[] keyValue = part.Split(':');
                    if (keyValue.Length == 2)
                    {
                        string key = keyValue[0].Trim();
                        string value = keyValue[1].Trim();
                        Application.Current.Dispatcher.Invoke(() =>
                        {
                            UpdateUI(key, value);
                            StatusLines.Add($"{key} : {value}");
                            if (key == "Correct Password")
                            {
                                KeyValue = "";
                            }
                        });
                    }
                }
            }
            catch (Exception ex)
            {
                Application.Current.Dispatcher.Invoke(() =>
                {
                    StatusLines.Add("Parse Error: " + ex.Message);
                });
            }
        }
        public void UpdateUI(string key, string value)
        {
            switch(key[0])
            {
                case 'T':
                    switch(key[1])
                    {
                        case '1':
                            Temperature1 = double.Parse(value);
                            break;
                        case '2':
                            Temperature2 = double.Parse(value);
                            break;
                        case '3':
                            Temperature3 = double.Parse(value);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'H':
                    switch (key[1])
                    {
                        case '1':
                            Humidity1 = double.Parse(value);
                            break;
                        case '2':
                            Humidity2 = double.Parse(value);
                            break;
                        case '3':
                            Humidity3 = double.Parse(value);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'L':
                    switch (key[1])
                    {
                        case '1':
                            IsLed1 = value == "1";
                            break;
                        case '2':
                            IsLed2 = value == "1";
                            break;
                        case '3':
                            IsLed3 = value == "1";
                            break;
                        case '4':
                            IsLed4 = value == "1";
                            break;
                        default:
                            break;
                    }
                    break;
                case 'A':
                    switch (key[1])
                    {
                        case '1':
                            IsAir1 = value == "1";
                            break;
                        case '2':
                            IsAir2 = value == "1";
                            break;
                        case '3':
                            IsAir3 = value == "1";
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        private double _airFan1;
        public double AirFan1
        {
            get => _airFan1;
            set
            {
                if (_airFan1 != value)
                {
                    _airFan1 = value;
                    OnPropertyChanged();
                }
            }
        }
        private double _airFan2;
        public double AirFan2
        {
            get => _airFan2;
            set
            {
                if (_airFan2 != value)
                {
                    _airFan2 = value;
                    OnPropertyChanged();
                }
            }
        }
        private double _airFan3;
        public double AirFan3
        {
            get => _airFan3;
            set
            {
                if (_airFan3 != value)
                {
                    _airFan3 = value;
                    OnPropertyChanged();
                }
            }
        }
        
        
        public MainWindowViewModel()
        {
            Air_timer1 = new DispatcherTimer
            {
                Interval = TimeSpan.FromMilliseconds(1)
            };
            Air_timer1.Tick += (s, e) =>
            {
                AirFan1 += 5;
                if (AirFan1 >= 360)
                    AirFan1 = 0;
            };
            Air_timer2 = new DispatcherTimer
            {
                Interval = TimeSpan.FromMilliseconds(1)
            };
            Air_timer2.Tick += (s, e) =>
            {
                AirFan2 += 5;
                if (AirFan2 >= 360)
                    AirFan2 = 0;
            };
            Air_timer3 = new DispatcherTimer
            {
                Interval = TimeSpan.FromMilliseconds(1)
            };
            Air_timer3.Tick += (s, e) =>
            {
                AirFan3 += 5;
                if (AirFan3 >= 360)
                    AirFan3 = 0;
            };


            StatusLines = [];
            uart = new UARTCommunication();
            uart.DataReceived += Uart_DataReceived;
            PressCommand = new RelayCommand(param =>
            {
                if (param?.ToString() == "E")
                {
                    uart.SendData(KeyValue + "\n");
                }
                else if (param?.ToString() == "R")
                {
                    int n = KeyValue.Length;
                    if (n > 0)
                        KeyValue = KeyValue.Substring(0, n - 1);
                }
                else
                    KeyValue += param?.ToString();
            });
            Baud_Rate =
            [
                "300",
                "600",
                "1200",
                "2400",
                "4800",
                "9600",
                "14400",
                "19200",
                "28800",
                "57600",
                "76800",
                "115200",
                "128000"
            ];
            BaudRate = int.Parse(Baud_Rate[Index]);


            #region Stream
            Feed_Comb =
            [
                "None",
                "Camera 1",
                "Camera 2",
                "Camera 3"
            ];
            CAMConnectCommand = new RelayCommand(_ => ExecuteCAMConnectCommand());
            StreamCommand = new RelayCommand(_ => ExecuteStreamCommand());
            RecordCommand = new RelayCommand(_ => ExecuteRecordCommand());
            Feed1 = new CAMStream(Cam1_Port);
            Feed1.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "Image")
                {
                    Image1 = Feed1.Image;
                }
            };
            Feed1.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "Port")
                {
                    Cam1_Port = Feed1.Port;
                }
            };
            Feed2 = new CAMStream(Cam2_Port);
            Feed2.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "Image")
                {
                    Image2 = Feed2.Image;
                }
            };
            Feed2.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "Port")
                {
                    Cam2_Port = Feed2.Port;
                }
            };
            Feed3 = new CAMStream(Cam3_Port);
            Feed3.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "Image")
                {
                    Image3 = Feed3.Image;
                }
            };
            Feed3.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "Port")
                {
                    Cam3_Port = Feed3.Port;
                }
            };
            Image1 = Feed1.Image;
            Image2 = Feed2.Image;
            Image3 = Feed3.Image;
            PIndex1 = 1;
            PIndex2 = 2;
            PIndex3 = 3;
            CAMConnectedTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            CAMConnectedTimer.Tick += CAMConnected_Tick;
            #endregion
            #region UART
            UARTConnectCommand = new RelayCommand(_ => ExecuteUARTConnectCommand());
            uart.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "LocalPort")
                {
                    LocalPort = uart.LocalPort;
                }
            };
            uart.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "RemotePort")
                {
                    RemotePort = uart.RemotePort;
                }
            };
            uart.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "BaudRate")
                {
                    BaudRate = uart.BaudRate;
                }
            };
            UARTConnectedTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            UARTConnectedTimer.Tick += UARTConnected_Tick;
            #endregion
        }
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string name = null) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        public void Dispose()
        {
            uart.Dispose();
            Feed1?.Dispose();
            Feed2?.Dispose();
            Feed3?.Dispose();
            Air_timer1.Stop();
            Air_timer2.Stop();
            Air_timer3.Stop();
        }
    }
}
