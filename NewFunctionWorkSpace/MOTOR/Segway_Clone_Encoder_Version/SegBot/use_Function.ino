//初始設定
void main_set(){
  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while(i2cWrite(0x19,i2cData,4,false)); // Write to all four registers at once
  while(i2cWrite(0x6B,0x01,true)); // PLL with X axis gyroscope reference and disable sleep mode 

  while(i2cRead(0x75,i2cData,1));
  if(i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while(1);
  }
  delay(100); // Wait for sensor to stabilize
  timer = micros();
  
}

//濾波器
void use_CompAngle(){
  compAngleX = (0.97*(compAngleX+(gyroXrate*(double)(micros()-timer)/1000000)))+(0.03*accXangle); // Calculate the angle using a Complimentary filter
  compAngleY = (0.97*(compAngleY+(gyroYrate*(double)(micros()-timer)/1000000)))+(0.03*accYangle);
}

//轉向與增益數值讀取
void read_pots(){  
  steer_reading = analogRead(steeringPot);
  steer_val = 0;//map(steer_reading, 0, 1023, steer_range, -steer_range);
  gain_reading = analogRead(gainPot);
  gain_val = 50;//map(gain_reading, 0, 1023, 50, 500);
}


//輸出馬達控制器
void update_motor_speed(){
   digitalWrite(13, HIGH);
   output = (K * 1000);
   motor_out = map(output,-30000,30000,-253,253); // map the angle     
    // assign steering bias
    motor_1_out = motor_out - (steer_val); //
    motor_2_out = motor_out + (steer_val); //
    // test for and correct invalid values
    if(motor_1_out > 253)motor_1_out = 253;
    if(motor_1_out < -253)motor_1_out = -253;
    if(motor_2_out > 253)motor_2_out = 253;
    if(motor_2_out < -253)motor_2_out = -253;
    // assign final motor output values
    m1_speed = 0 + motor_1_out;
    m2_speed = 0 + motor_2_out;
    m1_speed = constrain(m1_speed, -253, 253);
    m2_speed = constrain(m2_speed, -253, 253);
    //eastWillow Version Motor Moudle 
    if(m1_speed > 0){
      OCR0B = 0;
      OCR0A = m1_speed;
    }
    else if(m1_speed < 0){
      OCR0A = 0;
      m1_speed = abs(m1_speed);
      OCR0B = m1_speed;
    }
    else{
      OCR0A = 0;
      OCR0B = 0;
    }
    //Motor 2
    if(m2_speed > 0){
      OCR1B = 0;
      OCR1A = m2_speed;
    }
    else if(m2_speed < 0){
      OCR1A = 0;
      m2_speed = abs(m2_speed);
      OCR1B = m2_speed;
    }
    else{
      OCR1A = 0;
      OCR1B = 0;
    }
}




