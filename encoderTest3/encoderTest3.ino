#include <Joystick.h>

// Create the Joystick
//Joystick_ Joystick;

#define CLK 0
#define DT 1
#define SW 2

volatile int encoderCount = 0;
//int encoderCountLastState = encoderCount;

void setup() {
  Serial.begin (2000000);
  
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), isr, RISING);
  //attachInterrupt(digitalPinToInterrupt(CLK), readEncoder, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(DT), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SW), swPressed, FALLING);
  
  // Initialize Joystick Library
  //Joystick.begin();
}

void loop() {
  /*
  if( encoderCountLastState != encoderCount ){
    //Joystick.setXAxis(encoderCount);
    encoderCountLastState = encoderCount;
    
    Serial.print( "pos:" );
    Serial.println( encoderCount, DEC );
  }
  */
}

volatile int clkCurrentState = LOW;
volatile int dtCurrentState = LOW;
void isr(){
  d();  
  //p();
}

void d(){
  Serial.print( digitalRead(CLK) );
  Serial.print( " - " );
  Serial.println( digitalRead(DT) );  
}

void p(){
  if( digitalRead(CLK) == HIGH ){
    if( digitalRead(DT) == HIGH ) {
      encoderCount += 1;
      Serial.println( encoderCount );
    }else{
      encoderCount -= 1;
      Serial.println( encoderCount );
    }
  }
}

void readEncoder() {
  encoderCount += ( digitalRead(CLK) == digitalRead(DT) ) ? 10 : -10;
}

void swPressed(){
  encoderCount = 0;
}
