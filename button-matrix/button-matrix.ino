#include <Keypad.h>
#include <Joystick.h>

const byte ROWS = 1; //four rows
const byte COLS = 2; //four columns

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2'}
};

byte rowPins[ROWS] = {10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

Joystick_ Joystick;

void setup(){
  Serial.begin(9600);

  customKeypad.setHoldTime(10);               // Default is 1000mS
  customKeypad.setDebounceTime(50);           // Default is 50mS
  // Initialize Joystick Library
  Joystick.begin();
}

void loop(){
  char customKey = customKeypad.getKey();
  if(customKey){
    //Joystick.setButton( customKey, 1 );
    Serial.println(customKey);
  }
}
