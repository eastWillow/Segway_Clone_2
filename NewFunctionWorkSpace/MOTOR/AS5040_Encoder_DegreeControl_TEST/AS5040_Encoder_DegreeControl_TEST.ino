#include <Encoder.h>
#define PWM1 6
#define PWM2 11
#define EN 8
Encoder Enc(0, 1);
void setup() {
  pinMode(PWM1, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(PWM2, OUTPUT);
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  digitalWrite(EN, LOW);
}

void loop() {
  static long oldPosition  = -999;
  static int degree = 0;
  static int power = 0;
  const float P = 5.5;
  const float I = 0.001;//積分需要加上固定的時間去積分
  const float K = 0.01;
  static float err = 0;
  static float totalErr = 0;

  long newPosition = Enc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
  }
  while (Serial.available() > 0) {
    degree = Serial.parseInt();
  }
  totalErr += err;
  err = (degree* 48.30 - newPosition);
  power = round(((err) * P + (totalErr) * I)*K);
  power = constrain(power, -255.0, 255.0);
  if (power > 0) {
    analogWrite(PWM1, abs(power));
    analogWrite(PWM2, 0);
  }
  else {
    analogWrite(PWM1, 0);
    analogWrite(PWM2, abs(power));
  }
  Serial.print(newPosition);
  Serial.print("\t");
  Serial.print(err/48.30);
  Serial.print("\t");
  Serial.println(power);
}
