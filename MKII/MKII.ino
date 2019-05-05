#include <Encoder.h>
#include <Keypad.h>
#include <Joystick.h>

#define DEBUG 0

const int sizeofEncoders = 2;
long encoderMIN = 0;
long encoderMAX = 1023;
long encoderSTEP = 25;
int RIGHT_STEP_ROTATION = -1;
int LEFT_STEP_ROTATION = 1;

Encoder encoders[sizeofEncoders] = { Encoder(1, 0), Encoder(2, 3) }; // Encoder(CLK, DT)
long joystickValues[sizeofEncoders] = { encoderMAX/2, encoderMAX/2 };
long encoderStatus[sizeofEncoders] = { 0, 0 };
int encoderButton[sizeofEncoders] = {4, 5};
int encoderButtonState[sizeofEncoders] = {0, 0};

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'0', '1', '2'},
  {'3', '4', '5'},
  {'6', '7', '8'},
  {'9', '!', '@'},
};
byte rowPins[ROWS] = {10,16,14,15};
byte colPins[COLS] = {7,8,9};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int switchState = LOW;
int switchPin = 6;


Joystick_ Joystick(
  0x04,     // joystick's unique HID
  JOYSTICK_TYPE_JOYSTICK, // HID input device type
  33,       // button count: default 32
  2,        // hat switches: default 2
  false,    // include X Axis
  false,    // include Y Axis
  false,    // include Z Axis
  false,    // include Rx Axis
  false,    // include Ry Axis 
  false,    // include Rz Axis 
  false,    // include Rudder Axis
  false,    // include Throttle Axis 
  false,    // include Accelerator Axis
  false,    // include Brake Axis
  false     // include Steering Axis
);

void setup(){
  Serial.begin(2000000);
  Joystick.begin();

  Joystick.setRxAxisRange(encoderMIN, encoderMAX);
  Joystick.setRyAxisRange(encoderMIN, encoderMAX);
  Joystick.setRzAxisRange(encoderMIN, encoderMAX);

  for( int i=0; i<sizeofEncoders; i++ ){
    applyValueToJoystick( i, joystickValues[i] );
    pinMode(encoderButton[i], INPUT_PULLUP);
  }

  pinMode(switchPin, INPUT_PULLUP);
}

void applyValueToJoystick( int encoderIndex, long value ){
  switch( encoderIndex ){
    case 0: Joystick.setRxAxis( value ); break;
    case 1: Joystick.setRyAxis( value ); break;
    case 2: Joystick.setRzAxis( value ); break;
  }
}

void stepJoystick( int encoderIndex, int rORl ){
  joystickValues[encoderIndex] += (rORl)*encoderSTEP;
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
    stepJoystick( encoderIndex, RIGHT_STEP_ROTATION ); // right
    
    //if( DEBUG == 1) printDebugInfo( encoderIndex );
  }else if( enc_pos < encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, LEFT_STEP_ROTATION ); // left
    
    //if( DEBUG == 1) printDebugInfo( encoderIndex );
  }
}

int char2int( char c ){
  if(c=='0') return 0+(switchState*12);
  else if(c=='1') return 1+(switchState*12);
  else if(c=='2') return 2+(switchState*12);
  else if(c=='3') return 3+(switchState*12);
  else if(c=='4') return 4+(switchState*12);
  else if(c=='5') return 5+(switchState*12);
  else if(c=='6') return 6+(switchState*12);
  else if(c=='7') return 7+(switchState*12);
  else if(c=='8') return 8+(switchState*12);
  else if(c=='9') return 9+(switchState*12);
  else if(c=='!') return 10+(switchState*12);
  else if(c=='@') return 11+(switchState*12);
  else return -1;
}

int int2int( int e ){
  if( e == 0 ) return 31-(switchState*2);
  else if( e == 1 ) return 30-(switchState*2);
  else return -1;
}

//void printDebugInfo( int encoderIndex ){
//  Serial.print(encoderIndex);
//  Serial.print(" - ");
//  Serial.print(encoderStatus[encoderIndex]);
//  Serial.print(" - ");
//  Serial.println(joystickValues[encoderIndex]);
//}

void loop(){
  int currentSwitchState = !digitalRead(switchPin);
  if( currentSwitchState != switchState ){
    switchState = currentSwitchState;
  }
  
  for( int i=0; i<sizeofEncoders; i++ ){
    int currentEncoderButtonState = !digitalRead(encoderButton[i]);
    if( currentEncoderButtonState != encoderButtonState[i] ){
      encoderButtonState[i] = currentEncoderButtonState;
      //Serial.println(encoderButtonState[i]);
      if( encoderButtonState[i] == HIGH )
        Joystick.setButton(int2int(i), 1);
      else if( encoderButtonState[i] == LOW )
        Joystick.setButton(int2int(i), 0);
    }
    
    readFrom(i);
  }

  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if( kpd.getKeys() ){
    for( int i=0; i<LIST_MAX; i++ ){   // Scan the whole key list.
      if( kpd.key[i].stateChanged ){   // Only find keys that have changed state.
        switch( kpd.key[i].kstate ) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
        case PRESSED:
            Joystick.setButton(char2int(kpd.key[i].kchar), 1);
            break;
        case HOLD: break;
        case RELEASED:
            Joystick.setButton(char2int(kpd.key[i].kchar), 0);
            break;
        case IDLE: break;
        }
      }
    }
  }
}
