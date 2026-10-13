// Host-side unit tests for FanController. No framework needed:
//   make test        (g++)
//   make test-clang  (clang++)

#include "../src/FanController.h"

#include <cmath>
#include <cstdio>

static int failures = 0;
static int checks = 0;

#define CHECK(cond)                                                    \
  do {                                                                 \
    ++checks;                                                          \
    if (!(cond)) {                                                     \
      ++failures;                                                      \
      std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);      \
    }                                                                  \
  } while (0)

// Celsius values that land on whole Fahrenheit degrees, for readable tests.
static float fToC(int f) {
  return (f - 32) * 5.0f / 9.0f + 0.01f;
}

static void test_defaults() {
  FanController c;
  CHECK(c.threshold() == 76);
  CHECK(!c.fanOn());
  CHECK(c.led() == Led::Red);
  CHECK(!c.sensorError());
}

static void test_celsius_to_fahrenheit_truncates() {
  CHECK(FanController::celsiusToFahrenheit(0.0f) == 32);
  CHECK(FanController::celsiusToFahrenheit(25.0f) == 77);
  CHECK(FanController::celsiusToFahrenheit(24.5f) == 76);  // 76.1 -> 76
  CHECK(FanController::celsiusToFahrenheit(24.0f) == 75);  // 75.2 -> 75
  CHECK(FanController::celsiusToFahrenheit(50.0f) == 122);
}

static void test_above_threshold_turns_fan_on() {
  FanController c;
  CHECK(c.update(fToC(77)));
  CHECK(c.temperatureF() == 77);
  CHECK(c.fanOn());
  CHECK(c.led() == Led::Blue);
}

static void test_equal_to_threshold_does_not_turn_fan_on() {
  FanController c;
  c.update(fToC(76));
  CHECK(!c.fanOn());
  CHECK(c.led() == Led::Red);
}

static void test_below_threshold_keeps_fan_off() {
  FanController c;
  c.update(fToC(70));
  CHECK(!c.fanOn());
}

static void test_hysteresis_holds_fan_on_at_threshold() {
  FanController c(76, 1);
  c.update(fToC(77));
  CHECK(c.fanOn());
  c.update(fToC(76));  // inside the band: stay on
  CHECK(c.fanOn());
  c.update(fToC(75));  // threshold - hysteresis: turn off
  CHECK(!c.fanOn());
  c.update(fToC(76));  // inside the band: stay off
  CHECK(!c.fanOn());
}

static void test_zero_hysteresis_matches_original_sketch() {
  FanController c(76, 0);
  c.update(fToC(77));
  CHECK(c.fanOn());
  c.update(fToC(76));
  CHECK(!c.fanOn());
}

static void test_wider_hysteresis_band() {
  FanController c(76, 3);
  c.update(fToC(80));
  CHECK(c.fanOn());
  c.update(fToC(74));
  CHECK(c.fanOn());
  c.update(fToC(73));
  CHECK(!c.fanOn());
}

static void test_negative_hysteresis_is_treated_as_zero() {
  FanController c(76, -5);
  c.update(fToC(77));
  c.update(fToC(76));
  CHECK(!c.fanOn());
}

static void test_buttons_adjust_threshold() {
  FanController c;
  c.increaseThreshold();
  CHECK(c.threshold() == 77);
  c.decreaseThreshold();
  c.decreaseThreshold();
  CHECK(c.threshold() == 75);
}

static void test_raising_threshold_changes_decision() {
  FanController c(76, 0);
  c.update(fToC(77));
  CHECK(c.fanOn());
  c.increaseThreshold();  // now 77
  c.update(fToC(77));
  CHECK(!c.fanOn());
}

static void test_threshold_is_clamped() {
  FanController high(FanController::kMaxThresholdF);
  high.increaseThreshold();
  CHECK(high.threshold() == FanController::kMaxThresholdF);

  FanController low(FanController::kMinThresholdF);
  low.decreaseThreshold();
  CHECK(low.threshold() == FanController::kMinThresholdF);

  FanController tooHigh(200);
  CHECK(tooHigh.threshold() == FanController::kMaxThresholdF);
}

static void test_bad_reading_keeps_previous_state() {
  FanController c;
  c.update(fToC(80));
  CHECK(c.fanOn());

  CHECK(!c.update(253.0f));  // DHT11 library timeout code
  CHECK(c.sensorError());
  CHECK(c.fanOn());
  CHECK(c.temperatureF() == 80);

  CHECK(!c.update(-1.0f));
  CHECK(!c.update(NAN));
  CHECK(c.fanOn());

  CHECK(c.update(fToC(70)));
  CHECK(!c.sensorError());
  CHECK(!c.fanOn());
}

static void test_sensor_range_edges_are_valid() {
  FanController c;
  CHECK(c.update(0.0f));
  CHECK(c.update(50.0f));
  CHECK(!c.update(50.1f));
}

int main() {
  test_defaults();
  test_celsius_to_fahrenheit_truncates();
  test_above_threshold_turns_fan_on();
  test_equal_to_threshold_does_not_turn_fan_on();
  test_below_threshold_keeps_fan_off();
  test_hysteresis_holds_fan_on_at_threshold();
  test_zero_hysteresis_matches_original_sketch();
  test_wider_hysteresis_band();
  test_negative_hysteresis_is_treated_as_zero();
  test_buttons_adjust_threshold();
  test_raising_threshold_changes_decision();
  test_threshold_is_clamped();
  test_bad_reading_keeps_previous_state();
  test_sensor_range_edges_are_valid();

  std::printf("%d/%d checks passed\n", checks - failures, checks);
  return failures == 0 ? 0 : 1;
}
