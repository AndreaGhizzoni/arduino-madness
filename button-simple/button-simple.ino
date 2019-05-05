#define INPUT 0

int pins[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 14, 15 ,16};
int numOfPins = 14;

void setup() {
  Serial.begin(2000000);
  for( int i=0; i<14; i++ ) {
    pinMode(pins[i], INPUT_PULLUP);
  }  
}


void loop() {
  //Serial.println(currentState);
  for( int i=0; i<14; i++ ) {
      if( !digitalRead(pins[i]) == HIGH ) {
        Serial.println(pins[i]);
        delay(1000);
      }
  }  
}
