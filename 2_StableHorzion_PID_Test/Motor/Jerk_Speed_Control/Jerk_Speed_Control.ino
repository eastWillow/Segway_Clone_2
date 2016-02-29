#include <TimerOne.h>
#include <TimerThree.h>
#include <math.h>

#define maxA (double) 20
#define maxV (double) 200
#define jerkValue 50 //

volatile static double jerk = jerkValue;
static double j = 0.0;
static double lastJ = 0.0;
static double a = 0.0 ;
static double lastA = 0.0 ;
static double t = 0.0 ;
static double v = 0.0 ;
static double targetV = 0.0 ;
static double lastTargetV = 0.0 ;
static double err = 0.0 ;
static double tErr = 0.0;
static double t0 = 0.0;
static double t1 = 0.0;//jerk * t1 = maxAccel
static double t2 = 0.0;//t2 = k* t1
static double t3 = 0.0;//t3 = t1
static double k = 1.0;//k*t1 = t2
static double derivative = 0.0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  while (!Serial);
}

void loop() {
  t = micros()/1000000.0;
  while (Serial.available() > 0) {
    lastTargetV = targetV;
    targetV = Serial.parseInt();
    t0 = t;
  }
  err = targetV - lastTargetV;
  tErr = sqrt(((2.0 * fabs(err)) / (jerk)) * (1.0 / (2.0 * k + 2.0)));
  t1 = t0 + tErr;
  t2 = k * tErr + t1;
  t3 = tErr + t2;
  if (t < t1) {
    lastJ = j;
    j = jerk * direction(err);
  }
  else if (t < t2) {
    lastJ = j;
    j = 0.0;
  }
  else if (t < t3) {
    lastJ = j;
    j = -jerk * direction(err);
  }
  else {
    lastJ = j;
    j = 0.0;
  }
  lastA = a;
  lastJ = j;
  derivative = ((micros()/1000000.0) - t);
  a += ((lastJ + j) / 2.0 ) * derivative ; //jerk integral to accel
  v += ((lastA + a ) / 2.0 ) * derivative; //accel integral to speed
  Serial.print(t);
  Serial.print('\t');
  Serial.print(j);
  Serial.print('\t');
  Serial.print(a);
  Serial.print('\t');
  Serial.print(v);
  Serial.print("\t,");
  Serial.print(derivative,10);
  Serial.print('\t');
  Serial.print(t0);
  Serial.print('\t');
  Serial.print(tErr);
  Serial.print('\t');
  Serial.print(t1);
  Serial.print('\t');
  Serial.print(t2);
  Serial.print('\t');
  Serial.println(t3);
  /*Timer1.initialize();
  Timer3.initialize();
  Timer1.attachInterrupt();
  Timer3.attachInterrupt();*/
}
double direction(double x) {
  return (x >= 0) ? (1.0) : (-1.0);
}
