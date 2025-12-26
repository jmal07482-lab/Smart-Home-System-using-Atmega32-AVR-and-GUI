using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Smart_Home_GUI.Model
{
    public class Temperature
    {
        public double Celsius { get; set; }
        public double ConvertToFahrenheit()
        {
            return (Celsius * 9.0 / 5.0) + 32;
        }
    }
}
