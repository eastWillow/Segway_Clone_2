struct _data {
  int command;
  bool reset;
  int heart;
} data;

void motorCommand(int command,bool reset,int heart)
{
  void *p = &data;
  size_t dataSize = sizeof(data);
  data.command = command;
  data.reset = reset;
  data.heart = heart;
  Wire.beginTransmission('L'); // transmit to device 
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
