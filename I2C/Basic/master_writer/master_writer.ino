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
}

byte a = 0;
byte b = 0;

void loop()
{
  Wire.beginTransmission('A'); // transmit to device #4
  Wire.write("a is ");        // sends five bytes
  Wire.write(a);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  a++;
  Wire.beginTransmission('B'); // transmit to device #4
  Wire.write("b is ");        // sends five bytes
  Wire.write(b);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  b--;
}
