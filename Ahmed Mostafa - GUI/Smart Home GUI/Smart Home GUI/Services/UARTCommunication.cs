using System;
using System.Text;
using System.IO.Ports;
using System.ComponentModel;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
namespace Smart_Home_GUI.Services
{
    public class SerialDataEventArgs(string rawData) : EventArgs
    {
        public string RawData { get; } = rawData;
    }
    public class UARTCommunication : INotifyPropertyChanged, IDisposable
    {
        private SerialPort serialPort;
        private readonly StringBuilder rxBuffer = new();
        bool isconnect = false;
        public ObservableCollection<string> FilteredPorts { get; }
            = [];
        private bool isConnect = false;
        public bool IsConnect
        {
            get => isConnect;
            set
            {
                if (isConnect != value)
                {
                    isConnect = value;
                    OnPropertyChanged();
                }
            }
        }
        private string localPort;
        public string LocalPort
        {
            get => localPort;
            set
            {
                if (localPort != value)
                {
                    localPort = value;
                    OnPropertyChanged();
                }
            }
        }
        private string remotePort;
        public string RemotePort
        {
            get => remotePort;
            set
            {
                if (remotePort != value)
                {
                    remotePort = value;
                    OnPropertyChanged();
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
                }
            }
        }
        public event EventHandler<SerialDataEventArgs> DataReceived;
        public UARTCommunication()
        {
            
        }
        public void SetUART(string locallort, string remoteport, int baudrate)
        {
            if (isconnect)
            {
                Disconnect();
                LocalPort = locallort;
                RemotePort = remoteport;
                BaudRate = baudrate;
                Connect();
            }
            else
            {
                LocalPort = locallort;
                RemotePort = remoteport;
                BaudRate = baudrate;
            }
        }
        public void Connect()
        {
            try
            {
                Disconnect();
                serialPort = new SerialPort(LocalPort, BaudRate)
                {
                    Encoding = System.Text.Encoding.ASCII,
                    Handshake = Handshake.None,
                    DtrEnable = true,
                    RtsEnable = true
                };
                serialPort.DataReceived += SerialPort_DataReceived;
                serialPort.Open();
                isconnect = true;
            }
            catch (Exception)
            {
                
            }
        }
        public async Task<bool> IsRemoteConnected()
        {
            FilteredPorts.Clear();
            foreach (var p in SerialPort.GetPortNames())
                FilteredPorts.Add(p);
            if (!FilteredPorts.Contains(LocalPort))
                return false;
            if (!FilteredPorts.Contains(RemotePort))
                return false;
            return await Task.Run(() =>
            {
                try
                {
                    using SerialPort sp = new(RemotePort, BaudRate);
                    sp.Open();
                    sp.Close();
                    return false;
                }
                catch
                {
                    return true;
                }
            });
        }
        public void Disconnect()
        {
            try
            {
                if (serialPort != null)
                {
                    if (serialPort.IsOpen)
                        serialPort.Close();
                    serialPort.DataReceived -= SerialPort_DataReceived;
                    serialPort.Dispose();
                    isconnect = false;
                }
            }
            catch { }
        }
        public void SendData(string data)
        {
            if (isconnect)
            {
                try
                {
                    serialPort.WriteLine(data);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("[UART] Send error: " + ex.Message);
                }
            }
        }
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string chunk = serialPort.ReadExisting();
                rxBuffer.Append(chunk);
                if (rxBuffer.ToString().Contains("\n"))
                {
                    string[] messages = rxBuffer.ToString().Split('\n');
                    for (int i = 0; i < messages.Length - 1; i++)
                    {
                        string fullMessage = messages[i].Trim();
                        if (fullMessage.Length > 0)
                        {
                            DataReceived?.Invoke(this, new SerialDataEventArgs(fullMessage));
                        }
                    }
                    rxBuffer.Clear();
                    rxBuffer.Append(messages[messages.Length - 1]);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("UART receive error: " + ex.Message);
            }
        }
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string name = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
        public void Dispose()
        {
            Disconnect();
        }
    }
}
