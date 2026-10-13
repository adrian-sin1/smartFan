#include <DHT11.h>
#include <LiquidCrystal_I2C.h>
#include "src/FanController.h"

LiquidCrystal_I2C lcd(0x27, 16, 3);

#define tempsensor 2

#define FAN 7
#define RED_LIGHT 6
#define BLUE_LIGHT 5
#define Increase 13
#define Decrease 12

DHT11 dht(tempsensor);

FanController controller;
int pressed = 0;

void showSetTemperature() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Temperature");
  lcd.setCursor(0, 1);
  lcd.print(controller.threshold());
  lcd.print(" F");
  delay(1500);
  lcd.clear();
}

void setup() {
  lcd.begin();

  pinMode(FAN, OUTPUT);
  pinMode(Increase, INPUT_PULLUP);
  pinMode(Decrease, INPUT_PULLUP);

  pinMode(RED_LIGHT, OUTPUT);
  pinMode(BLUE_LIGHT, OUTPUT);
  digitalWrite(FAN, LOW);

  Serial.begin(9600);
}

void loop() {
  Serial.println(controller.threshold());

  int increaseButton = digitalRead(Increase);
  int decreaseButton = digitalRead(Decrease);

  delay(10);


  if (decreaseButton == pressed) {
    controller.decreaseThreshold();
    showSetTemperature();
  }

  if (increaseButton == pressed) {
    controller.increaseThreshold();
    showSetTemperature();
  }


  float temperatureCelsius = dht.readTemperature();
  float humidity = dht.readHumidity();

  bool validReading = controller.update(temperatureCelsius);

  if (validReading) {
    Serial.print("Temperature = ");
    Serial.print(controller.temperatureF());
    Serial.println("°F");
    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println("%");
  } else {
    Serial.println("Sensor read failed");
  }


  digitalWrite(FAN, controller.fanOn() ? HIGH : LOW);
  digitalWrite(RED_LIGHT, controller.led() == Led::Red ? HIGH : LOW);
  digitalWrite(BLUE_LIGHT, controller.led() == Led::Blue ? HIGH : LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  if (validReading) {
    lcd.print("Temp: ");
    lcd.print(controller.temperatureF());
    lcd.print(" F");
  } else {
    lcd.print("Sensor error");
  }

  lcd.setCursor(0, 1);
  if (controller.fanOn()) {
    Serial.println("Fan ON");
    lcd.print("Fan On");
  } else {
    Serial.println("Fan OFF");
    lcd.print("Fan Off");
  }

}
