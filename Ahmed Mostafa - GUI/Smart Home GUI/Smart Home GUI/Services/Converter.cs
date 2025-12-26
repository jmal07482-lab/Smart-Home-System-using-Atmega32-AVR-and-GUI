using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Media;

namespace Smart_Home_GUI.Services
{
    public class StreamButtonConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            TextBlock textBlock = new();
            if ((bool)value)
            {
                textBlock.Inlines.Add(new Run("Stop Streaming ") { Foreground = Brushes.White });
                textBlock.Inlines.Add(new Run("⏹") { Foreground = Brushes.Red });
            }
            else
            {
                textBlock.Inlines.Add(new Run("Start Streaming ") { Foreground = Brushes.White });
                textBlock.Inlines.Add(new Run("▶️") { Foreground = Brushes.LimeGreen });
            }
            return textBlock;
        }
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value?.ToString()?.ToLower() == "ON";
        }
    }
    public class RecordButtonConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            TextBlock textBlock = new();
            if ((bool)value)
            {
                textBlock.Inlines.Add(new Run("Stop Recordnig ") { Foreground = Brushes.White });
                textBlock.Inlines.Add(new Run("⏹") { Foreground = Brushes.Red });
            }
            else
            {
                textBlock.Inlines.Add(new Run("Start Recordnig ") { Foreground = Brushes.White });
                textBlock.Inlines.Add(new Run("▶️") { Foreground = Brushes.LimeGreen });
            }
            return textBlock;
        }
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value?.ToString()?.ToLower() == "ON";
        }
    }
    public class UARTButtonConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            TextBlock textBlock = new();
            if ((bool)value)
            {
                textBlock.Inlines.Add(new Run("Disconnect UART") { Foreground = Brushes.White });
            }
            else
            {
                textBlock.Inlines.Add(new Run("Connect UART") { Foreground = Brushes.White });
            }
            return textBlock;
        }
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value?.ToString()?.ToLower() == "ON";
        }
    }
    public class LedConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)value ? Visibility.Visible : Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (Visibility)value == Visibility.Visible;
        }
    }
    public class ConnectIndicatorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)value ? Visibility.Visible : Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (Visibility)value == Visibility.Visible;
        }
    }
    internal class Converter
    {
    }
}
