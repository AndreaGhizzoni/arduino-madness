// http://www.brescianet.com/appunti/Elettronica/Arduino/corso/Esempio_ROTARYENCODER.htm
// ----------------------------------------------------------------
// Programma che utilizza un rotary encoder a quadratura
// ......::  M O D A L I T A'   P O L L I N G   ::......
// ----------------------------------------------------------------
#define encoderCLK A3 // CLK (canale A) connesso al pin D2
#define encoderDT A2  // DT (Canale B) connesso al pin D4
#define encoderSW A1  // SW (bottone) connesso al pin D3
int encoderCount = 0; // Contatore
int actCLKState;      // Lettura attuale del canale CLK (A)
int prevCLKState;     // Lettura precedente del canale CLK (A)

void setup() 
{ 
    // I pin sono impostati come ingresso
    pinMode(encoderCLK, INPUT);
    pinMode(encoderDT, INPUT);   
    // Setto la resistenza di pull-up - E' equivalente a:
    // pinMode(encoderSW, INPUT);   
    // digitalWrite(encoderSW, HIGH); 
    pinMode(encoderSW, INPUT_PULLUP);
    // inizializza la comunicazione seriale a 9600 bit per secondo:
    Serial.begin(9600);
    // Memorizzo l'ultimo stato del canale CLK
    prevCLKState = digitalRead(encoderCLK);
}

void loop()
{ 
    actCLKState = digitalRead(encoderCLK);
    // Se lo stato dello switch SW diventa LOW significa che ho premuto l'asta dell'encoder
    if (!digitalRead(encoderSW) && encoderCount!=0)
    {
        encoderCount = 0;
        Serial.println(encoderCount);
    }
    // Se cambia lo stato di CLK significa che il pomello e' stato ruotato
    if (actCLKState != prevCLKState)
    {
        /* ---------------------------------------------------------------
        * Per determinare il verso della rotazione occorre leggere DT (B) 
        * Se i 2 canali CLK e DT sono uguali la rotazione è oraria           
        * Se i 2 canali CLK e DT sono diversi la rotazione è antioraria
        *  
        *           Orario                              Antiorario
        * 1    ----      ----      --              ----      ----      --   
        *     |    |    |    |    |    CLK (A)    |    |    |    |    |     CLK (A)
        * 0 --      ----      ----              --      ----      ----    
        * 1 ----      ----      ----                 ----      ----      
        *       |    |    |    |    |  DT (B)       |    |    |    |    |   DT  (B)
        * 0      ----      ----                 ----      ----      ----  
        --------------------------------------------------------------- */
        encoderCount += (actCLKState == digitalRead(encoderDT) ? 1 : -1);
        Serial.println(encoderCount);
        prevCLKState = actCLKState;
  } 
} 
