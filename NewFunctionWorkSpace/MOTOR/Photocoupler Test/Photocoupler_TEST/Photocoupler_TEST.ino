#define PWM1 6
#define PWM2 11
#define EN 8
void setup() {
  pinMode(PWM1, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(PWM2, OUTPUT);
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  digitalWrite(EN, LOW);
}

void loop() {
  while(Serial.available() > 0){
    analogWrite(PWM1, Serial.parseInt());
  }
  analogWrite(PWM2, 0);
}
