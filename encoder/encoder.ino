/**
 * Tested with encoder KY-040
 */
#include <Encoder.h>

const long ENCODER_MIN = 0;
const long ENCODER_MAX = 1023;
const long ENCODER_STEP = 25;
const int RIGHT_ROTATION = -1;
const int LEFT_ROTATION = 1;
const int SIZE_OF_ENCODERS = 3;
// Encoder(CLK_pin, DT_pin)
Encoder encoders[SIZE_OF_ENCODERS] = { Encoder(1, 0), Encoder(2, 3), Encoder(4, 5) };
long encoderStatus[SIZE_OF_ENCODERS] = { 0, 0, 0 };
long encodedValues[SIZE_OF_ENCODERS] = { ENCODER_MAX/2, ENCODER_MAX/2, ENCODER_MAX/2 };


void setup(){
  Serial.begin(2000000);
}

void stepEncoder( int encoderIndex, int rORl ){
  encodedValues[encoderIndex] += (rORl)*ENCODER_STEP;
  if( encodedValues[encoderIndex] > ENCODER_MAX ){
    encodedValues[encoderIndex] = ENCODER_MAX;
  }else if( encodedValues[encoderIndex] < ENCODER_MIN ){
    encodedValues[encoderIndex] = ENCODER_MIN;
  }
}

void readFromEncoder( int encoderIndex ){
  long enc_pos = encoders[encoderIndex].read();
  enc_pos /= 4;
  
  if( enc_pos > encoderStatus[encoderIndex] ){ // right
    encoderStatus[encoderIndex] = enc_pos;
    stepEncoder( encoderIndex, RIGHT_ROTATION );
    
    printDebugInfo( encoderIndex ); 
  }else if( enc_pos < encoderStatus[encoderIndex] ){ // left
    encoderStatus[encoderIndex] = enc_pos;
    stepEncoder( encoderIndex, LEFT_ROTATION ); 
    
    printDebugInfo( encoderIndex );
  }
}

void printDebugInfo( int encoderIndex ){
  Serial.print("Encoder: ");
  Serial.print(encoderIndex);
  Serial.println("----------------------------");
  Serial.print("Internal Encoder Status: ");
  Serial.println(encoderStatus[encoderIndex]);
  Serial.print("Encoded Status: ");
  Serial.println(encodedValues[encoderIndex]);
  Serial.println("======================================");
}

void loop(){
  for( int i=0; i<SIZE_OF_ENCODERS; i++ ){
    readFromEncoder(i);
  }
}
