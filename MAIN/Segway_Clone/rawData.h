/* IMU Data */
int16_t accX, accY;
int16_t gyroZ;

long accZangle; // Angle calculate using the accelerometer
long gyroZrate;
long gyroZangle; // Angle calculate using the gyro
long compAngleZ; // Calculate the angle using a complementary filter



uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data
void use_CompAngle(){
  compAngleZ = (0.96*(compAngleZ+(gyroZrate*(double)(micros()-timer)/1000000.0)))+(0.04*accZangle); // Calculate the angle using a Complimentary filter

}

const uint8_t IMUAddress = 0x68; // AD0 is logic low on the PCB
const uint16_t I2C_TIMEOUT = 1000; // Used to check for errors in I2C communication



uint8_t i2cWrite(uint8_t registerAddress, uint8_t *data, uint8_t length, bool sendStop) {
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  Wire.write(data, length);
  uint8_t rcode = Wire.endTransmission(sendStop); // Returns 0 on success
  return rcode; // See: http://arduino.cc/en/Reference/WireEndTransmission
}

uint8_t i2cWrite(uint8_t registerAddress, uint8_t data, bool sendStop) {
  return i2cWrite(registerAddress,&data,1,sendStop); // Returns 0 on success
}

uint8_t i2cRead(uint8_t registerAddress, uint8_t *data, uint8_t nbytes) {
  uint32_t timeOutTimer;
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  uint8_t rcode = Wire.endTransmission(false); // Don't release the bus
  if (rcode) {
    return rcode; // See: http://arduino.cc/en/Reference/WireEndTransmission
  }
  Wire.requestFrom(IMUAddress, nbytes,(uint8_t)true); // Send a repeated start and then release the bus after reading
  for (uint8_t i = 0; i < nbytes; i++) {
    if (Wire.available())
      data[i] = Wire.read();
    else {
      timeOutTimer = micros();
      while (((micros() - timeOutTimer) < I2C_TIMEOUT) && !Wire.available());
      if (Wire.available())
        data[i] = Wire.read();
      else {
        return 5; // This error value is not already taken by endTransmission
      }
    }
  }
  return 0; // Success
}
void sample_angle(){
  while(i2cRead(0x3B,i2cData,14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);
  accZangle = (atan2(accY,accX)+PI)*RAD_TO_DEG;
  gyroZrate = (double)gyroZ/131.0;
  gyroZangle += gyroZrate*((double)(micros()-timer)/1000000.0); // Calculate gyro angle without any filter  
}
void main_set(){
  i2cData[0] = 9; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x06; // Disable FSYNC and set 5 Hz Acc filtering, 5 Hz Gyro filtering, 1 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to 
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to
  while(i2cWrite(0x19,i2cData,4,false)); // Write to all four registers at once
  while(i2cWrite(0x6B,0x01,true)); // PLL with X axis gyroscope reference and disable sleep mode 

  while(i2cRead(0x75,i2cData,1));
  if(i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    while(1);
  }
  //delay(100); // Wait for sensor to stabilize
  timer = micros();
  
}
