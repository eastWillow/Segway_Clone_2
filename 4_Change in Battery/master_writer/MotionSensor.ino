#include "I2Cdev.h"
#include "MPU6050.h"
/*
FS_SEL 0 -> 131.0
FS_SEL 1 -> 65.5
FS_SEL 2 -> 32.8
FS_SEL 3 -> 16.4
*/
#define FS_SEL 1

#if FS_SEL == 0
  #define Gryo_LSB 131.0
#elif FS_SEL == 1
  #define Gryo_LSB 65.5
#endif

MPU6050 accelgyro;

int16_t accX, accY;
int16_t gyroZ;

double accZangle; // Angle calculate using the accelerometer
double gyroZrate;
double gyroZangle; // Angle calculate using the gyro
double compAngleZ; // Calculate the angle using a complementary filter

double use_CompAngle(uint32_t timer) {
  compAngleZ = (0.98 * (compAngleZ + (gyroZrate * (double)(micros() - timer) / 1000000.0))) + (0.02 * accZangle);
  return compAngleZ;
}
void sample_angle(uint32_t timer) {
  accX = accelgyro.getAccelerationX();
  accY = accelgyro.getAccelerationY();
  gyroZ = accelgyro.getRotationZ();
  accZangle = (atan2(accY, accX) + PI) * RAD_TO_DEG;
  gyroZrate = (double)gyroZ / (Gryo_LSB); //According to FS_SEL
  gyroZangle += gyroZrate * ((double)(micros() - timer) / 1000000.0); // Calculate gyro angle without any filter
  
  #ifdef GryoAccel_SERIAL_OUTPUT
  Serial.print(F("accX:"));Serial.print(accX);//Serial.print('\t');
  Serial.print(F(" accY:"));Serial.print(accY);//Serial.print('\t');
  Serial.print(F(" gyroZ:"));Serial.print(gyroZ);//Serial.print('\t');
  Serial.print(F(" accZangle:"));Serial.print(accZangle);//Serial.print('\t');
  Serial.print(F(" gyroZrate:"));Serial.print(gyroZrate);//Serial.print('\t');
  Serial.print(F(" gyroZangle:"));Serial.print(gyroZangle);//Serial.print('\t');
  Serial.println();
  #endif
}
void main_set() {
  accelgyro.initialize();
  accelgyro.setXAccelOffset(-3843);
  accelgyro.setYAccelOffset(1491);
  accelgyro.setZAccelOffset(1011);
  accelgyro.setXGyroOffset(139);
  accelgyro.setYGyroOffset(-58);
  accelgyro.setZGyroOffset(27);
  accelgyro.setClockSource(MPU6050_CLOCK_PLL_YGYRO);
  accelgyro.setDLPFMode(5);//5 is best
  accelgyro.setFullScaleGyroRange(FS_SEL);
  accelgyro.setFullScaleAccelRange(FS_SEL);
}
