#include <Encoder.h>
#include <Keypad.h>
#include <Joystick.h>

#define DEBUG 0

long encoderMIN = 0;
long encoderMAX = 1023;
long encoderSTEP = 25;
int RIGHT_STEP_ROTATION = -1;
int LEFT_STEP_ROTATION = 1;

const int sizeofEncoders = 2;
Encoder encoders[sizeofEncoders] = { Encoder(1, 0), Encoder(2, 3) }; // Encoder(CLK, DT)
long joystickValues[sizeofEncoders] = { encoderMAX/2, encoderMAX/2 };
long encoderStatus[sizeofEncoders] = { 0, 0 };
int enableEncoderButtons = 0;
int encoderButton[sizeofEncoders] = { 4, 5 };
int encoderButtonState[sizeofEncoders] = { LOW, LOW };

const byte ROWS = 2;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'0', '1', '2', '.'},
  {'3', '4', '5', '?'}
};
byte rowPins[ROWS] = {16,10};
byte colPins[COLS] = {6,7,8,9};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int sizeofSparePins = 3;
int sparePin[sizeofSparePins] = {4, 5, 14};
int sparePinState[sizeofSparePins] = {LOW, LOW, LOW};

int switchPin = 15;
int switchState = LOW;

Joystick_ Joystick(
  0x04,     // joystick's unique HID
  JOYSTICK_TYPE_JOYSTICK, // HID input device type
  32,       // button count: default and max 32
  2,        // hat switches: default 2
  true,    // include X Axis
  true,    // include Y Axis
  true,    // include Z Axis
  true,    // include Rx Axis
  true,    // include Ry Axis 
  true,    // include Rz Axis 
  true,    // include Rudder Axis
  true,    // include Throttle Axis 
  true,    // include Accelerator Axis
  true,    // include Brake Axis
  true     // include Steering Axis
);

void setup(){
  Serial.begin(2000000);
  Joystick.begin();

  Joystick.setXAxisRange(encoderMIN, encoderMAX);
  Joystick.setYAxisRange(encoderMIN, encoderMAX);
  Joystick.setZAxisRange(encoderMIN, encoderMAX);
  Joystick.setRxAxisRange(encoderMIN, encoderMAX);
  Joystick.setRyAxisRange(encoderMIN, encoderMAX);
  Joystick.setRzAxisRange(encoderMIN, encoderMAX);
  Joystick.setRudderRange(encoderMIN, encoderMAX);
  Joystick.setThrottleRange(encoderMIN, encoderMAX);
  Joystick.setAcceleratorRange(encoderMIN, encoderMAX);
  Joystick.setBrakeRange(encoderMIN, encoderMAX);
  Joystick.setSteeringRange(encoderMIN, encoderMAX);

  for( int i=0; i<sizeofEncoders; i++ ){
    applyValueToJoystick( i, joystickValues[i] );
    if( enableEncoderButtons == 1 )
      pinMode(encoderButton[i], INPUT_PULLUP);
  }

  pinMode(switchPin, INPUT_PULLUP);
  
  for( int j=0; j<sizeofSparePins; j++ ){
    pinMode(sparePin[j], INPUT_PULLUP);
  }
}

void applyValueToJoystick( int encoderIndex, long value ){
  switch( encoderIndex ){
    case  0: Joystick.setRxAxis( value ); break;
    case  1: Joystick.setRyAxis( value ); break;
    case  2: Joystick.setRzAxis( value ); break;
    case  3: Joystick.setXAxis( value ); break;
    case  4: Joystick.setYAxis( value ); break;
    case  5: Joystick.setZAxis( value ); break;
    case  6: Joystick.setRudder( value ); break;
    case  7: Joystick.setThrottle( value ); break;
    case  8: Joystick.setAccelerator( value ); break;
    case  9: Joystick.setBrake( value ); break;
    case 10: Joystick.setSteering( value ); break;
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


void readEncoder( int encoderIndex ){
  long enc_pos = encoders[encoderIndex].read();
  enc_pos /= 4;
  
  if( enc_pos > encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, RIGHT_STEP_ROTATION ); // right
    
    if( DEBUG == 1 ) printDebugInfo( encoderIndex );
  }else if( enc_pos < encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, LEFT_STEP_ROTATION ); // left
    
    if( DEBUG == 1 ) printDebugInfo( encoderIndex );
  }
}

int convertChar2KeyStroke( char c ){
  int count = 0;
  int rowsTimesColumns = ROWS*COLS;
  for( int i=0; i<ROWS; i++ ){
    for( int j=0; j<COLS; j++ ){
      if( c == keys[i][j] ) return count+(switchState*rowsTimesColumns);
      count++;
    }
  }
}

int convertInput2KeyStroke( int base, int i, int span ){
  return base-i-(switchState*span);
}

void printDebugInfo( int encoderIndex ){
  Serial.print(encoderIndex);
  Serial.print(" - ");
  Serial.print(encoderStatus[encoderIndex]);
  Serial.print(" - ");
  Serial.println(joystickValues[encoderIndex]);
}

void loop(){
  // READING MODE SWITCH
  int currentSwitchState = !digitalRead(switchPin);
  if( currentSwitchState != switchState ){
    switchState = currentSwitchState;
    if( DEBUG == 1 )
      Serial.print("switch state: ");Serial.println(switchState);
  }

  // READING SPARE PINS
  for( int i=0; i<sizeofSparePins; i++ ){
    int sparePinCurrentState = !digitalRead( sparePin[i] );
    if( sparePinCurrentState != sparePinState[i] ){
      sparePinState[i] = sparePinCurrentState;
      if( sparePinState[i] == HIGH ) Joystick.setButton( convertInput2KeyStroke(29, i, sizeofSparePins), 1 );
      else if( sparePinState[i] == LOW ) Joystick.setButton( convertInput2KeyStroke(29, i, sizeofSparePins), 0 );
    }
  }

  // READING ENCODER
  for( int i=0; i<sizeofEncoders; i++ ){
    if( enableEncoderButtons == 1 ){    
      int currentEncoderButtonState = !digitalRead(encoderButton[i]);
      if( currentEncoderButtonState != encoderButtonState[i] ){
        encoderButtonState[i] = currentEncoderButtonState;
        if( encoderButtonState[i] == HIGH ) Joystick.setButton(convertInput2KeyStroke(31, i, 2), 1);
        else if( encoderButtonState[i] == LOW ) Joystick.setButton(convertInput2KeyStroke(31, i, 2), 0);
      }
    }
    readEncoder(i);
  }

  // READING BUTTON MATRIX
  if( kpd.getKeys() ){
    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    for( int i=0; i<LIST_MAX; i++ ){   // Scan the whole key list.
      if( kpd.key[i].stateChanged ){   // Only find keys that have changed state.
        switch( kpd.key[i].kstate ) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
        case PRESSED:
            if( DEBUG == 1 ){
              Serial.print("press ");
              Serial.print(kpd.key[i].kchar);Serial.print(" ");
              Serial.println(convertChar2KeyStroke(kpd.key[i].kchar));
            }
            Joystick.setButton(convertChar2KeyStroke(kpd.key[i].kchar), 1);
            break;
        case HOLD: break;
            Serial.println("hold");
        case RELEASED:
            if( DEBUG == 1 ){
              Serial.print("released ");
              Serial.print(kpd.key[i].kchar);Serial.print(" ");
              Serial.println(convertChar2KeyStroke(kpd.key[i].kchar));
            }
            Joystick.setButton(convertChar2KeyStroke(kpd.key[i].kchar), 0);
            break;
        case IDLE: break;
        }
      }
    }
  }
}
