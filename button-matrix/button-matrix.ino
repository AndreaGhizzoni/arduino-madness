#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 5;
char keys[ROWS][COLS] = {
  {'0', '1', '2', 'a', 'b'},
  {'3', '4', '5', 'c', 'd'},
  {'6', '7', '8', 'e', 'f'},
  {'9', '!', '@', 'g', 'h'},
};
byte rowPins[ROWS] = {15, 14, 16, 10};
byte colPins[COLS] = {5, 6, 7, 8, 9};
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
  else if(c=='a') return 12;
  else if(c=='b') return 13;
  else if(c=='c') return 14;
  else if(c=='d') return 15;
  else if(c=='e') return 16;
  else if(c=='f') return 17;
  else if(c=='g') return 18;
  else if(c=='h') return 19;
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
