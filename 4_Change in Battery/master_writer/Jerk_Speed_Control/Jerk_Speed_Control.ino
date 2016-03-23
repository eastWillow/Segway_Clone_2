#include <TimerOne.h>
#include <TimerThree.h>
#include <math.h>
#define derivative 0.001 //

const double jerk = 3000.0 * derivative ; //we must to change this value to change speed
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

static long initialTimer = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  while (!Serial);
  Timer1.initialize(derivative * 1000000); //0.001s to Integrator
  Timer3.initialize(derivative * 5000000); //0.005s to change Jerk
  initialTimer = millis();
}

void loop() {
  t  = (millis() - initialTimer) / 1000.0;
  while (Serial.available() > 0) {
    targetV = Serial.parseInt();
    Timer1.detachInterrupt();
    Timer3.detachInterrupt();
    a = 0;
    j = 0;
    errV = targetV - v;
    tErr = sqrt(((2.0 * fabs(errV)) / jerk) * (1.0 / (2.0 * k + 2.0)));
    t0 = t;
    t1 = t0 + tErr;
    t2 = k * tErr + t1;
    t3 = tErr + t2;
  }
  Timer3.attachInterrupt(JerkCommand);
  Timer1.attachInterrupt(Integrator);
  /*Serial.print(t0, 4);
  Serial.print('\t');
  Serial.print(t1, 4);
  Serial.print('\t');
  Serial.print(t2, 4);
  Serial.print('\t');
  Serial.print(t3, 4);
  Serial.print('\t');*/
  Serial.print(t, 4);
  Serial.print('\t');
  Serial.print(j, 4);
  Serial.print('\t');
  Serial.print(a, 4);
  Serial.print('\t');
  Serial.println(v, 4);
}
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
    if ( fabs(v - targetV) > 0.005 && t > t3) {
      a = 0.0;
      v = targetV;
    }
  }
}
double direction(double x) {
  return (x >= 0) ? (1.0) : (-1.0);
}
