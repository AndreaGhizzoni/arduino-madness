void setup() {
  Serial.begin(2000000);

  pinMode(6, INPUT_PULLUP);
}


void loop() {
  //Serial.println(currentState);
  if( !digitalRead(6) == HIGH ) {
    Serial.println("1");
  }
}
