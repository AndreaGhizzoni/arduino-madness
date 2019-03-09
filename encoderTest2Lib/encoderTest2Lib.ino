/* 
* Dimodifikasi dari kode contoh:
* Encoder Library - Basic Example
* http://www.pjrc.com/teensy/td_libs_Encoder.html
*
* This example code is in the public domain.
*/

#include <Encoder.h>
#include <Joystick.h>

#define DEBUG 1

Joystick_ Joystick;

int sizeofEncoders = 3;
long encoderMIN = 0;
long encoderMAX = 1023;
long encoderSTEP = 50;
// Encoder(CLK, DT)
Encoder encoders[3] = { Encoder(1, 0), Encoder(2, 3), Encoder(4, 5) };
long joystickValues[3] = { encoderMAX/2, encoderMAX/2, encoderMAX/2 };
long encoderStatus[3] = { 0, 0, 0 };

void setup(){
  Serial.begin(2000000);
  Joystick.begin();

  Joystick.setRxAxisRange(encoderMIN, encoderMAX);
  Joystick.setRyAxisRange(encoderMIN, encoderMAX);
  Joystick.setRzAxisRange(encoderMIN, encoderMAX);

  Joystick.setRxAxis( joystickValues[0] );
  Joystick.setRyAxis( joystickValues[1] );
  Joystick.setRzAxis( joystickValues[2] );
}

void applyValueToJoystick( int encoderIndex, long value ){
  switch( encoderIndex ){
    case 0: Joystick.setRxAxis( value );
            break;
    case 1: Joystick.setRyAxis( value );
            break;
    case 2: Joystick.setRzAxis( value );
            break;
  }
}

long getNextJoystickValue( int rotation ){
  return (rotation)*encoderSTEP;
}

void stepJoystick( int encoderIndex, int rORl ){
  joystickValues[encoderIndex] += getNextJoystickValue(rORl);
  if( joystickValues[encoderIndex] > encoderMAX ){
    joystickValues[encoderIndex] = encoderMAX;
  }else if( joystickValues[encoderIndex] < encoderMIN ){
    joystickValues[encoderIndex] = encoderMIN;
  }
  applyValueToJoystick( encoderIndex, joystickValues[encoderIndex] );
}

void readFrom( int encoderIndex ){
  long enc_pos = encoders[encoderIndex].read();
  enc_pos /= 4;
  
  if( enc_pos > encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, 1 ); // right
    
    if( DEBUG == 1){
      Serial.print(encoderIndex);
      Serial.print(" - ");
      Serial.println(encoderStatus[encoderIndex]);
    }     
  }else if( enc_pos < encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, -1 ); // left
    
    if( DEBUG == 1){
      Serial.print(encoderIndex);
      Serial.print(" - ");
      Serial.println(encoderStatus[encoderIndex]);
    } 
  }
  
//  long axis_pos = mapEncoderPositionToJoystick( enc_pos );
//  if( axis_pos != encoderStatus[encoderIndex] ) {
//    encoderStatus[encoderIndex] = axis_pos;
//    applyToJoystick( encoderIndex );
//    
//    if( DEBUG == 1){
//      Serial.print(encoderIndex);
//      Serial.print(" - ");
//      Serial.println(encoderStatus[encoderIndex]);
//    }    
//  }
}

void loop(){
  for( int i=0; i<sizeofEncoders; i++ ){
    readFrom(i);
  }
}
