#include <TimerOne.h>
#include <TimerThree.h>
#include <math.h>
#define derivative 0.001 //

const double jerk = 9080000.0 * derivative ; //we must to change this value to change speed
volatile double j = 0.0;
volatile double a = 0.0 ;
volatile double t = 0.0 ;
volatile double v = 0.0 ;
volatile double targetV = 0.0 ;
volatile double lastTargetV = 0.0 ;
volatile double errV = 0.0 ;
volatile double tErr = 0.0;
volatile double t0 = 0.0;
volatile double t1 = 0.0;//jerk * t1 = maxAccel
volatile double t2 = 0.0;//t2 = k* t1
volatile double t3 = 0.0;//t3 = t1
volatile double k = 1.0;//k*t1 = t2


void ScurveInitial() {
  Timer1.initialize(derivative * 1019000); //0.001s to Integrator add offset
  Timer3.initialize(derivative * 1000000); //0.001s to change Jerk
}

void ScurveCommand(int command, double time) {
  t  = time;

  if (t > t2 && command != targetV) {
    Timer1.detachInterrupt();
    Timer3.detachInterrupt();
    targetV = command;
    a = 0;
    j = 0;
    errV = targetV - v;
    tErr = sqrt(((2.0 * fabs(errV)) / jerk) * (1.0 / (2.0 * k + 2.0)));
    t0 = t;
    t1 = t0 + tErr;
    t2 = k * tErr + t1;
    t3 = tErr + t2;
    Timer3.attachInterrupt(JerkCommand);
    Timer1.attachInterrupt(Integrator);
  }

}

int ScurveSpeedOutput(void) {
  /*Serial.print(t, 4);
  Serial.print('\t');
  Serial.print(j, 4);
  Serial.print('\t');
  Serial.print(a, 4);
  Serial.print('\t');*/
  Serial.print(v, 4);
  Serial.print('\t');
  return (int)round(v);
}
/*
  Serial.print(t, 4);
  Serial.print('\t');
  Serial.print(j, 4);
  Serial.print('\t');
  Serial.print(a, 4);
  Serial.print('\t');
  Serial.println(v, 4);
*/
void Integrator(void) {
  a += (j * derivative);
  v += (a * derivative);
}

void JerkCommand(void) {
  if (t < t1) {
    j = jerk * direction(errV);
  }
  else if (t < t2) {
    j = 0.0;
  }
  else if (t < t3) {
    j = -jerk * direction(errV);
  }
  else {
    j = 0.0;
    if ( fabs(v - targetV) > 0.005 && t >= t3) {
      a = 0.0;
      //v = targetV;
    }
  }
}
double direction(double x) {
  return (x >= 0) ? (1.0) : (-1.0);
}
void ScurveReset(void) {
  j = 0.0;
  a = 0.0 ;
  t = 0.0 ;
  v = 0.0 ;
  targetV = 0.0 ;
  lastTargetV = 0.0 ;
  errV = 0.0 ;
  tErr = 0.0;
  t0 = 0.0;
  t1 = 0.0;//jerk * t1 = maxAccel
  t2 = 0.0;//t2 = k* t1
  t3 = 0.0;//t3 = t1
}
