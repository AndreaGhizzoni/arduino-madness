void setup() {
  Serial.begin(2000000);
  pinMode(21, INPUT);
}

int isHIGH( long analog ){
  if( analog > 900 ) return HIGH;
  else return LOW;
}

void loop() {
//  if( isHIGH(analogRead(A3)) )
  if( digitalRead(21) )
    Serial.println("1");
  delay(1000);
}
