using System;
using System.Threading;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Cluster {
    public partial class MainWindow : Window {
        // Global variables //
        private const string resourcePath = "pack://application:,,,/Resources/";
        private int startAngle = -125; // start angle of the speedometer arrow
        private const int blinkDelay = 500; // time interval between each blink of the blinkers
        private uint mask = 0;
        private int speed = 0;

        // Global state variables //
        private bool headlightsState = false; // headlights state - on/off
        private bool seatbeltState = false; // seatbelt state - buckled/unbuckled
        private int currentBlinker = -1; // which blinkers are working currently: 0 -> left, 1 -> right, -1 -> none
        private bool batteryLevelState = false; // battery level state - normal/warning
        private bool engineTempState = false; // engine temp state - normal/warning
        private bool fuelLevelState = false; // fuel level state - normal/warning
        private uint blinkerState = 0;
        private bool hornButtonState = false;
        private string curr_time = DateTime.Now.ToString("HH:mm");

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
        private BitmapImage hornImage;

        // Blink lights thread //
        Thread blink;

        // CANManager instance //
        CANManager can_manager;

        public MainWindow() {
            InitializeComponent();

            window.ResizeMode = ResizeMode.NoResize; // Disable resize of the window

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
            hornImage = new BitmapImage(new Uri(resourcePath + "horn_button.png"));

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
            hornButton.Click += HornButtonClicked;
            window.Closed += WindowClosed;

            // Initialize thread for blinkers //
            blink = new Thread(CheckBlink);
            blink.Start();

            // Initialize the can manager //
            can_manager = new CANManager();

            System.Timers.Timer timer = new System.Timers.Timer(200);
            timer.Elapsed += CANManager.TimerHandler;
            timer.AutoReset = true;
            timer.Enabled = true;
        }

        /// <summary>
        /// Horn button clicked event handler
        /// </summary>
        private void HornButtonClicked(object sender, RoutedEventArgs e)
        {
            hornButtonState = !hornButtonState;
            if (hornButtonState)
            {
                ScaleTransform scaleTransform = new ScaleTransform(1.1, 1.1, 256, 256);
                horn.RenderTransform = scaleTransform;
            } else
            {
                ScaleTransform scaleTransform = new ScaleTransform(0.99, 0.99);
                horn.RenderTransform = scaleTransform;
            }
        }

        /// <summary>
        /// Fuel level button clicked event handler
        /// </summary>
        private void FuelLevelClicked(object sender, RoutedEventArgs e) {
            fuelLevelState = !fuelLevelState;
            fuelLevel.Source = (!fuelLevelState) ? fuelLevelNormal : fuelLevelWarning;
            CalculateMaskAndSpeed();
        }

        /// <summary>
        /// Calculates masks and speed for the CAN Manager
        /// </summary>
        private void CalculateMaskAndSpeed()
        {
            mask = 0;
            speed = (int)speedSlider.Value;

            if(currentBlinker == 0)
            {
                mask = mask | blinkerState;
                mask = mask << 1;
                mask = mask | 0;
            }

            if (currentBlinker == 1)
            {
                mask = mask | 0;
                mask = mask << 1;
                mask = mask | blinkerState;
            }

            mask = mask << 1;
            mask = mask | Convert.ToUInt32(engineTempState);

            mask = mask << 1;
            mask = mask | Convert.ToUInt32(batteryLevelState);

            mask = mask << 1;
            mask = mask | Convert.ToUInt32(headlightsState);

            mask = mask << 1;
            mask = mask | Convert.ToUInt32(!seatbeltState);

            mask = mask << 1;
            mask = mask | Convert.ToUInt32(fuelLevelState);

            mask = mask << 1;

            curr_time = DateTime.Now.ToString("HH:mm");

            CANManager.SetInfo(mask, speed, Convert.ToInt32(hornButtonState), int.Parse(curr_time.Split(':')[0]), int.Parse(curr_time.Split(':')[1]));
        }

        /// <summary>
        /// Engine temperature button clicked event handler
        /// </summary>
        private void EngineTemperatureClicked(object sender, RoutedEventArgs e) {
            engineTempState = !engineTempState;
            engineTemperature.Source = (!engineTempState) ? engineTemperatureNormal : engineTemperatureWarning;
            CalculateMaskAndSpeed();
        }

        /// <summary>
        /// Battery level clicked event handler
        /// </summary>
        private void BatteryLevelClicked(object sender, RoutedEventArgs e) {
            batteryLevelState = !batteryLevelState;
            batteryLevel.Source = (!batteryLevelState) ? batteryLevelNormal : batteryLevelWarning;
            CalculateMaskAndSpeed();
        }

        /// <summary>
        /// Window closed event handler
        /// </summary>
        private void WindowClosed(object sender, EventArgs e) {
            blink.Abort(); // stops blink thread
            CANManager.ResetCluster();
        }

        /// <summary>
        /// Turn left button event handler
        /// </summary>
        private void TurnLeftClicked(object sender, RoutedEventArgs e) {
            if (currentBlinker == -1 || currentBlinker == 1) { // Check if currently the right or none are working
                currentBlinker = 0; // set the current blinker to left
            } else {
                currentBlinker = -1; // set the current blinker to none
            }

            CalculateMaskAndSpeed();
        }

        /// <summary>
        /// Turn right button event handler 
        /// </summary>
        private void TurnRightClicked(object sender, RoutedEventArgs e) {
            if (currentBlinker == -1 || currentBlinker == 0) { // Check if currently the left or none blinkers are working
                currentBlinker = 1; // set the current blinker to right
            } else {
                currentBlinker = -1; // set the current blinker to none
            }

            CalculateMaskAndSpeed();
        }

        /// <summary>
        /// Seatbet button event handler
        /// </summary>
        private void SeatbeltButtonClicked(object sender, RoutedEventArgs e) {
            seatbelt.Source = (seatbeltState) ? seatbeltUnbuckled : seatbeltBuckled;
            seatbeltButton.Content = (seatbeltState) ? " Seatbelt:\nUnbuckled" : "Seatbelt:\n Buckled";
            seatbeltState = !seatbeltState;

            CalculateMaskAndSpeed();
        }

        /// <summary>
        /// Slider value changed event handler
        /// </summary>
        private void ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e) {
            speedLabel.Content = $"{(int)speedSlider.Value} km/h";
            RotateArrow((int)speedSlider.Value);

            CalculateMaskAndSpeed();
        }

        /// <summary>
        /// Headlights button event handler
        /// </summary>
        private void HeadlightsButtonClicked(object sender, RoutedEventArgs e) {
            bulbLeft.Source = (headlightsState) ? headligtsOff : headlightsOn;
            headlightsState = !headlightsState;
            headlightsButton.Content = (headlightsState) ? "Headlights: On" : "Headlights: Off";

            CalculateMaskAndSpeed();
        }

        /// <summary>
        /// Rotates speedometer dial by a certain degree
        /// </summary>
        /// <param name="speed"></param>
        private void RotateArrow(int speed) {
            RotateTransform rotateTransform = new RotateTransform(speed + startAngle);
            speedometer_arrow.RenderTransform = rotateTransform;
        }

        /// <summary>
        /// Blink thread's job
        /// </summary>
        private void CheckBlink() {
            while (true) {
                this.Dispatcher.Invoke(() => {
                    if (currentBlinker == 0) {
                        blinkLeft.Source = turnLightLeftGreen;
                        blinkRight.Source = turnLightRight;
                        blinkerState = 1;
                    } else if (currentBlinker == 1) {
                        blinkLeft.Source = turnLightLeft;
                        blinkRight.Source = turnLightRightGreen;
                        blinkerState = 1;
                    } else {
                        blinkLeft.Source = turnLightLeft;
                        blinkRight.Source = turnLightRight;
                        blinkerState = 0;
                    }
                    CalculateMaskAndSpeed();
                });

                Thread.Sleep(blinkDelay);

                this.Dispatcher.Invoke(() => {
                    blinkLeft.Source = turnLightLeft;
                    blinkRight.Source = turnLightRight;
                    blinkerState = 0;
                    CalculateMaskAndSpeed();
                });

                Thread.Sleep(blinkDelay);

            }
        }
    }
}
