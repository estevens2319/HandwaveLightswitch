// Author: Eric Stevens



#include <Arduino.h>
#include "PinDefinitionsAndMore.h" 
#include <IRremote.hpp>
#include <Wire.h>
#include <VL53L0X.h>
#define REMOTE_CODE 0xF302FD
VL53L0X DistanceSensor;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  IrSender.begin(); 
  delay(500);
  
  Wire.begin();
  delay(150);
  Serial.println("00");
  DistanceSensor.init(true);
  Serial.println("01");
  delay(100);
  DistanceSensor.setAddress((uint8_t)22);
  Serial.println("02");
  DistanceSensor.setTimeout(500);
  if (!DistanceSensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  Serial.println("Successfully Configured.");
  DistanceSensor.setMeasurementTimingBudget(20000);

}

boolean on = 1;
void toggleOn(){
  if(on){
    Serial.println("On");
  }
  else{
    Serial.println("Off");
  }
   IrSender.sendNEC(REMOTE_CODE, 32);
   IrSender.sendNEC(REMOTE_CODE, 32);
   IrSender.sendNEC(REMOTE_CODE, 32);
   on = 1- on;
}

void loop() {
  double Distance = DistanceSensor.readRangeSingleMillimeters();  // Collect DistanceSensor data
  if(Distance < 150){
    Distance = DistanceSensor.readRangeSingleMillimeters();  // Must have at least 2 readings <150 to eliminate noise
    if(Distance < 150){
      toggleOn();
      delay(1500);
    }
  }
 }
