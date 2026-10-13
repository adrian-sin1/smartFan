#include "FanController.h"

FanController::FanController(int thresholdF, int hysteresisF)
    : threshold_(clampThreshold(thresholdF)),
      hysteresis_(hysteresisF < 0 ? 0 : hysteresisF),
      temperatureF_(0),
      fanOn_(false),
      sensorError_(false) {}

void FanController::increaseThreshold() {
  threshold_ = clampThreshold(threshold_ + 1);
}

void FanController::decreaseThreshold() {
  threshold_ = clampThreshold(threshold_ - 1);
}

int FanController::threshold() const {
  return threshold_;
}

bool FanController::update(float tempC) {
  // Written so a NaN reading also fails the check.
  if (!(tempC >= kSensorMinC && tempC <= kSensorMaxC)) {
    sensorError_ = true;
    return false;
  }

  sensorError_ = false;
  temperatureF_ = celsiusToFahrenheit(tempC);

  if (temperatureF_ > threshold_) {
    fanOn_ = true;
  } else if (temperatureF_ <= threshold_ - hysteresis_) {
    fanOn_ = false;
  }
  return true;
}

bool FanController::fanOn() const {
  return fanOn_;
}

Led FanController::led() const {
  return fanOn_ ? Led::Blue : Led::Red;
}

int FanController::temperatureF() const {
  return temperatureF_;
}

bool FanController::sensorError() const {
  return sensorError_;
}

int FanController::celsiusToFahrenheit(float tempC) {
  // Truncates to whole degrees, same as the original sketch.
  return (int)(tempC * 9.0f / 5.0f + 32.0f);
}

int FanController::clampThreshold(int thresholdF) {
  if (thresholdF < kMinThresholdF) return kMinThresholdF;
  if (thresholdF > kMaxThresholdF) return kMaxThresholdF;
  return thresholdF;
}
