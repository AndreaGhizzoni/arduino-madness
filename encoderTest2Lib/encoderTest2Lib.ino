#include <Encoder.h>
#include <Joystick.h>

#define DEBUG 0

Joystick_ Joystick;
Encoder encoders[3] = { Encoder(1, 0), Encoder(2, 3), Encoder(4, 5) };

int sizeofEncoders = 3;
long encoderMIN = 0;
long encoderMAX = 1023;
long encoderSTEP = 25;
int RIGHT_STEP_ROTATION = -1;
int LEFT_STEP_ROTATION = 1;

// Encoder(CLK, DT)

long joystickValues[3] = { encoderMAX/2, encoderMAX/2, encoderMAX/2 };
long encoderStatus[3] = { 0, 0, 0 };

void setup(){
  Serial.begin(2000000);
  Joystick.begin();

  Joystick.setRxAxisRange(encoderMIN, encoderMAX);
  Joystick.setRyAxisRange(encoderMIN, encoderMAX);
  Joystick.setRzAxisRange(encoderMIN, encoderMAX);

  for( int i=0; i<sizeofEncoders; i++ ){
    applyValueToJoystick( i, joystickValues[i] );
  }
}

void applyValueToJoystick( int encoderIndex, long value ){
  switch( encoderIndex ){
    case 0: Joystick.setRxAxis( value ); break;
    case 1: Joystick.setRyAxis( value ); break;
    case 2: Joystick.setRzAxis( value ); break;
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

void printDebugInfo( int encoderIndex ){
  Serial.print(encoderIndex);
  Serial.print(" - ");
  Serial.print(encoderStatus[encoderIndex]);
  Serial.print(" - ");
  Serial.println(joystickValues[encoderIndex]);
}

void readFrom( int encoderIndex ){
  long enc_pos = encoders[encoderIndex].read();
  enc_pos /= 4;
  
  if( enc_pos > encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, RIGHT_STEP_ROTATION ); // right
    
    if( DEBUG == 1) printDebugInfo( encoderIndex );
  }else if( enc_pos < encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, LEFT_STEP_ROTATION ); // left
    
    if( DEBUG == 1) printDebugInfo( encoderIndex );
  }
}

void loop(){
  for( int i=0; i<sizeofEncoders; i++ ){
    readFrom(i);
  }
}
