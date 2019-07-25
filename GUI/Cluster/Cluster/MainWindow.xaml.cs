using System;
using System.Threading;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Cluster {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window {
        // Global variables //
        private string resourcePath = "C:/Resources/";
        private int startAngle = -125; // start angle of the speedometer arrow
        private const int blinkDelay = 1000; // time interval between each blink of the blinkers

        // Global state variables //
        private bool headlightsState = false; // headlights state - on/off
        private bool seatbeltState = false; // seatbelt state - buckled/unbuckled
        private int currentBlinker = -1; // which blinkers are working currently: 0 -> left, 1 -> right, -1 -> none
        private bool batteryLevelState = false; // battery level state - normal/warning
        private bool engineTempState = false; // engine temp state - normal/warning
        private bool fuelLevelState = false; // fuel level state - normal/warning

        // Global icon images //
        private BitmapImage headlightsOn; // headlights turned on
        private BitmapImage headligtsOff; // headlights turned off
        private BitmapImage seatbeltBuckled; // buckled seatbelt
        private BitmapImage seatbeltUnbuckled; // unbuckled seatbelt
        private BitmapImage turnLightLeft; // left blinker off
        private BitmapImage turnLightLeftGreen; // left blinker on
        private BitmapImage turnLightRight; // right blinker off
        private BitmapImage turnLightRightGreen; // left blinker on
        private BitmapImage batteryLevelNormal; // battery level normal
        private BitmapImage batteryLevelWarning; // battery level warning
        private BitmapImage engineTemperatureNormal; // engine temperature normal
        private BitmapImage engineTemperatureWarning; // engine temperature warning
        private BitmapImage fuelLevelNormal; // fuel level normal
        private BitmapImage fuelLevelWarning; // fuel level warning

        Thread blink;

        public MainWindow() {
            InitializeComponent();
            window.ResizeMode = ResizeMode.NoResize; // Remove resize of the window 

            // Initialize images for the UI //
            headlightsOn = new BitmapImage(new Uri(resourcePath + "1.png"));
            headligtsOff = new BitmapImage(new Uri(resourcePath + "2.png"));
            seatbeltBuckled = new BitmapImage(new Uri(resourcePath + "seatbelt_buckled.png"));
            seatbeltUnbuckled = new BitmapImage(new Uri(resourcePath + "seatbelt_unbuckled.png"));
            turnLightLeft = new BitmapImage(new Uri(resourcePath + "turn_light_left.png"));
            turnLightLeftGreen = new BitmapImage(new Uri(resourcePath + "turn_light_left_green.png"));
            turnLightRight = new BitmapImage(new Uri(resourcePath + "turn_light_right.png"));
            turnLightRightGreen = new BitmapImage(new Uri(resourcePath + "turn_light_right_green.png"));
            batteryLevelNormal = new BitmapImage(new Uri(resourcePath + "battery_level.png"));
            batteryLevelWarning = new BitmapImage(new Uri(resourcePath + "battery_level_red.png"));
            engineTemperatureNormal = new BitmapImage(new Uri(resourcePath + "engine_temp.png"));
            engineTemperatureWarning = new BitmapImage(new Uri(resourcePath + "engine_temp_red.png"));
            fuelLevelNormal = new BitmapImage(new Uri(resourcePath + "fuel_level.png"));
            fuelLevelWarning = new BitmapImage(new Uri(resourcePath + "fuel_level_red.png"));

            // Set speedometer arrow position intially //
            RotateTransform rotateTransform = new RotateTransform(startAngle);
            speedometer_arrow.RenderTransform = rotateTransform;

            // Bind events to buttons & sliders //
            headlightsButton.Click += HeadlightsButtonClicked;
            seatbeltButton.Click += SeatbeltButtonClicked;
            speedSlider.ValueChanged += ValueChanged;
            turnLeftButton.Click += TurnLeftClicked;
            turnRightButton.Click += TurnRightClicked;
            batteryLevelButton.Click += BatteryLevelClicked;
            engineTemperatureButton.Click += EngineTemperatureClicked;
            fuelLevelButton.Click += FuelLevelClicked;
            window.Closed += WindowClosed;

            // Initialize thread for blinkers //
            blink = new Thread(CheckBlink);
            blink.Start();
        }

        // Fuel level button clicked event handler //
        private void FuelLevelClicked(object sender, RoutedEventArgs e) {
            fuelLevelState = !fuelLevelState;
            fuelLevel.Source = (!fuelLevelState) ? fuelLevelNormal : fuelLevelWarning;
        }

        // Engine temperature button clicked event handler //
        private void EngineTemperatureClicked(object sender, RoutedEventArgs e) {
            engineTempState = !engineTempState;
            engineTemperature.Source = (!engineTempState) ? engineTemperatureNormal : engineTemperatureWarning;
        }

        // Battery level clicked event handler //
        private void BatteryLevelClicked(object sender, RoutedEventArgs e) {
            batteryLevelState = !batteryLevelState;
            batteryLevel.Source = (!batteryLevelState) ? batteryLevelNormal : batteryLevelWarning;
        }

        // Window closed event handler //
        private void WindowClosed(object sender, EventArgs e) {
            blink.Abort(); // stops blink thread
        }

        // Turn left button event handler //
        private void TurnLeftClicked(object sender, RoutedEventArgs e) {
            if (currentBlinker == -1 || currentBlinker == 1) { // Check if currently the right or none are working
                currentBlinker = 0; // set the current blinker to left
            } else {
                currentBlinker = -1; // set the current blinker to none
            }
        }

        // Turn right button event handler //
        private void TurnRightClicked(object sender, RoutedEventArgs e) {
            if (currentBlinker == -1 || currentBlinker == 0) { // Check if currently the left or none blinkers are working
                currentBlinker = 1; // set the current blinker to right
            } else {
                currentBlinker = -1; // set the current blinker to none
            }
        }

        // Seatbet button event handler //
        private void SeatbeltButtonClicked(object sender, RoutedEventArgs e) {
            ChangeSeatbeltState();
        }

        // Slider value changed event handler //
        private void ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e) {
            ChangeSpeedLabel();
            RotateArrow((int)speedSlider.Value);
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
            headlightsState = !headlightsState;
            headlightsButton.Content = (headlightsState) ? "Headlights: On" : "Headlights: Off";
        }

        // Update speed label content //
        private void ChangeSpeedLabel() {
            speedLabel.Content = $"{(int)speedSlider.Value} km/h";
        }

        // Rotates speedometer dial by a certain degree //
        private void RotateArrow(int speed) {
            RotateTransform rotateTransform = new RotateTransform(speed + startAngle);
            speedometer_arrow.RenderTransform = rotateTransform;
        }

        // Blink thread's job //
        private void CheckBlink() {
            while (true) {
                this.Dispatcher.Invoke(() => {
                    if (currentBlinker == 0) {
                        blinkLeft.Source = turnLightLeftGreen;
                        blinkRight.Source = turnLightRight;
                    } else if (currentBlinker == 1) {
                        blinkLeft.Source = turnLightLeft;
                        blinkRight.Source = turnLightRightGreen;
                    } else {
                        blinkLeft.Source = turnLightLeft;
                        blinkRight.Source = turnLightRight;
                    }
                });

                Thread.Sleep(blinkDelay);

                this.Dispatcher.Invoke(() => {
                    blinkLeft.Source = turnLightLeft;
                    blinkRight.Source = turnLightRight;
                });

                Thread.Sleep(blinkDelay);
            }
        }
    }
}
