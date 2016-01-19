#define EN1 7
#define PWM1 3
#define PWM2 11
#define POT 0
int potOffset;
int potOrign;
void setup(){
  pinMode(EN1,OUTPUT);
  pinMode(PWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  potOrign = analogRead(POT);
  Serial.begin(115200);
}
void loop(){
  potOffset = analogRead(POT) - potOrign;
  Serial.println(potOffset);
  if(potOffset > 5){
    analogWrite(PWM1,0);
    analogWrite(PWM2,map(potOffset,0,470,0,250));
  }
  else if(potOffset < -5){
    analogWrite(PWM2,0);
    potOffset = abs(potOffset);
    analogWrite(PWM1,map(potOffset,0,470,0,250));
  }
  else{
    analogWrite(PWM1,0);
    analogWrite(PWM2,0);
  }
  digitalWrite(EN1,LOW);
}
