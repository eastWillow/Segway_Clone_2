struct _data {
  int command;
} data;

void motorCommand(int command)
{
  void *p = &data;
  size_t dataSize = sizeof(data);
  data.command = command;
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
