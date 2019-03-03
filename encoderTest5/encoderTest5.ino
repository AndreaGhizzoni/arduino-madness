// ----------------------------------------------------------------
//   Programma che utilizza un rotary encoder a quadratura
// ......::  M O D A L I T A'   I N T E R R U P T   ::......
// ----------------------------------------------------------------
#define encoderCLK 2 // CLK (canale A) connesso al pin D2
#define encoderDT 4  // DT (Canale B) connesso al pin D4
#define encoderSW 3  // SW (bottone) connesso al pin D3
#define interrupt0 0 // Associato al pin 2 di Arduino
#define interrupt1 1 // Associato al pin 3 di Arduino
int encoderCount = 0; // Contatore
int actCLKState;      // Lettura attuale del canale CLK (A)
int prevCLKState;     // Lettura precedente del canale CLK (A)
 
void setup()
{
    // I pin sono impostati come ingresso
    pinMode(encoderCLK, INPUT);    // interrupt 0
    pinMode(encoderDT, INPUT);   
    // Setto la resistenza di pull-up - E' equivalente a:
    // pinMode(encoderSW, INPUT);  
    // digitalWrite(encoderSW, HIGH); 
    pinMode(encoderSW, INPUT_PULLUP); // interrupt 1
    // Attivo l'interrupt 0 quando lo stato del canale CLK "cambia" poiche'
    // questo devo effettuare la lettura di DT ad ogni variazione di CLK
    attachInterrupt(interrupt0, CLKChanged, CHANGE);
    // Imposto l'interrupt 1 sul fronte di discesa poichè quando premo il pomello dell'encoder 
    // il segnale (canale SW) da alto diventa basso
    // A) Se metto RISING la routine di interrupt viene eseguita quando rilascio il pomello;
    //    Se provo a ruotare premendo contemporaneamente il pomello osservo che il contatore 
    //    visualizzato sul Serial Monitor non si azzera fino a quando non rilascio il pomello
    // B) Se mettessi CHANGE eseguo la routine di interrupt sia quando premo, sia quando rilascio
    //    il pomello. Se imposto l'interrupt su CHANGE e provo a premere il pomello 
    //    osservo che il contatore si azzera! Se continuo la rotazione, mantenendo premuto
    //    il pomello dell'albero, noto che il contatore si riazzera solo quando rilascio il pomello
    attachInterrupt(interrupt1, SWPressed, FALLING);
    // inizializza la comunicazione seriale a 9600 bit per secondo:
    Serial.begin(9600);
    // Memorizzo l'ultimo stato del canale CLK
    prevCLKState = digitalRead(encoderCLK);
}
 
void loop() {}

// ---------------------------------------------------------------------------
// Routine relativa alla rotazione dell'albero associata all'interrupt 0
// ---------------------------------------------------------------------------
void CLKChanged()
{
    int actCLKState = digitalRead(encoderCLK);// Leggo il canale A (CLK)
    // Questo if serve per gestire chiamate multiple alla routine di interrupt 
    // causate dal cosiddetto bouncing: ogni volta che si ruota l'albero vengono
    // in realtà generate diverse variazioni (per ognuna viene scatenato
    // l'interrupt!), dovute al funzionamento meccanico del rotore. Si possono
    // determinare effetti indesiderati come ad esempio la ripetizione di numeri
    // ma con questo IF vengono evitati.
    if (prevCLKState != actCLKState)
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

// ---------------------------------------------------------------------------
// Routine relativa alla pressione sull'albero associata all'interrupt 1
// ---------------------------------------------------------------------------
void SWPressed()
{
    // Questo if serve per gestire chiamate multiple alla routine di interrupt 
    // causate dal cosiddetto bouncing: ogni volta che si preme un pulsante reale
    // vengono in realtà generate diverse pressioni e rilasci (per ognuno dei quali
    // viene scatenato un interrupt!), dovute al funzionamento meccanico del pulsante. 
    // Lo stesso accade anche quando lo si rilascia. 
    if (encoderCount != 0) 
    {
        encoderCount = 0;
        Serial.println(encoderCount);
    }
}
