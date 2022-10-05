
#include <Arduino.h>

//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>
#include <Wire.h>
#include <VL53L0X.h>
#define HIGH_SPEED
#define REMOTE_CODE 0xF302FD
VL53L0X DistanceSensor;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  IrSender.begin(); // Start with IR_SEND_PIN as send pin and if NO_LED_FEEDBACK_CODE is NOT defined, enable feedback LED at default feedback LED pin
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

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  DistanceSensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  DistanceSensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  DistanceSensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

 

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  DistanceSensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  DistanceSensor.setMeasurementTimingBudget(200000);
#endif
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
