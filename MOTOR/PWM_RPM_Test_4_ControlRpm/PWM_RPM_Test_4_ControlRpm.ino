#include <Encoder.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include <medianFilter.h>
#include <FilterBeBp1.h>

medianFilter Filter;
FilterBeBp1 LowPassFilter;

#define PWM1 9
#define PWM2 11
#define EN 8
#define CYCLE_PLUSE 17408.0
#define CYCLE_DEGREE 360.0
#define PluseToCycle(pluse) (pluse/CYCLE_PLUSE)
#define PluseToDegree(pluse) (pluse/CYCLE_PLUSE)*CYCLE_DEGREE
#define MinPWM 200 //MAX 1023

Encoder Enc(0, 1);

volatile static long oldPosition  = -999;
volatile static long newPosition  = 0;
volatile float rpm = 0;

void setup() {
  pinMode(PWM1, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(PWM2, OUTPUT);
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  digitalWrite(EN, LOW);
  Timer1.initialize(500);  // 0.5 ms 2khz
  Timer3.initialize(1000); // 0.2ms
  Timer3.attachInterrupt(getRpm);
  Timer1.attachInterrupt(getEncoder);

  Filter.begin();
  LowPassFilter.begin();
  
  Serial.begin(9600);
  Serial.setTimeout(50);
}

void loop() {

  static long degree = 0;
  static long targetRpm = 0;
  static long power = 0;
  static long lastPower = 0;
  const long P = 12.0;
  const long I = 0;//積分需要乘上固定的時間去積分
  const long K = 10.0;
  static long err = 0;
  static long totalErr = 0;


  while (Serial.available() > 0) {
    targetRpm = Serial.parseInt();
  }
  targetRpm = constrain(targetRpm,-55.0,55.0);
  totalErr += err;
  //err = (degree* 48.30 - newPosition);
  err = (targetRpm - rpm);
  lastPower = power;
  power = round(((err) * P + (totalErr) * I)*K);
  power = (int)constrain(power, -1023.0, 1023.0);
  if (power > 0) {
    Timer1.pwm(PWM1, abs(power));
    Timer1.pwm(PWM2, 0);
  }
  else {
    Timer1.pwm(PWM1, 0);
    Timer1.pwm(PWM2, abs(power));
  }
  Serial.print(rpm);
  Serial.print("\t");
  //Serial.print(err / 48.30);//degree
  //Serial.print("\t");
  Serial.print(err);//Rpm
  Serial.print('\t');
  Serial.print(lastPower);
  Serial.print("\t");
  Serial.println(power);
}
void getRpm() {
  static unsigned char times = 0;
  Timer3.detachInterrupt();
  rpm = PluseToCycle(LowPassFilter.run(Filter.run((newPosition) - (oldPosition)))) * 1000 * 60;
  oldPosition = newPosition;
  Timer3.attachInterrupt(getRpm);
}
void getEncoder() {
  Timer1.detachInterrupt();
  newPosition = Enc.read();
  Timer1.attachInterrupt(getEncoder);
}
