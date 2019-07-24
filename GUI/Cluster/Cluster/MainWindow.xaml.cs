using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Cluster {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window {
        // Global state variables //
        private bool headlightsState = false;
        private bool seatbeltState = false;

        // Global icon images //
        private BitmapImage headlightsOn;
        private BitmapImage headligtsOff;
        private BitmapImage seatbeltBuckled;
        private BitmapImage seatbeltUnbuckled;

        public MainWindow() {
            InitializeComponent();

            // Remove resize of the window //
            window.ResizeMode = ResizeMode.NoResize;

            // Initialize images for the UI //
            headlightsOn = new BitmapImage(new Uri("C:/1.png"));
            headligtsOff = new BitmapImage(new Uri("C:/2.png"));
            seatbeltBuckled = new BitmapImage(new Uri("C:/seatbelt_buckled.png"));
            seatbeltUnbuckled = new BitmapImage(new Uri("C:/seatbelt_unbuckled.png"));

            // Bind events to buttons & sliders //
            headlightsButton.Click += HeadlightsButtonClicked;
            seatbeltButton.Click += SeatbeltButtonClicked;
            speedSlider.ValueChanged += ValueChanged;

            // Initialize component content //
            seatbeltButton.Content = " Seatbelt:\nUnbuckled";
        }

        // Seatbet button event handler //
        private void SeatbeltButtonClicked(object sender, RoutedEventArgs e) {
            ChangeSeatbeltState();
        }

        // Slider value changed event handler //
        private void ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e) {
            ChangeSpeedLabel();
        }

        // Headlights button event handler //
        private void HeadlightsButtonClicked(object sender, RoutedEventArgs e) {
            ChangeHeadLightsState();
        }

        // Updates seatbelt icon //
        private void ChangeSeatbeltState() {
            seatbelt.Source = (seatbeltState) ? seatbeltUnbuckled : seatbeltBuckled;
            seatbeltButton.Content = (seatbeltState) ? " Seatbelt:\nUnbuckled" : "Seatbelt:\n Buckled";
            seatbeltState = !seatbeltState;
        }

        // Updates headlights icons // 
        private void ChangeHeadLightsState() {
            bulbLeft.Source = (headlightsState) ? headligtsOff : headlightsOn;
            bulbRight.Source = (headlightsState) ? headligtsOff : headlightsOn;

            headlightsState = !headlightsState;

            headlightsButton.Content = (headlightsState) ? "Headlights: On" : "Headlights: Off";
        }

        // Update speed label content //
        private void ChangeSpeedLabel() {
            speedLabel.Content = $"{(int)speedSlider.Value} km/h";
        }
    }
}
