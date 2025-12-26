using System.Windows;
using System.Windows.Input;
using System.Windows.Controls;
#nullable enable
namespace Smart_Home_GUI.View
{
    /// <summary>
    /// Interaction logic for KeyPad.xaml
    /// </summary>
    public partial class KeyPad : UserControl
    {
        public static readonly DependencyProperty PressCommandProperty =
            DependencyProperty.Register(
                nameof(PressCommand),
                typeof(ICommand),
                typeof(KeyPad));
        public ICommand? PressCommand
        {
            get => (ICommand?)GetValue(PressCommandProperty);
            set => SetValue(PressCommandProperty, value);
        }
        public static readonly DependencyProperty PressedValueProperty =
            DependencyProperty.Register(
                nameof(PressedValue),
                typeof(string),
                typeof(KeyPad),
                new PropertyMetadata(""));

        public string PressedValue
        {
            get => (string)GetValue(PressedValueProperty);
            set => SetValue(PressedValueProperty, value);
        }
        public KeyPad()
        {
            InitializeComponent();
        }
    }
}
