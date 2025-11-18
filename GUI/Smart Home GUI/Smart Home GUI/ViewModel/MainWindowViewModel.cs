using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Smart_Home_GUI.ViewModel
{

    public class MainWindowViewModel : IDisposable
    {
        public ObservableCollection<string> Rate_Baud { get; }

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
        public void Dispose() 
        { }
    }
}
