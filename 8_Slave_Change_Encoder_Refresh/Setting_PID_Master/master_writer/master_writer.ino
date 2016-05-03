#include <Wire.h>
#define motorOff true
#define motorOn false
#define motorHeart random(128)
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.setTimeout(20);
  pinMode(13, OUTPUT);
  interrupt_initial();
  main_set();
  ScurveInitial();
  motorCommand(0, motorOff, motorHeart);
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
  static int command = 0;
  while (Serial.available() > 0) {
    command = Serial.parseInt();
    if (command > 100) {
        command = 100;
    }
    if (command < -100) {
      command = -100;
    }
  }
  motorCommand(command, motorOn, motorHeart);
  if(interruptResetFlag == true){
    motorCommand(0, motorOff, motorHeart);
    interruptResetFlag = false;
    command = 0;
  }
  Serial.println(command);
}
