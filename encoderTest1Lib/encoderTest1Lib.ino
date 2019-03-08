/* 
* Dimodifikasi dari kode contoh:
* Encoder Library - Basic Example
* http://www.pjrc.com/teensy/td_libs_Encoder.html
*
* This example code is in the public domain.
*/

#include <Encoder.h>
#include <Joystick.h>

#define DEBUG 0

Joystick_ Joystick;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability. 
//                     Pin 2 & pin 3 di Arduino Uno 
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

Encoder encoders[3] = { Encoder(0, 1), Encoder(2, 3), Encoder(4, 5) };
int sizeofEncoders = 3;
long encoderMIN = 0;
long encoderMAX = 1023;
long encoderSTEP = 50;
long encoderStatus[3] = { encoderMAX/2, encoderMAX/2, encoderMAX/2 };

void setup(){
  Serial.begin(2000000);
  Joystick.begin();

  Joystick.setRxAxisRange(encoderMIN, encoderMAX);
  Joystick.setRyAxisRange(encoderMIN, encoderMAX);
  Joystick.setRzAxisRange(encoderMIN, encoderMAX);

  Joystick.setRxAxis(encoderStatus[0]);
  Joystick.setRyAxis(encoderStatus[1]);
  Joystick.setRzAxis(encoderStatus[2]);
}

long mapEncoderPositionToJoystick( long encoderPosition ){
  long axisPosition = (encoderPosition/4)*encoderSTEP;
  
  if( axisPosition >= encoderMAX ) return encoderMAX;
  else if( axisPosition <= encoderMIN ) return encoderMIN;
  else return axisPosition;
}

void applyToJoystick( int encoderIndex ){
  switch( encoderIndex ){
    case 0: Joystick.setRxAxis( encoderStatus[encoderIndex] );
            break;
    case 1: Joystick.setRyAxis( encoderStatus[encoderIndex] );
            break;
    case 2: Joystick.setRzAxis( encoderStatus[encoderIndex] );
            break;
  }
}

void readFrom( int encoderIndex ){
  long enc_pos = encoders[encoderIndex].read();
  long axis_pos = mapEncoderPositionToJoystick( -enc_pos );
  
  if( axis_pos != encoderStatus[encoderIndex] ) {
    encoderStatus[encoderIndex] = axis_pos;
    applyToJoystick( encoderIndex );
    
    if( DEBUG == 1){
      Serial.print(encoderIndex);
      Serial.print(" - ");
      Serial.println(encoderStatus[encoderIndex]);
    }    
  }
}

void loop(){
  for( int i=0; i<sizeofEncoders; i++ ){
    readFrom(i);
  }
}
