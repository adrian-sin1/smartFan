#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

// Decision logic for the smart fan, kept free of Arduino calls so it builds
// with any C++ compiler: avr-gcc for the board, g++/clang++ for the tests.
// No standard library headers here, because avr-gcc ships without libstdc++.

enum class Led { Red, Blue };

class FanController {
public:
  static const int kDefaultThresholdF = 76;
  static const int kMinThresholdF = 50;
  static const int kMaxThresholdF = 95;

  // The DHT11 measures 0-50 C. Anything outside that is a failed read
  // (the DHT11 library returns error codes such as 253 and 254).
  static const int kSensorMinC = 0;
  static const int kSensorMaxC = 50;

  // The fan turns on when the temperature rises above the threshold and turns
  // off once it drops to (threshold - hysteresis) or lower. A hysteresis of 0
  // gives a plain on/off comparison; 1 stops the fan flickering at the edge.
  explicit FanController(int thresholdF = kDefaultThresholdF, int hysteresisF = 1);

  void increaseThreshold();
  void decreaseThreshold();
  int threshold() const;

  // Takes a raw sensor reading in Celsius. Returns false on a bad reading,
  // in which case the fan keeps its previous state.
  bool update(float tempC);

  bool fanOn() const;
  Led led() const;
  int temperatureF() const;
  bool sensorError() const;

  static int celsiusToFahrenheit(float tempC);

private:
  static int clampThreshold(int thresholdF);

  int threshold_;
  int hysteresis_;
  int temperatureF_;
  bool fanOn_;
  bool sensorError_;
};

#endif
