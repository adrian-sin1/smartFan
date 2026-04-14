#include <DHT11.h>             
#include <LiquidCrystal_I2C.h>    

LiquidCrystal_I2C lcd(0x27, 16, 3); 

#define tempsensor 2  

#define FAN 7
#define RED_LIGHT 6
#define BLUE_LIGHT 5
#define Increase 13
#define Decrease 12

DHT11 dht(tempsensor);  

int threshold = 76; 
int pressed = 0;

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
  Serial.println(threshold); 

  int increaseButton = digitalRead(Increase);
  int decreaseButton = digitalRead(Decrease);

  delay(10);


  if (decreaseButton == pressed) {
    threshold -= 1;
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Set Temperature");
    lcd.setCursor(0, 1);  
    lcd.print(threshold);
    lcd.print(" F");
    delay(1500);
    lcd.clear();
  }

  if (increaseButton == pressed) {
    threshold += 1;
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Set Temperature");
    lcd.setCursor(0, 1);  
    lcd.print(threshold);
    lcd.print(" F");
    delay(1500);
    lcd.clear();
  }


  float temperatureCelsius = dht.readTemperature();  
  float temperatureFahrenheit = (temperatureCelsius * 9.0 / 5.0) + 32.0; 
  int temperature = (int)temperatureFahrenheit;  

  float humidity = dht.readHumidity();  

  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println("°F");
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println("%");


  if (temperature > threshold) {
    lcd.clear();
    lcd.setCursor(0, 0);  
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" F");

    digitalWrite(FAN, HIGH);
    digitalWrite(RED_LIGHT, LOW);
    digitalWrite(BLUE_LIGHT, HIGH);
    Serial.println("Fan ON");
    lcd.setCursor(0, 1);  
    lcd.print("Fan On");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);  
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" F");

    digitalWrite(FAN, LOW);
    digitalWrite(RED_LIGHT, HIGH);
    digitalWrite(BLUE_LIGHT, LOW);
    Serial.println("Fan OFF");
    lcd.setCursor(0, 1);  
    lcd.print("Fan OFF");
  }

}