#include <Wire.h>
#define _AUTO_DEGREE //_MANUAL_PWM, _AUTO_DEGREE, _MANUAL_SPEED
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.setTimeout(50);
  main_set();
  while (!Serial);
  motorCommand(2048);//Shutdown
  motorCommand(0);//Initial
  motorCommand(2048);//Shutdown
}

void loop()
{
  static int command = 2048;
  static long initialMillis = 0;
  static long errMillis = 0;
  static double errAngle = 0;
  static double initialAngle = 0;
#ifdef _AUTO_DEGREE
  static double PID_target = 0;
#endif
  while (Serial.available() > 0) {
    command = Serial.parseInt();
#ifdef _MANUAL_SPEED
    if (command > 200) {
      if (command != 2048)
        command = 200;
    }
    if (command < -200) {
      command = -200;
    }
#endif
#ifdef _MANUAL_PWM
    if (command > 1023) {
      if (command != 2048)
        command = 1023;
    }
    if (command < -1023) {
      command = -1023;
    }
#endif
#ifdef _AUTO_DEGREE
    if (command > 90) {
      if (command != 2048)
        command = 90;
    }
    if (command < -90) {
      command = -90;
    }
#endif
  }
  sample_angle(micros());
#ifndef _AUTO_DEGREE
  motorCommand(command);
#endif
  if (command == 2048) {
    initialMillis = millis();
    initialAngle = use_CompAngle(micros());
    PIDreset();
    motorCommand(2048);
  }
  else {
    errMillis = millis() - initialMillis;
    errAngle = initialAngle - use_CompAngle(micros());
    Serial.print((double)errMillis / 1000.0, 4);
    Serial.print('\t');
    Serial.print(command);
    Serial.print('\t');
#ifdef _AUTO_DEGREE
    PID_target = PID(errAngle, command);
    motorCommand(PID_target);
    Serial.print(PID_target);
    Serial.print('\t');
#endif
    Serial.println(errAngle);
  }
}
