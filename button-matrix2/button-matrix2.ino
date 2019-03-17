/* @file MultiKey.ino
|| @version 1.0
|| @author Mark Stanley
|| @contact mstanley@technologist.com
||
|| @description
|| | The latest version, 3.0, of the keypad library supports up to 10
|| | active keys all being pressed at the same time. This sketch is an
|| | example of how you can get multiple key presses from a keypad or
|| | keyboard.
|| #
*/

#include <Keypad.h>
#include <Joystick.h>

Joystick_ Joystick;

const byte ROWS = 1; //four rows
const byte COLS = 2; //three columns
char keys[ROWS][COLS] = {
  {'0','1'}
};
byte rowPins[ROWS] = {10}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {8,9}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//unsigned long loopCount;
//unsigned long startTime;
String msg;

void setup() {
    Serial.begin(2000000);
    /*
    loopCount = 0;
    startTime = millis();
    */
    msg = "";
    pinMode(A3, INPUT_PULLUP);
    
    Joystick.begin();
}

int char2int(char c){
  if(c=='0') return 0;
  else if(c=='1') return 1;
  else if(c=='2') return 2;
  else return -1;
}

void loop() {
    /*
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }
    */

    if( !digitalRead(A3) )
      Serial.println("1");
      
    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if( kpd.getKeys() ){
        for( int i=0; i<LIST_MAX; i++ )   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch( kpd.key[i].kstate ) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                case PRESSED:
                    msg = " PRESSED.";
                    Joystick.setButton(char2int(kpd.key[i].kchar), 1);
                    break;
                case HOLD:
                    msg = " HOLD.";
                    break;
                case RELEASED:
                    msg = " RELEASED.";
                    Joystick.setButton(char2int(kpd.key[i].kchar), 0);
                    break;
                case IDLE:
                    msg = " IDLE.";
                }
                Serial.print("Key ");
                Serial.print(kpd.key[i].kchar);
                Serial.println(msg);
            }
        }
    }
}  // End loop
