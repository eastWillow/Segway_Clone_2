void interrupt_initial(void) {
  pinMode(7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(7), interruptReset, FALLING);
}
void interruptReset(void) {
  errMillis = 0;
  errAngle = 0;
  command = 0;
  PIDreset();
  ScurveReset();
  initialMillis = millis();
  initialAngle = use_CompAngle(micros());
  digitalWrite(13, LOW);
}

