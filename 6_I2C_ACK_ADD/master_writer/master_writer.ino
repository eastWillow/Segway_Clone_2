#include <Wire.h>
#define motorOff true
#define motorOn false
#define motorHeart random(128)
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.setTimeout(50);
  pinMode(13, OUTPUT);
  interrupt_initial();
  main_set();
  ScurveInitial();
  motorCommand(0,motorOff,motorHeart);
  digitalWrite(13, HIGH);
}

volatile double initialAngle = 0;
volatile double initialMillis = 0;
volatile double errMillis = 0;
volatile double errAngle = 0;
volatile bool interruptResetFlag = false;

void loop()
{
  static double PID_target = 0;
  sample_angle(micros());
  if (interruptResetFlag == true) {
    interruptResetFlag = false;
    motorCommand(0,motorOff,motorHeart);
    PIDreset();
    ScurveReset();
    errMillis = 0;
    errAngle = 0;
    initialMillis = millis();
    initialAngle = use_CompAngle(micros());
    digitalWrite(13, LOW);
  }
  else {
    errMillis = (millis() - initialMillis) / 1000.0;
    errAngle = initialAngle - use_CompAngle(micros());
    PID_target = PID(errAngle);
    ScurveCommand(PID_target, errMillis);
    motorCommand(ScurveSpeedOutput(),false,motorHeart);
    Serial.print((double)errMillis, 4);
    Serial.print('\t');
    Serial.print(PID_target);
    Serial.print('\t');
    Serial.println(errAngle);
  }
  
}
