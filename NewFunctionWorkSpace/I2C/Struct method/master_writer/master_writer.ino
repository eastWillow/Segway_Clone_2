// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

struct _data{
  float angle;
}data;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  while(!Serial);
}

byte b = 0;

void loop()
{
  void *p = &data;
  data.angle = 0.55;
  size_t dataSize = sizeof(data);
  Wire.beginTransmission('A'); // transmit to device #4
  for(int i = 0;i < dataSize; i++){
    Wire.write(*((uint8_t*)p + i));
  }
  Wire.endTransmission();    // stop transmitting
  Wire.beginTransmission('B'); // transmit to device #4
  Wire.write("b is ");        // sends five bytes
  Wire.write(b);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  b--;
  delay(300);
}
