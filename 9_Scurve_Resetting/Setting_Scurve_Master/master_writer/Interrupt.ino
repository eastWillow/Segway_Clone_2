void interrupt_initial(void) {
  pinMode(7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(7), interruptReset, FALLING);
}
void interruptReset(void) {
  interruptResetFlag = true;
}

