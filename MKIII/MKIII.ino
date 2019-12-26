#include <Encoder.h>
#include <Keypad.h>
#include <Joystick.h>

#define DEBUG 0

#define ENCODER_MIN 0
#define ENCODER_MAX 1023
#define ENCODER_STEP 25
#define ENCODER_RIGHT_STEP_ROTATION -1
#define ENCODER_LEFT_STEP_ROTATION 1

#define ENCODERS_SIZE 2
Encoder encoders[ENCODERS_SIZE] = { Encoder(1, 0), Encoder(2, 3) }; // Encoder(CLK, DT)
long joystickValues[ENCODERS_SIZE] = { ENCODER_MAX/2, ENCODER_MAX/2 };
long encoderStatus[ENCODERS_SIZE] = { ENCODER_MIN, ENCODER_MIN };

#define ENCODER_ENABLE_BUTTON 0
int encoderButton[ENCODERS_SIZE] = { 4, 5 };
int encoderButtonState[ENCODERS_SIZE] = { LOW, LOW };

const byte ROWS = 2;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'0', '1', '2', '.'},
  {'3', '4', '5', '?'}
};
byte rowPins[ROWS] = {16,10};
byte colPins[COLS] = {6,7,8,9};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define SPARE_PINS_SIZE 3
int sparePin[SPARE_PINS_SIZE] = {4, 5, 14};
int sparePinState[SPARE_PINS_SIZE] = {LOW, LOW, LOW};

#define MODE_SWITCH_ENABLE 1
const int pinModeSwitch = 15;
int modeSwitchState = LOW;

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

  Joystick.setXAxisRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setYAxisRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setZAxisRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setRxAxisRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setRyAxisRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setRzAxisRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setRudderRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setThrottleRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setAcceleratorRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setBrakeRange(ENCODER_MIN, ENCODER_MAX);
  Joystick.setSteeringRange(ENCODER_MIN, ENCODER_MAX);

  for( int i=0; i<ENCODERS_SIZE; i++ ){
    applyValueToJoystick( i, joystickValues[i] );
    if( ENCODER_ENABLE_BUTTON == 1 )
      pinMode(encoderButton[i], INPUT_PULLUP);
  }

  if( MODE_SWITCH_ENABLE == 1 ){
    pinMode(pinModeSwitch, INPUT_PULLUP);
  }
  
  for( int j=0; j<SPARE_PINS_SIZE; j++ ){
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
  joystickValues[encoderIndex] += (rORl)*ENCODER_STEP;
  if( joystickValues[encoderIndex] > ENCODER_MAX ){
    joystickValues[encoderIndex] = ENCODER_MAX;
  }else if( joystickValues[encoderIndex] < ENCODER_MIN ){
    joystickValues[encoderIndex] = ENCODER_MIN;
  }
  applyValueToJoystick( encoderIndex, joystickValues[encoderIndex] );
}


void readEncoder( int encoderIndex ){
  long enc_pos = encoders[encoderIndex].read();
  enc_pos /= 4;
  
  if( enc_pos > encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, ENCODER_RIGHT_STEP_ROTATION ); // right
    
    if( DEBUG == 1 ) printDebugEncoder( encoderIndex );
  }else if( enc_pos < encoderStatus[encoderIndex] ){
    encoderStatus[encoderIndex] = enc_pos;
    stepJoystick( encoderIndex, ENCODER_LEFT_STEP_ROTATION ); // left
    
    if( DEBUG == 1 ) printDebugEncoder( encoderIndex );
  }
}

int convertChar2KeyStroke( char c ){
  int count = 0;
  int rowsTimesColumns = ROWS*COLS;
  for( int i=0; i<ROWS; i++ ){
    for( int j=0; j<COLS; j++ ){
      if( c == keys[i][j] ) return convertInput2KeyStroke(count, 0, rowsTimesColumns); 
      //return count+(modeSwitchState*rowsTimesColumns);      
      count++;
    }
  }
}

int convertInput2KeyStroke( int base, int i, int span ){
  return base-i-(modeSwitchState*span);
}

void printDebugEncoder( int encoderIndex ){
  Serial.print(encoderIndex); Serial.print(" - ");
  Serial.print(encoderStatus[encoderIndex]); Serial.print(" - ");
  Serial.println(joystickValues[encoderIndex]);
}

void printDebugSwitchState(){
  Serial.print("switch state: "); Serial.println(modeSwitchState);
}

void printDebugButtonMatrix( char* message, char keyEvent ){
  Serial.print(message); Serial.print(" ");
  Serial.print(keyEvent); Serial.print(" ");
  Serial.println(convertChar2KeyStroke(keyEvent));
}

void loop(){
  // READING MODE SWITCH
  int currentSwitchState = !digitalRead(pinModeSwitch);
  if( currentSwitchState != modeSwitchState ){
    modeSwitchState = currentSwitchState;
    
    if( DEBUG == 1 ) printDebugSwitchState();
  }

  // READING SPARE PINS
  for( int i=0; i<SPARE_PINS_SIZE; i++ ){
    int sparePinCurrentState = !digitalRead( sparePin[i] );
    if( sparePinCurrentState != sparePinState[i] ){
      sparePinState[i] = sparePinCurrentState;
      if( sparePinState[i] == HIGH ) Joystick.setButton( convertInput2KeyStroke(29, i, SPARE_PINS_SIZE), 1 );
      else if( sparePinState[i] == LOW ) Joystick.setButton( convertInput2KeyStroke(29, i, SPARE_PINS_SIZE), 0 );
    }
  }

  // READING ENCODER
  for( int i=0; i<ENCODERS_SIZE; i++ ){
    if( ENCODER_ENABLE_BUTTON == 1 ){    
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
  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if( kpd.getKeys() ){  
    for( int i=0; i<LIST_MAX; i++ ){   // Scan the whole key list.
      if( kpd.key[i].stateChanged ){   // Only find keys that have changed state.
        switch( kpd.key[i].kstate ) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
        case PRESSED:
            if( DEBUG == 1 ) printDebugButtonMatrix("press", kpd.key[i].kchar);
            Joystick.setButton(convertChar2KeyStroke(kpd.key[i].kchar), 1);
            break;
        
        case HOLD: 
            if( DEBUG == 1 ) printDebugButtonMatrix("hold", kpd.key[i].kchar); 
            break;            
        
        case RELEASED:
            if( DEBUG == 1 ) printDebugButtonMatrix("released", kpd.key[i].kchar);
            Joystick.setButton(convertChar2KeyStroke(kpd.key[i].kchar), 0);
            break;
        
        case IDLE: break;
        }
      }
    }
  }
}
