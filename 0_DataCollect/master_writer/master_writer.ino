// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

struct _data {
  int power;
} data;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  data.power = 2048;
  Serial.begin(115200);
  Serial.setTimeout(50);
  while (!Serial);
}

void loop()
{
  void *p = &data;
  size_t dataSize = sizeof(data);
  while(Serial.available() > 0){
    data.power = Serial.parseInt();
  }
  Wire.beginTransmission('L'); // transmit to device #4
  for (int i = 0; i < dataSize; i++) {
    Wire.write(*((uint8_t*)p + i));
  }
  Wire.endTransmission();    // stop transmitting
  Wire.beginTransmission('R'); // transmit to device #4
  for (int i = 0; i < dataSize; i++) {
    Wire.write(*((uint8_t*)p + i));
  }
  Wire.endTransmission();    // stop transmitting
}
