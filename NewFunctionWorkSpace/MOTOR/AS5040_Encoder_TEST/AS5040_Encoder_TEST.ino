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
  static unsigned int power = 0;
  long newPosition = Enc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
  
  while(Serial.available() > 0){
    power = Serial.parseInt();
    if (power > 0){
      analogWrite(PWM1, abs(power));
      analogWrite(PWM2, 0);
    }
    else{
      analogWrite(PWM1, 0);
      analogWrite(PWM2, abs(power));
    }
  }
}
