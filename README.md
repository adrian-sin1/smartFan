# Set Up
Download the two libraries needed to run the code for this project.

Temperature Sensor (DHT11): https://github.com/dhrubasaha08/DHT11

LCD Screen (Liquid Crystal I2C): https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

Locate this project's 'Libraries' folder in your local files and insert the files you downloaded.

Open the Arduino IDE.

Connect your computer with the Arduino, verify the code, and upload it.

# Smart Fan System
The Smart Fan System works autonomously for the most part. On start-up, the preferred temperature is set to 76 degrees Fahrenheit.

If the DHT11 sensor detects the temperature dropping below the threshold, the Arduino will turn on the red LED and display that the 'Fan OFF' on the screen. To test this feature, the user can wait for the fan to cool down the temperature around the sensor.

Otherwise, if the sensor detects the temperature rising above the threshold, the Arduino will turn on the blue LED and display that the 'fan is on' on the screen. A fan will also turn on to cool the air around the sensor. To test this feature, the user can wrap their hand around the sensor to heat it.

If the user would like to adjust the preferred temperature, they can with the two buttons located on the breadboard.

The breadboard is split into two sides, one side where the LEDs are located and the buttons on the other. With the button side facing towards the user, the left button would allow the user to lower the set temperature, and the right button would allow the user to raise the set temperature.