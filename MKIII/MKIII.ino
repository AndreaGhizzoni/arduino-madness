#include <Encoder.h>
#include <Keypad.h>
#include <Joystick.h>

#define DEBUG 0

// ARDUINO PRO MICRO PINOUT
#define PIN_ONE 1
#define PIN_ZERO 0
#define PIN_TWO 2
#define PIN_THREE 3
#define PIN_FOUR 4
#define PIN_FIVE 5
#define PIN_SIX 6
#define PIN_SEVEN 7
#define PIN_EIGHT 8
#define PIN_NINE 9
#define PIN_TEN 10
#define PIN_SIXTEEN 16
#define PIN_FOURTEEN 14
#define PIN_FIFTHTEEN 15
// analog
#define PIN_EIGHTTEEN 6
#define PIN_NINETEEN 6
#define PIN_TWENTY 6
#define PIN_TWENTYONE 6


//==========================================================================================
// ENCODER SETTINGS
//==========================================================================================
#define ENCODER_MIN 0
#define ENCODER_MAX 1023
#define ENCODER_STEP 25
#define ENCODER_DEFAULT ENCODER_MAX/2

#define ENCODERS_SIZE 2
Encoder encoders[ENCODERS_SIZE] = { Encoder(1, 0), Encoder(2, 3) }; // Encoder(CLK, DT)
long encoderRawValues[ENCODERS_SIZE] = { ENCODER_MIN, ENCODER_MIN };
long encoderValues[ENCODERS_SIZE] = { ENCODER_DEFAULT, ENCODER_DEFAULT };

#define ENCODER_ENABLE_BUTTON 0
int encoderButton[ENCODERS_SIZE] = { 4, 5 };
int encoderButtonState[ENCODERS_SIZE] = { LOW, LOW };

//==========================================================================================
// MODE SWITCH
//==========================================================================================
#define MODE_SWITCH_ENABLE 1
#define PIN_MODE_SWITCH 15
int modeSwitchState = LOW;

//==========================================================================================
// MATRIX BUTTON
//==========================================================================================
#define ROWS 2
#define COLS 4
char keys[ROWS][COLS] = {
  {'0', '1', '2', '.'},
  {'3', '4', '5', '?'}
};
byte rowPins[ROWS] = {16, 10};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//==========================================================================================
// SPARE BUTTON
//==========================================================================================
#define SPARE_PINS_SIZE 3
int sparePin[SPARE_PINS_SIZE] = {4, 5, 14};
int sparePinState[SPARE_PINS_SIZE] = {LOW, LOW, LOW};


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
    applyValueToJoystick( i, encoderValues[i] );
    
    if( ENCODER_ENABLE_BUTTON == 1 )
      pinMode(encoderButton[i], INPUT_PULLUP);
  }

  if( MODE_SWITCH_ENABLE == 1 ){
    pinMode(PIN_MODE_SWITCH, INPUT_PULLUP);
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

void stepEncoderRight( int encoderIndex ){
  stepEncoder( encoderIndex, -1 );
}

void stepEncoderLeft( int encoderIndex ){
  stepEncoder( encoderIndex, 1 );
}

void stepEncoder( int encoderIndex, int rORl ){
  int newEncoderkValue = encoderValues[encoderIndex] + (rORl)*ENCODER_STEP;

  if( newEncoderkValue > ENCODER_MAX ){
    newEncoderkValue = ENCODER_MAX;
    
  }else if( newEncoderkValue < ENCODER_MIN ){
    newEncoderkValue = ENCODER_MIN;
  }

  encoderValues[encoderIndex] = newEncoderkValue;
  applyValueToJoystick( encoderIndex, encoderValues[encoderIndex] );
}

void readEncoder( int encoderIndex ){
  long enc_pos = encoders[encoderIndex].read();
  enc_pos /= 4;
  
  if( enc_pos > encoderRawValues[encoderIndex] ){
    encoderRawValues[encoderIndex] = enc_pos;
    stepEncoderRight( encoderIndex );
    if( DEBUG == 1 ) printDebugEncoder( encoderIndex );
    
  }else if( enc_pos < encoderRawValues[encoderIndex] ){
    encoderRawValues[encoderIndex] = enc_pos;
    stepEncoderLeft( encoderIndex );
    if( DEBUG == 1 ) printDebugEncoder( encoderIndex );
  }
}

int convertChar2KeyStroke( char c ){
  int count = 0;
  int rowsTimesColumns = ROWS*COLS;
  for( int i=0; i<ROWS; i++ ){
    for( int j=0; j<COLS; j++ ){
      if( c == keys[i][j] ) return convertInput2KeyStroke(count, 0, rowsTimesColumns); 
      count++;
    }
  }
}

int convertInput2KeyStroke( int base, int i, int span ){
  return base-i-(modeSwitchState*span);
}

void printDebugEncoder( int encoderIndex ){
  Serial.print(encoderIndex); Serial.print(" - ");
  Serial.print(encoderRawValues[encoderIndex]); Serial.print(" - ");
  Serial.println(encoderValues[encoderIndex]);
}

void printDebugSwitchState(){
  Serial.print("switch state: "); Serial.println(modeSwitchState);
}

void printDebugButtonMatrix( const char* message, char keyEvent ){
  Serial.print(message); Serial.print(" ");
  Serial.print(keyEvent); Serial.print(" ");
  Serial.println(convertChar2KeyStroke(keyEvent));
}

void loop(){
  // READING MODE SWITCH
  int currentSwitchState = !digitalRead(PIN_MODE_SWITCH);
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
