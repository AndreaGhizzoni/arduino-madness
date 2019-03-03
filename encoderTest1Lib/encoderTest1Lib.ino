/* 
* Dimodifikasi dari kode contoh:
* Encoder Library - Basic Example
* http://www.pjrc.com/teensy/td_libs_Encoder.html
*
* This example code is in the public domain.
*/

#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability. 
//                     Pin 2 & pin 3 di Arduino Uno 
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(0, 1);
//   avoid using pins with LEDs attached

void setup() 
{
  Serial.begin(2000000);
  Serial.println("Basic Encoder Test:");
}

long oldPosition  = -999;
//long oldPosition  = 0;


void loop() 
{
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) 
  {
    oldPosition = newPosition;
    Serial.println(newPosition);
    //Serial.println(newPosition/4);
  }
}
