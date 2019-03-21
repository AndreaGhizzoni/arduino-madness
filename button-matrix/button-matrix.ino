#include <Keypad.h>

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

String msg = "";

void setup() {
    Serial.begin(2000000);
}

int char2int( char c ){
  if(c=='0') return 0;
  else if(c=='1') return 1;
  else if(c=='2') return 2;
  else if(c=='3') return 3;
  else if(c=='4') return 4;
  else if(c=='5') return 5;
  else if(c=='6') return 6;
  else if(c=='7') return 7;
  else if(c=='8') return 8;
  else if(c=='9') return 9;
  else if(c=='!') return 10;
  else if(c=='@') return 11;
  else return -1;
}

void loop() {     
    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if( kpd.getKeys() ){
        for( int i=0; i<LIST_MAX; i++ ){  // Scan the whole key list.
            if( kpd.key[i].stateChanged ){  // Only find keys that have changed state.
                switch( kpd.key[i].kstate ) { // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                case PRESSED:
                    msg = " PRESSED.";
                    break;
                case HOLD:
                    msg = " HOLD.";
                    break;
                case RELEASED:
                    msg = " RELEASED.";
                    break;
                case IDLE:
                    msg = " IDLE.";
                    break;
                }
                Serial.print("Key ");
                Serial.print(kpd.key[i].kchar);
                Serial.println(msg);
            }
        }
    }
}
