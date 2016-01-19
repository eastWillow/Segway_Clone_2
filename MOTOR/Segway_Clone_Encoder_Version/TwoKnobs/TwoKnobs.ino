/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */
#include <Encoder.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobLeft(0, 1);
Encoder knobRight(2, 3);
//   avoid using pins with LEDs attached

void setup() {
  mySerial.begin(115200);
  mySerial.println("Hello, world?");
}

long positionLeft  = -999;
long positionRight = -999;

void loop() {
  long newLeft, newRight;
  newLeft = knobLeft.read()/32;
  newRight = knobRight.read()/32;
  if (newLeft != positionLeft || newRight != positionRight) {
    mySerial.print(newLeft);
    mySerial.print(",");
    mySerial.print(newRight);
    mySerial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (mySerial.available()) {
    mySerial.read();
    mySerial.println("Reset both knobs to zero");
    knobLeft.write(0);
    knobRight.write(0);
  }
}
