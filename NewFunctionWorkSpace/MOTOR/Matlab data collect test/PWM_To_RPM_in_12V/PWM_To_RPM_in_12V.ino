#define ENCODER_USE_INTERRUPTS
/*
本次只有傳送pluse 給 Matlab 剩下運算在電腦端
*/
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
volatile long pluse;
volatile long last_pluse;
volatile int power;

void setup() {
  pinMode(PWM1, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(PWM2, OUTPUT);
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  digitalWrite(EN, LOW);
  Timer1.initialize(250);  // 0.25 ms 5khz
  Timer3.initialize(10000); // 10ms = 0.01 s

  Timer1.attachInterrupt(getEncoder);
  Timer3.attachInterrupt(getRpm);

  Filter.begin();
  LowPassFilter.begin();

  Serial.begin(115200);
  Serial.setTimeout(50);
}

void loop() {
  static long initialMillis = 0;
  static long errMillis = 0;
  while (!Serial) {
    initialMillis = millis();
  }
  while(Serial.available() > 0){
    power = (Serial.parseInt())*100;
  }
  errMillis = millis() - initialMillis;
  if (power > 0) {
    Timer1.pwm(PWM1, abs(power));
    Timer1.pwm(PWM2, 0);
  }
  else {
    Timer1.pwm(PWM1, 0);
    Timer1.pwm(PWM2, abs(power));
  }
  Serial.print((double)errMillis / 1000.0);
  Serial.print('\t');
  Serial.print(pluse);
  Serial.print('\t');
  Serial.println(power);
}
void getRpm() {
  static unsigned char times = 0;
  Timer3.detachInterrupt();
  //rpm = PluseToCycle(LowPassFilter.run(Filter.run((newPosition) - (oldPosition)))) * 1000 * 60;
  pluse = (newPosition) - (oldPosition);
  if (abs(pluse - last_pluse) > 100) {
    pluse = last_pluse;
  }
  oldPosition = newPosition;
  last_pluse = pluse;
  Timer3.attachInterrupt(getRpm);
}
void getEncoder() {
  Timer1.detachInterrupt();
  newPosition = Enc.read();
  Timer1.attachInterrupt(getEncoder);
}
