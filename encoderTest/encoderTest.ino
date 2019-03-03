#define CLK 2
#define DT 4
#define SW 3

int clk = 0;
int dt = 0;
int sw = 0;
 
void setup() {
   pinMode( CLK, INPUT );
   pinMode( DT, INPUT );
   pinMode( SW, INPUT_PULLUP );

   Serial.begin (9600);
}

void loop() {
   debug();
   delay(1000);
}

void debug(){
   clk = analogRead( CLK );
   dt = analogRead( DT );
   sw = digitalRead( SW );
   Serial.print("CLK: ");
   Serial.println(clk);
   Serial.print("DT: ");
   Serial.println(dt);
   Serial.print("SW: ");
   Serial.println(sw);
}

   
   // Reads the initial state of the outputA
   //aLastState = analogRead( outputA );
   /*
   aState = analogRead( outputA ); // Reads the "current" state of the outputA
   Serial.print("outA: ");
   Serial.println(aState);
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if( aState != aLastState + 20 || aState != aLastState -20 ){
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if( analogRead(outputB) != aState ) { 
        counter ++;
      } else {
        counter --;
      }
      Serial.print("Position: ");
      Serial.println(counter);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
   */
