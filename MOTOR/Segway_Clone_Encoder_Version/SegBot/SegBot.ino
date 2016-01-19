#include <Wire.h>

/* IMU Data */
int16_t accX, accY, accZ;
int16_t tempRaw;
int16_t gyroX, gyroY, gyroZ;

double accXangle, accYangle; // Angle calculate using the accelerometer
double gyroXrate, gyroYrate;
double temp; // Temperature
double gyroXangle, gyroYangle; // Angle calculate using the gyro
double compAngleX, compAngleY; // Calculate the angle using a complementary filter

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data


int steeringPot = 1; //　轉向可變電阻角位 A
int gainPot = 0; //　角度範圍可變電阻角位 A
int steer_range = 100;
int steer_val ,gain_val ,steer_reading ,gain_reading ,output,last_output;

int engage_switch = 8; //　馬達啟動按鈕 D
int engage_state = 1;
boolean engage = false;
double err,L_e,P,I,D,U,K;
float angle = 0.00;

int m_1_EN = 2; //　馬達控制板通電角位 D
int m1_PWM1 = 6; //　PWM_OC0A
int m1_PWM2 = 5; //　PWM_OC0B

int m_2_EN = 4; //　馬達控制板通電角位 D
int m2_PWM1 = 9; // PWM_OC1A
int m2_PWM2 = 10; //　PWM_OC1B

int motor_out = 0;
int motor_1_out = 0;
int motor_2_out = 0;
int m1_speed = 0;
int m2_speed = 0;

int offset = 4;

float kp =151.00;
float ki =0.0;//5.00;
float kd =0.0;//51.00;

void setup() {  
  Serial.begin(115200);
  Wire.begin();
  pinMode(engage_switch, INPUT);
  pinMode(m_1_EN,OUTPUT);
  pinMode(m1_PWM1,OUTPUT);
  pinMode(m1_PWM2,OUTPUT);
  TCCR0A = _BV(COM0A1) | _BV(WGM00) | _BV(COM0B1);
  TCCR0B = _BV(CS01);
  pinMode(m_2_EN,OUTPUT);
  pinMode(m2_PWM1,OUTPUT);
  pinMode(m2_PWM2,OUTPUT);
  TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(COM1B1);
  TCCR1B = _BV(CS11);

  pinMode(13, OUTPUT);
  digitalWrite(engage_switch, HIGH);
  main_set();
}

void loop() {
  //從感應器取得角度
  sample_angle();
  
  //過去雜訊
  use_CompAngle();
  
  //角度轉換與補正
  angle = (compAngleY-180-(offset))/100;
  
  //PID控制器
  err = -angle;  
  P = err;
  I = 0.92*(I+err);
  D = err-L_e;
  U = P*kp + I*ki + D*kd;
  L_e = err;  
  K = abs(U)*angle;    
  
  read_pots();
  update_motor_speed();
  
  timer = micros();
  delay(67);
  Print_Data();
}
