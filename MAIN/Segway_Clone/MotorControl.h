#define LED_PIN 13
#define MOTOR_LEFT 0x0A //I2C Address
#define MOTOR_RIGHT 0x0B //I2C Address

#define steeringPot A1 //　Analog pin 1; potentiometer for steering 轉向可變電阻角位 A

static long initialAngle = 0.0;
static int initialSteering = 0;
const long SterringGain = 0.0;

const long kp = 1.0;
const long ki = 0.0;
const long kd = 0.0;
const long Ku = 1.0;



bool getInitialAngleFlag = 0;

void getInitialAngle(long compAngleZ) {
  if (getInitialAngleFlag == 0) {
    initialAngle = compAngleZ;
    initialSteering = analogRead(steeringPot);
    if (millis() > 1000 ) {
      getInitialAngleFlag = 1;
      digitalWrite(LED_PIN, HIGH);
    }
  }
}

void motorUpdateSpeed(long compAngleZ) {
  int steeringValue;
  static long errSteeringValue ;
  static long  err;
  static long  totalErr;
  static long U;

  steeringValue = analogRead(steeringPot);

  totalErr += err;
  err = compAngleZ - initialAngle;
  U = err * kp + totalErr * ki;
  errSteeringValue = (long)(steeringValue - initialSteering) * SterringGain;
  
  i2c_transition(U+errSteeringValue,MOTOR_LEFT);
  i2c_transition(U-errSteeringValue,MOTOR_RIGHT);
  
  Serial.print(F("angleZ:"));
  Serial.print(compAngleZ);
  Serial.print(F("\t"));
  Serial.print(F("err:"));
  Serial.print(err);
  Serial.print(F("\t"));
  Serial.print(F("U:"));
  Serial.print(U);
  Serial.println();
}
