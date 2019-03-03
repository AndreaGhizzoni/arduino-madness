//Constants
int pinA = 0;  // Connected to CLK on KY-040
int pinB = 1;  // Connected to DT on KY-040
int encoderCount = 0;
int aLast;
int aVal;
int bVal;


void setup() {
  Serial.begin(9600);
  pinMode (pinA, INPUT);
  pinMode (pinB, INPUT);
  aLast = digitalRead(pinA);
}

void loop() {
  aVal = digitalRead(pinA);
  bVal = digitalRead(pinB);
  if ((aVal != aLast)&&(aVal==LOW)) { // Means the knob is rotating
    if(bVal == LOW){ encoderCount++;} // If bVal is Low, too, CW  ++
    else {encoderCount--;}            // else, CCW --
   
    Serial.print("PinA: "); Serial.print(aVal);   //Always 0 due to 'if'
    Serial.print(" PinB: "); Serial.print(bVal);  // Changes with Direction
    Serial.print(" Counter: ");Serial.println(encoderCount);
  }
  aLast = aVal;    //Don't forget to reset aLast

}
