//輸出資料到電腦
void Print_Data(){
  
//  Serial.print(accX);Serial.print("\t");
//  Serial.print(accY);Serial.print("\t");
//  Serial.print(accZ);Serial.print("\t");
//   
//  Serial.print(gyroX);Serial.print("\t");
//  Serial.print(gyroY); Serial.print("\t");
//  Serial.print(gyroZ);Serial.print("\t");
   
//  Serial.print(accXangle);  Serial.print("\t");
//  Serial.print(gyroXangle);  Serial.print("\t");
//  Serial.print(compAngleX);  Serial.print("\t");
//
//  Serial.print("\t");
//
//  Serial.print(accYangle);  Serial.print("\t");
//  Serial.print(gyroYangle);  Serial.print("\t");
//  Serial.print(compAngleY);   Serial.print("\t");
//
//  Serial.print(temp);Serial.print("\t");
  
//  Serial.print(millis());Serial.print("\t");
  
  Serial.print(angle);Serial.print("\t");
  Serial.print(K);Serial.print("\t");
  //Serial.print(output);Serial.print("\t");
  //Serial.print(motor_out);Serial.print("\t");
  //Serial.print(steer_val);Serial.print("\t");
  //Serial.print(gain_val);Serial.print("\t");
  //Serial.print(m1_speed);Serial.print("\t");
  //Serial.print(m2_speed);Serial.print("\t");
  Serial.print(OCR0A);Serial.print('\t');
  Serial.print(OCR0B);Serial.print('\t');
  Serial.print(OCR1A);Serial.print('\t');
  Serial.print(OCR1B);Serial.print('\t');
  Serial.println();
}

