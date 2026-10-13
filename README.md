# Smart Fan System
The Smart Fan System is an Arduino project that turns a fan on and off based on the room temperature. On start-up, the preferred temperature is set to 76 degrees Fahrenheit.

If the DHT11 sensor detects the temperature rising above the threshold, the Arduino will turn on the blue LED and display 'Fan On' on the screen. A fan will also turn on to cool the air around the sensor. To test this feature, the user can wrap their hand around the sensor to heat it.

Once the temperature drops back down, the Arduino will turn the fan off, turn on the red LED, and display 'Fan Off' on the screen. The fan stays on until the temperature is 1 degree below the threshold (hysteresis), so it doesn't flicker on and off when the temperature sits right at the threshold. With the default 76°F threshold, the fan turns on at 77°F and off at 75°F. To test this feature, the user can wait for the fan to cool down the temperature around the sensor.

If the sensor returns a bad reading, the screen shows 'Sensor error' and the fan keeps its previous state instead of reacting to garbage data.

If the user would like to adjust the preferred temperature, they can with the two buttons located on the breadboard. The set temperature is limited to 50–95°F.

The breadboard is split into two sides, one side where the LEDs are located and the buttons on the other. With the button side facing towards the user, the left button would allow the user to lower the set temperature, and the right button would allow the user to raise the set temperature.

# Project Structure
The code is split into hardware and logic, a common embedded pattern (hardware abstraction):

```
code.ino                 Hardware layer: reads the sensor and buttons, drives the fan, LEDs, and LCD
src/FanController.h      Decision logic: threshold, hysteresis, fan on/off, LED color
src/FanController.cpp    (plain C++, no Arduino calls)
tests/test_fan.cpp       Unit tests for FanController, run on a PC
Makefile                 Builds and runs the tests with g++ or clang++
```

`FanController` has no Arduino dependencies, so the same code compiles with avr-gcc for the board and with g++/clang++ on a desktop. That makes the logic testable without any hardware.

# Set Up (Arduino)
Download the two libraries needed to run the code for this project.

Temperature Sensor (DHT11): https://github.com/dhrubasaha08/DHT11

LCD Screen (Liquid Crystal I2C): https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

Locate this project's 'Libraries' folder in your local files and insert the files you downloaded.

Open the Arduino IDE. The IDE compiles the files in `src/` along with the sketch automatically.

Connect your computer with the Arduino, verify the code, and upload it.

# Running the Tests (PC)
The tests need a C++ compiler and `make` (on Windows, use MSYS2 or WSL).

```
make test          # build and run with g++
make test-clang    # build and run with clang++
make test-all      # both compilers
make debug         # open the test binary in gdb
make clean
```

Or without make:

```
g++ -std=c++17 -Wall -Wextra -g src/FanController.cpp tests/test_fan.cpp -o test_fan
./test_fan
```

The tests also run on every push through GitHub Actions with both g++ and clang++ (`.github/workflows/tests.yml`).
