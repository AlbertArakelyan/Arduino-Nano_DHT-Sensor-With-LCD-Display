#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define LED_PIN 3
#define DHT_PIN 2

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT11);

void setup() {
  Serial.begin(115200);

  delay(5000);

  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("DHT11 Starting..");
}

void loop() {
  delay(10000); // Update every 10 seconds
  printTemperatureAndHumidityToLCD();
}

// Function definitions
void printTemperatureAndHumidityToLCD() {
  float temperature = readDHTTemperature();
  float humidity = readDHTHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  if (temperature != -1 && humidity != -1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
  }
}

void turnOnBlueLedAtLowTemperature(float temp) {
  if (temp < 20) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

float readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }

  turnOnBlueLedAtLowTemperature(t);

  Serial.println(t);
  return t;
}

float readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }

  Serial.println(h);
  return h;
}