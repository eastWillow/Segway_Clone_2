#include <Wire.h>

struct _data{
  double angle;
}data;

void i2c_initial(){
  Wire.begin();
}
void i2c_transition(double angle, uint8_t address){
  void *dataP = &data;
  data.angle = angle;
  size_t dataSize = sizeof(data);
  Wire.beginTransmission(address);
  for(size_t i = 0;i < dataSize; i++){
    Wire.write(*((uint8_t*)dataP + i));
  }
  Wire.endTransmission();
}
