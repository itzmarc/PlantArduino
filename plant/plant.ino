#include "DHT.h"
#include<LiquidCrystal.h>
#define DHTPIN 13  
#define DHTTYPE DHT22 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);

// PINS
const int motorPin_PIN = 9;
const int soilMoistureSensor_PIN = A3;

const int wet = 250;
const int dry = 500;
const int moistureLevel = (dry - wet) / 2;
const int startWatering = 400; 
const int stopWatering = 300; 
boolean watering = false;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  dht.begin();
  pinMode(motorPin_PIN, OUTPUT);
}

void loop() {
  long soilMoisture = analogRead(soilMoistureSensor_PIN);
  float humidity = getHumid();
  float temperature = getTemp();
  Serial.println("Soil Status: " + readSoilMoisture(soilMoisture));
  lcd.setCursor(0, 0);
  Serial.println("Humidity: " + String(humidity) + " %");
  lcd.print(String("Humid: ") + String(humidity) + String( " %"));
  lcd.setCursor(0, 1);
  Serial.println("Temperature: " + String(temperature) + " F");
  lcd.print(String("Temp: ") + String(temperature) + String( " F"));
  waterPlants(soilMoisture);
  delay(30000); //interval between each sensor read (30 seconds)
}

float getHumid() {
  float humidity = dht.readHumidity();
  return humidity;
}
float getTemp() {
  float temperature = dht.readTemperature(true);
  return temperature;
}

void waterPlants(int soilMoisture) { //example from online
  waterCheck(soilMoisture);
  if (watering) {
    pumpOn();
    delay(30000); //how long it "waters" (30 seconds)
    Serial.println("Watering Finished");
    pumpOff();
  }
    Serial.println("------------------------------------------------");
}

String readSoilMoisture(int soilMoisture){
  if (soilMoisture < dry && soilMoisture > (dry - moistureLevel)){
    return "Dry";
  } else if (soilMoisture > wet && soilMoisture < (wet + moistureLevel)){
    return "Wet";
  } else {
    return "Place in Soil";
  }
}

//Turn "pump" on or off
void pumpOn() {
  Serial.println("Water pump, aka motor, is turned on");
  digitalWrite(motorPin_PIN, HIGH);
}
void pumpOff() {
  Serial.println("Water pump, aka motor, is turned off");
  digitalWrite(motorPin_PIN, LOW);
}

void waterCheck(int soilMoisture) {
  if (soilMoisture > startWatering) {
    watering = true;
  } else if (soilMoisture < stopWatering) {
    watering = false;
  }
  Serial.println(watering ? "Start watering" : "Skip watering");
  }
