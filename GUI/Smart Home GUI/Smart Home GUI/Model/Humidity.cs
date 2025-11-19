using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Smart_Home_GUI.Model
{
    public class Humidity
    {
        public double RelativeHumidityPercentage { get; set; }
        public string Status
        {
            get
            {
                if (RelativeHumidityPercentage >= 60)
                {
                    return "High (Risk of mold)";
                }
                else if (RelativeHumidityPercentage <= 30)
                {
                    return "Low (Dry air)";
                }
                else
                {
                    return "Optimal";
                }
            }
        }
    }
}
