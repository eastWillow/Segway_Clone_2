#include "I2C_communication.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>
#include "MotorControl.h"
#include "rawData.h"

long angle = 0.00;
long offset = 0.00;

void setup() {
  Serial.begin(115200);
  i2c_initial();
  main_set(); //rawData.h
}

void loop() {
  //eastWillow degree update delay 2014/04/16
  sample_angle();
  use_CompAngle();
  timer = micros();
  getInitialAngle(compAngleZ);
  motorUpdateSpeed(compAngleZ);
}
