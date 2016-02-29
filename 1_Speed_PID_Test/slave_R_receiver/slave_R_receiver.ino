#include <Wire.h>

void setup()
{
  Wire.begin('R');                // join i2c bus with address #4
  Wire.onReceive(getI2C_Data); // register event
  motorInitial();
}

void loop()
{
  Motor();
}
