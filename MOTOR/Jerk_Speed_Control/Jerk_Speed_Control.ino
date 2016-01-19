#include <TimerOne.h>
#include <TimerThree.h>
#include <math.h>

#define PWM1 9
#define PWM2 11
#define EN 8
#define MinPWM 200 //MAX 1023
#define maxA (double) 20
#define maxV (double) 200
#define period 250 //us
#define derivative period*0.000001 // s
#define jerkValue 10 //jerk in 1s
#define maxT1 (maxA/jerkValue)
#define maxT2 (maxV-maxA*maxT1)/maxA
#define maxT3 (maxT1*2+maxT2)

volatile static double jerk = jerkValue;
volatile static double a = 0;
volatile static double lastA = 0;
volatile static double v = 0;
volatile static double targetV = 0;

volatile static double t = 0; // t1 = t3

void setup() {
  pinMode(PWM1, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(PWM2, OUTPUT);
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  digitalWrite(EN, LOW);
  Timer1.initialize(period);  // 0.5 ms 500us 2khz
  Timer3.initialize(period / 2); // 1ms 1khz

  Serial.begin(115200);
  Serial.setTimeout(50);
}

void loop() {
  while (!Serial);
  Timer1.attachInterrupt(speed_planer);
  Timer3.attachInterrupt(jerk_control);
  Serial.print(micros());
  Serial.print('\t');
  Serial.print(jerk);
  Serial.print('\t');
  Serial.print(a);
  Serial.print('\t');
  Serial.println(v);
}
void speed_planer() {
  t += derivative;
  lastA = a;
  a += jerk * derivative;
  v += ((lastA + a) / 2.0) * derivative;
}
void jerk_control() {
  Timer1.detachInterrupt();
  Timer3.detachInterrupt();
  if (t >= maxT1) {
    jerk = 0;
    if (t >= maxT2) {
      jerk = -jerkValue;
      /*if (t >= maxT3 ) {
        jerk = 0;
      }*/
    }
  }


  Timer1.attachInterrupt(speed_planer);
  Timer3.attachInterrupt(jerk_control);
}
