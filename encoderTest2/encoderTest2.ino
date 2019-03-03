#define CLK A2
#define DT 3
#define SW A3

int counter = 0;
int state;
int Laststate; 

void setup() {
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  //pinMode(SW,INPUT);
  Serial.begin(9600);

  Laststate = digitalRead(CLK);
}

void loop() {
  state = digitalRead(CLK);
  if(state != Laststate) {
    if(digitalRead(DT) != state) {
      counter++;
    }else{
      counter--;
    }
    Serial.println(counter);
  }

  if(digitalRead(SW) != 1){
    counter=0;
    Serial.println(counter);
  }
    
  Laststate = state;
}
  
