using Smart_Home_GUI.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Smart_Home_GUI.ViewModel
{

    public class MainWindowViewModel : IDisposable,  INotifyPropertyChanged
    {
        public ObservableCollection<string> Rate_Baud { get; }
        private Temperature _model = new Temperature();

        public double Celsius
        {
            get => _model.Celsius;
            set
            {
                if (_model.Celsius != value)
                {
                    _model.Celsius = value;
                    OnPropertyChanged();
                }
            }
        }
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
        private readonly Humidity Humidity_model = new Humidity();
        public string HumidityStatus
        {
            get => Humidity_model.Status;
        }
        public double CurrentHumidity
        {
            get => Humidity_model.RelativeHumidityPercentage;
            set
            {
                if (Humidity_model.RelativeHumidityPercentage != value)
                {
                    Humidity_model.RelativeHumidityPercentage = value;
                    OnPropertyChanged();
                    OnPropertyChanged(nameof(HumidityStatus));
                }
            }
        }

        public MainWindowViewModel()
        {
            Rate_Baud = new ObservableCollection<string>
            {
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
            };
        }
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string name = null) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        public void Dispose() 
        { }
    }
}
