

int val = 0;       // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600);
}

void loop() {
  int currentVal = analogRead(A0);    // read the value from the sensor
  if( currentVal != val ){
    val = map(currentVal, 0, 1023, 0, 255);
    Serial.println(val);
  }
  
  //delay(1000);                  // stop the program for some time
}
