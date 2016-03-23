#define ENCODER_USE_INTERRUPTS
#define _RIGHT
#define _PID
/*
本次只有傳送pluse 給 Matlab 剩下運算在電腦端
*/
#include <Encoder.h>
#include <TimerOne.h>
#include <TimerThree.h>

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
volatile static bool flag;

volatile long pluse;
volatile long last_pluse;
struct _data {
  volatile int target = 2048;
} data;

static long err = 0;
static long totalErr = 0;
  
void motorInitial() {
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

  Serial.begin(115200);
}

void Motor() {
  static long initialMillis = 0;
  static long errMillis = 0;
  static int target = 0;
  static int power = 0;
  while (data.target == 2048) {
    initialMillis = millis();
     err = 0;
    totalErr = 0;
  }
  errMillis = millis() - initialMillis;
  target = data.target;
  if (target == 2048)
    target = 0;
  #ifdef _PID
  power = PID(pluse,target);
  #endif
  #ifdef NO_PID
  power = target;
  #endif
  if (power > 0) {
    #ifdef _LEFT
    Timer1.pwm(PWM1, abs(power));
    Timer1.pwm(PWM2, 0);
    #endif
    #ifdef _RIGHT
    Timer1.pwm(PWM1, 0);
    Timer1.pwm(PWM2, abs(power));
    #endif
  }
  else {
    #ifdef _LEFT
    Timer1.pwm(PWM1, 0);
    Timer1.pwm(PWM2, abs(power));
    #endif
    #ifdef _RIGHT
    Timer1.pwm(PWM1, abs(power));
    Timer1.pwm(PWM2, 0);
    #endif
  }
  Serial.print((double)errMillis / 1000.0,4);
  Serial.print('\t');
  Serial.print(pluse);
  Serial.print('\t');
  Serial.print(target);
  Serial.print('\t');
  Serial.println(power);
}
void getRpm() {
  static unsigned char times = 0;
  Timer3.detachInterrupt();
  #ifdef _LEFT
  pluse = (newPosition) - (oldPosition);//Right Side Need to Add a Negative
  #endif
  #ifdef _RIGHT
  pluse = -((newPosition) - (oldPosition));
  #endif
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
void getI2C_Data(int x) {
  void *p = &data;
  size_t dataSize = sizeof(data);
  while (Wire.available()) // loop through all but the last
  {
    for (int i = 0; i < dataSize; i++) {
      *((uint8_t*)p + i) = Wire.read();
    }
  }
}
