/**
 * Tested with encoder KY-040
 */

#include <Encoder.h>
#include <Joystick.h>

//#define DEBUG 0

Joystick_ Joystick;

long encoderMIN = 0;
long encoderMAX = 1023;
long encoderSTEP = 25;
int RIGHT_ROTATION = -1;
int LEFT_ROTATION = 1;
const int sizeofEncoders = 3;
// Encoder(CLK_pin, DT_pin)
Encoder encoders[sizeofEncoders] = { Encoder(1, 0), Encoder(2, 3), Encoder(4, 5) };
long encoderStatus[3] = { 0, 0, 0 };
long joystickValues[3] = { encoderMAX/2, encoderMAX/2, encoderMAX/2 };


void setup(){
  Serial.begin(2000000);
  Joystick.begin();

  // Joystick.setRxAxisRange(encoderMIN, encoderMAX);
  // Joystick.setRyAxisRange(encoderMIN, encoderMAX);
  // Joystick.setRzAxisRange(encoderMIN, encoderMAX);

  // for( int i=0; i<sizeofEncoders; i++ ){
  //   applyValueToJoystick( i, joystickValues[i] );
  // }
}

void applyValueToJoystick( int encoderIndex, long value ){
  switch( encoderIndex ){
    case 0: Joystick.setRxAxis( value ); break;
    case 1: Joystick.setRyAxis( value ); break;
    case 2: Joystick.setRzAxis( value ); break;
  }
}

void stepEncoder( int encoderIndex, int rORl ){
  joystickValues[encoderIndex] += (rORl)*encoderSTEP;
  if( joystickValues[encoderIndex] > encoderMAX ){
    joystickValues[encoderIndex] = encoderMAX;
  }else if( joystickValues[encoderIndex] < encoderMIN ){
    joystickValues[encoderIndex] = encoderMIN;
  }
  applyValueToJoystick( encoderIndex, joystickValues[encoderIndex] );
}

void readFromEncoder( int encoderIndex ){
  long enc_pos = encoders[encoderIndex].read();
  enc_pos /= 4;
  
  if( enc_pos > encoderStatus[encoderIndex] ){ // right
    encoderStatus[encoderIndex] = enc_pos;
    stepEncoder( encoderIndex, RIGHT_ROTATION ); 
    
    //if( DEBUG == 1) printDebugInfo( encoderIndex );
  }else if( enc_pos < encoderStatus[encoderIndex] ){ // left
    encoderStatus[encoderIndex] = enc_pos;
    stepEncoder( encoderIndex, LEFT_ROTATION ); 
    
    //if( DEBUG == 1) printDebugInfo( encoderIndex );
  }
}

// void printDebugInfo( int encoderIndex ){
//   Serial.print(encoderIndex);
//   Serial.print(" - ");
//   Serial.print(encoderStatus[encoderIndex]);
//   Serial.print(" - ");
//   Serial.println(joystickValues[encoderIndex]);
// }

void loop(){
  for( int i=0; i<sizeofEncoders; i++ ){
    readFromEncoder(i);
  }
}
