#include <Wire.h>
#define _S_Curve //_MANUAL_PWM, _AUTO_DEGREE, _MANUAL_SPEED _S_Curve
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.setTimeout(50);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  interrupt_initial();
  main_set();
  ScurveInitial();
  motorCommand(2048);//Shutdown
  motorCommand(0);//Initial
  motorCommand(2048);//Shutdown
}

volatile double initialAngle = 0;
volatile double initialMillis = 0;
volatile int command = 2048;
volatile double errMillis = 0;
volatile double errAngle = 0;
volatile bool interruptResetFlag = false;
void loop()
{
#ifdef _AUTO_DEGREE
#else _S_Curve
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
#ifdef _S_Curve
    if (command > 200) {
      if (command != 2048)
        command = 200;
    }
    if (command < -200) {
      command = -200;
    }
#endif
  }
  sample_angle(micros());
#ifdef _MANUAL_SPEED
  motorCommand(command);
#endif
  if (command == 2048 || interruptResetFlag == true) {
    motorCommand(2048);
    interruptResetFlag = false;
    initialMillis = millis();
    initialAngle = use_CompAngle(micros());
    PIDreset();
    ScurveReset();
    digitalWrite(13, LOW);
  }
  else {
    errMillis = (millis() - initialMillis) / 1000.0;
    errAngle = initialAngle - use_CompAngle(micros());
    Serial.print((double)errMillis, 4);
    Serial.print('\t');
    Serial.print(command);
    Serial.print('\t');
#ifdef _AUTO_DEGREE
    PID_target = PID(errAngle, command);
    motorCommand(PID_target);
    Serial.print(PID_target);
    Serial.print('\t');
#endif
#ifdef _S_Curve
    PID_target = PID(errAngle, command);
    ScurveCommand(PID_target, errMillis);
    motorCommand(ScurveSpeedOutput());
    Serial.print(PID_target);
    Serial.print('\t');
#endif
    Serial.println(errAngle);
  }
  digitalWrite(13, HIGH);
}
