// 接線 GND==>GND  VCC==>5V  SDA==>A4  SCL==>A5
#include <LiquidCrystal_I2C.h>

#include <DHT.h>

#include <ThreadController.h>

#include "Potentionmeter.h"
#include "Sensor.h"
#include "Model.h"
#include "ModelControl.h"

ModelControl controller;
LiquidCrystal_I2C lcd(0x27,20,4);
// Instantiate a new ThreadController
ThreadController tasks = ThreadController();
Potentiometer* potentionmeter;
Sensor* sensor;


//Constants
#define PUMPPIN 8
#define COOLERPIN 4
#define MODEPIN 13

void setup()
{
  Serial.begin(9600);
  lcd.init();
  sensor = new Sensor();
  sensor->setInterval(100);
  tasks.add(sensor);
  potentionmeter = new Potentiometer();
  potentionmeter->setInterval(100);
  tasks.add(potentionmeter);
  pinMode(COOLERPIN, OUTPUT);
  digitalWrite(COOLERPIN, HIGH);
  pinMode(PUMPPIN, OUTPUT);
  digitalWrite(PUMPPIN, HIGH);
  pinMode(MODEPIN, INPUT);
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Greenhouse v1.0.0");
  lcd.setCursor(0,1);
  lcd.print("Power By Deflinhec");
  delay(2000);
  lcd.clear();
}

void loop()
{
  char value[4] = {0};
  char buffer[20] = {0};

  bool dirty = false;
  if (digitalRead(MODEPIN) == HIGH) {
    dirty = controller.select(potentionmeter);
  } else {
    dirty = controller.select(sensor);
  }
  Model* model = controller.model();

  // update humidity display if changed
  float humidity = 0.0f;
  if (model->humidity(&humidity) || dirty) {
    memset(value, 0, sizeof(value));
    dtostrf(humidity, 4, 2, value);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%-12s %5s ", "Humidity:", value);
    lcd.setCursor(0,0); buffer[19]='%'; lcd.print(buffer);
  }
  // update tempature display if changed
  float temperature = 0.0f;
  if (model->temperature(&temperature) || dirty) {
    memset(value, 0, sizeof(value));
    dtostrf(temperature, 4, 2, value);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%-12s %5s ", "Temperature:", value);
    lcd.setCursor(0,1); buffer[19]='C'; lcd.print(buffer); 
  }
  // update moisture display if changed
  int moisture = 0;
  if (model->moisture(&moisture) || dirty) {
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%-12s %4d", "SoilMoisture:", moisture);
    lcd.setCursor(0,2); lcd.print(buffer);
  }
  // update moisture display if changed
  if (model == sensor) {
    int max_moisture = 0;
    potentionmeter->moisture(&max_moisture);
    if (moisture < max_moisture)
      digitalWrite(PUMPPIN, LOW);
    else
      digitalWrite(PUMPPIN, HIGH);
    float max_temperature = 0.0f;
    potentionmeter->temperature(&max_temperature);
    if (temperature > max_temperature)
      digitalWrite(COOLERPIN, LOW);
    else
      digitalWrite(COOLERPIN, HIGH);
  }
  tasks.run(); delay(20);
}
