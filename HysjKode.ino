  
  //DEKLARERER ALLE KOMPONENTER SOM VARIABLER TIL PIN

  //LYS
  const int lysStart = 12;
  const int lysStop = 13;
  const int lysG = 11;
  const int lysO = 10;
  const int lysR = 9;

  //VIBRAJONSMOTOROER
  const int motor1 = 6;
  const int motor2 = 2;
  const int motor3 = 7;

  //LYDSENSOR
  const int lydSensor = 5;

  // KNAPPER
  const int startStoppKnapp = 3;
  int startStoppKnappStatus = 0;

  const int timeglassKnapp = 4;
  int timeglassKnappStatus = 0;


  // DEKLARERER OG INITIALISERER VARIABLER


  // Array med motorer
  int motorer[] = {motor1, motor2, motor3};

  // Array med lysdioder
  int lysDioder[] = {lysStart, lysStop, lysG, lysO, lysR};

  //(millis) Variabler
  unsigned long timeglass = 30000; //Timeglass start på 30 sekunder
  unsigned long startTidspunkt = 0;
  unsigned long diff = 0;
  unsigned long lydLaget = 0;

  //Blinking
  unsigned long forrigeBlink = 0;
  const long blinkInterval = 500; // 0.5 sekunder mellom blink
  int blinkStatus = LOW;

  //Boolske verdier
  boolean lydSensorVerdi = 0;
  boolean erIgang = false;



  void setup() {
  
  //SETTER PIN-MODE
    //lys
    pinMode(lysStop, OUTPUT);
    pinMode(lysStart, OUTPUT);
    pinMode(lysG, OUTPUT);
    pinMode(lysO, OUTPUT);
    pinMode(lysR, OUTPUT);

    //Vibrasjonsmotorer
    pinMode(motor1, OUTPUT);
    pinMode(motor2, OUTPUT);
    pinMode(motor3, OUTPUT);

    //Lydsensor
    pinMode(lydSensor, INPUT);

    //Knapper
    pinMode(startStoppKnapp, INPUT_PULLUP);
    pinMode(timeglassKnapp, INPUT_PULLUP);
  }




  // --------------- HJELPEMETODER ---------------

  void startSpill(){
      //oppdaterer start/stop lys (PÅ)
      digitalWrite(lysStart, HIGH);
      digitalWrite(lysStop, LOW);   
  }

  void stoppSpill(){
      //oppdaterer start/stop lys (AV)
      digitalWrite(lysStart, LOW);
      digitalWrite(lysStop, HIGH); 
  }


  /*
    Lyd laget
    Metoden lager lys og vibrasjonsmønster for å 
    indikere at lydsensor oppfattet en lyd
  */
  void lyd() {
    for (int i = 0; i < 8; i++) {
      //Setter alle motorer HIGH
      for (int motor : motorer) {
        digitalWrite(motor, HIGH);
      }

      //Setter alle lys HIGH
      for (int lys : lysDioder) {
        digitalWrite(lys, HIGH);
      }
      delay(100);

      //Setter alle lys LOW
      for (int lys : lysDioder) {
          digitalWrite(lys, LOW);
        }
        delay(100);
    }

      //Setter alle motorer LOW
    for (int motor : motorer) {
        digitalWrite(motor, LOW);
      }
  }


  /*
    Runden ferdig
    Metoden lager lysmønster for å 
    indikere at runden er ferdig (timeglass = 0)
  */
  void rundeFullfoert() {
      int teller = 0;

      for (int lys : lysDioder) {
        digitalWrite(lys, LOW);
      }

      while (teller < 8) {
        for (int lys: lysDioder) {
          digitalWrite(lys, HIGH);
          
          delay(100);

          digitalWrite(lys, LOW);
      }
      teller ++;
    }
  }


  /*
  Sjekker tid på timeglass og setter riktig lys
  */
void oppdaterTimeglass() {

    // 0 < sekunder igjen < 10 (RØD Blinking)
    //Unngår bruk av delay for å holde LydSensor aktiv
    if (timeglass <= 10000 && timeglass > 0){
      if(millis() - forrigeBlink >= blinkInterval){

        forrigeBlink = millis();

        //Bytter om på LOW/HIGH 
        if(blinkStatus == LOW){
            blinkStatus = HIGH;

        } else{ 
          blinkStatus = LOW;
        }

        digitalWrite(lysR, blinkStatus);
      }
    }

    // 10 < sekunder igjen < 30
    else if (timeglass <= 30000 && timeglass > 10000) {
      digitalWrite(lysG, LOW);
      digitalWrite(lysO, LOW);
      digitalWrite(lysR, HIGH);
    }


    // 30 < sekunder igjen < 35 (ORANGE Blinking)
    else if (timeglass <= 35000){
      if(millis() - forrigeBlink >= blinkInterval){
        forrigeBlink = millis();

        if (blinkStatus == LOW){
            blinkStatus = HIGH;

        } else{
          blinkStatus = LOW;
        }
        digitalWrite(lysO, blinkStatus);
      }
    }

    
    // 35 < sekunder igjen < 60
    else if (timeglass <= 60000) {
      digitalWrite(lysG, LOW);
      digitalWrite(lysO, HIGH);
      digitalWrite(lysR, HIGH);
    }


    // 60 < sekunder igjen < 65 (GRØNN Blinking)
    else if (timeglass <= 65000){
      if(millis() - forrigeBlink >= blinkInterval){
        forrigeBlink = millis();

        if(blinkStatus == LOW){
            blinkStatus = HIGH;
        } else {
          blinkStatus = LOW;
        }
        digitalWrite(lysG, blinkStatus);
      }
    }


    //  65 < sekunder igjen < 90 
    else {
      digitalWrite(lysG, HIGH);
      digitalWrite(lysO, HIGH);
      digitalWrite(lysR, HIGH);
    }
}




  void loop() {

    //Lagrer verdi fra lydlydSensor
    lydSensorVerdi = digitalRead(lydSensor);

    //Lagrer verdi fra startStoppKnapp 
    startStoppKnappStatus = digitalRead(startStoppKnapp);

    //Lagrer verdi fra timeglassKnapp 
    timeglassKnappStatus = digitalRead(timeglassKnapp);



    // ------------ TIMEGLASS ---------------
        //Oppdaterer timeglass i starten av hver loop
        oppdaterTimeglass(); 



    //------------- START/STOP ---------------
    if (startStoppKnappStatus == LOW) {
        startTidspunkt = millis(); //Lagrer startTidspunkt
        lydLaget = millis();       //LydLaget settes til millis
        delay(50); //Debunce 

        erIgang = !erIgang;       //oppdaterer boolsk verdi

            if(!erIgang){       //Dersom runden avsluttes resettes timeglass
            timeglass = 30000;
            oppdaterTimeglass();
          }
        }


    // ---------------- SPILL I GANG ---------------------------
    if (erIgang) {
        //Lagrer tid gått fra forrige måling i diff
        //Oppdaterer differanse fra sist måling
        //Teller ned timeglass ved å trekke fra diff
        diff = millis() - startTidspunkt;
        startTidspunkt += diff;
        timeglass -= diff; 

        //oppdaterer lys
        digitalWrite(lysStart, HIGH);
        digitalWrite(lysStop, LOW); 
      }



       //------------ SPILL IKKE I GANG -----------------
    if (!erIgang) {
        //oppdaterer lys
        digitalWrite(lysStart, LOW);
        digitalWrite(lysStop, HIGH); 




      //----------- OPPDATERER TIMEGLASS -------------
      // Timeglass kan kun endres når !erIgang
      if (timeglassKnappStatus == LOW) {
          delay(50); //Debounce

          if (timeglass != 90000) {
            timeglass += 30000;

          }else{
           timeglass = 30000;
          }
      }
    }



    // ------------- LYDSENSOR ---------------
      /*
      //lydSensor == inaktiv 3 sekund etter lyd og startTidspunkt
      (unngå loop mellom virasjonsmotor og lydSensor/ evt utslag på lyder fra knappetrykking)
      */
      if (millis() - lydLaget > 3000) {
        if (lydSensorVerdi == HIGH && erIgang) {
          lydLaget = millis(); //Lagrer tiden
          delay(200);
          lyd();
          erIgang= !erIgang;
          timeglass = 30000; 
        }
      }


    //------ RUNDE FULLFØRT ----------
       // TID UTE 
      if (timeglass <= 0) {
        rundeFullfoert();
        erIgang = false;
        timeglass = 30000; 
      }
      
}


  


    







    




