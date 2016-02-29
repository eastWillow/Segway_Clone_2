// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

struct _data {
  float angle;
} data;


void setup()
{
  Serial.begin(9600);           // start serial for output
  while (!Serial);
  Wire.begin('A');                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  void *p = &data;
  size_t dataSize = sizeof(data);
  while (Wire.available()) // loop through all but the last
  {
    for (int i = 0; i < dataSize; i++) {
      *((uint8_t*)p + i) = Wire.read();
    }
    Serial.println(data.angle,2);
  }
}
