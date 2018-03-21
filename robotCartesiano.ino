// ENTRADAS //
const int EMERG=18; // Seta de emergencia en el pin 18
const int CMARCHA=32; // Entrada contactor 
const int ALARMAX=22; // EJE X, variador o guardamotor pin 22
const int ALARMAY=27; // EJE Y, variador o guardamotor pin 27
volatile int FCX1=23; // Final carrera lado izquierdo
const int FCX2=24; // Final carrera lado derecho
const int FCY1=28; // Final carrera arriba
const int FCY2=29; // Final carrera abajo
const int JKIZQ=25; // Botón izquierda joystick
const int JKDER=26;
const int JKARR=30;
const int JKABA=31;
volatile int INTFINC=19; // Flanco de bajada al activarse un FC
const int ENX=2;
const int ENY=3;

// SALIDAS //
volatile int MARCHA=42; // Salida contactor
volatile int FRENO=43; // 
volatile int PAX=44;
volatile int PAY=45;
volatile int STFX=46; // Señal de avance derecha eje X
volatile int STRX=47;// Señal de retroceso izquierda eje X
volatile int STFY=48;
volatile int STRY=49;
volatile int RLX=37;
volatile int RMX=36;
volatile int RHX=35;
volatile int RLY=41;
volatile int RMY=40;
volatile int RHY=39;
void emergencia();
void finalCarrera();
void alarmaX();
void alarmaY();


// VARIABLES
long timeCounter=0;
const int timeThreshold=400;
int contadorx=0;
int contadory=0;

void contadorX();
void contadorY();

void setup() {
  Serial.begin(9600);
    
  pinMode(EMERG, INPUT);
  pinMode(ALARMAX,INPUT);
  pinMode(ALARMAY,INPUT);
  pinMode(FCX1,INPUT);
  pinMode(FCX2,INPUT);
  pinMode(FCY1,INPUT);
  pinMode(FCY2,INPUT);
  pinMode(JKIZQ,INPUT);
  pinMode(JKDER,INPUT);
  pinMode(JKARR,INPUT);
  pinMode(JKABA,INPUT);
  pinMode(INTFINC,INPUT);
  pinMode(ENX,INPUT);
  pinMode(ENY,INPUT);

  pinMode(MARCHA,OUTPUT);
  pinMode(FRENO,OUTPUT);
  pinMode(PAX,OUTPUT);
  pinMode(PAY,OUTPUT);
  pinMode(STFX,OUTPUT);
  pinMode(STRX,OUTPUT);
  pinMode(STFY,OUTPUT);
  pinMode(STRY,OUTPUT);
  pinMode(RLX,OUTPUT);
  pinMode(RMX,OUTPUT);
  pinMode(RHX,OUTPUT);
  pinMode(RLY,OUTPUT);
  pinMode(RMY,OUTPUT);
  pinMode(RHY,OUTPUT);

  digitalWrite(MARCHA, HIGH);
  digitalWrite(FRENO,LOW);
  digitalWrite(EMERG, HIGH);
  digitalWrite(PAX,LOW); // Se enciende si hay alarma en el variador X o en el guardamotor X.
  digitalWrite(PAY,LOW);
  digitalWrite(STFX,LOW);
  digitalWrite(STRX,LOW);
  digitalWrite(STFY,LOW);
  digitalWrite(STRY,LOW);
  digitalWrite(RLX, LOW);
  digitalWrite(RMX, LOW);
  digitalWrite(RHX, LOW);
  digitalWrite(RLY, LOW);
  digitalWrite(RMY, LOW);
  digitalWrite(RHY, LOW);


  //Contadores encoder
  attachInterrupt(digitalPinToInterrupt(ENX), contadorX, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENY), contadorY, FALLING);
 
  //Interrupción de la Seta de Emergencia
  attachInterrupt(digitalPinToInterrupt(EMERG), emergencia, LOW);

  // Interrupción de FC
  attachInterrupt(digitalPinToInterrupt(INTFINC), finalCarrera, FALLING);
}

void loop() {

  // Seta de emergencia
  if(digitalRead(EMERG)==HIGH){
    digitalWrite(MARCHA,HIGH);
    digitalWrite(FRENO,LOW);
  } 

  //Alarmas
  if(digitalRead(ALARMAX)==true){
     alarmaX();
  }
  else{
    digitalWrite(PAX,LOW);
  }
  
  if(digitalRead(ALARMAY)==true){
     alarmaY();
  }
  else{
    digitalWrite(PAY,LOW);
  }
  
  // FINAL DE CARRERA
  if(digitalRead(FCX1)==HIGH){
    contadorx=0;
  }
  if(digitalRead(FCY1)==HIGH){
    contadory=0;
  }

  // Avance sin llegar al FC

  if(digitalRead(FCX1)==HIGH){
    digitalWrite(STRX,LOW);
  }else if(digitalRead(JKIZQ)==HIGH){
    digitalWrite(STRX,HIGH);
    digitalWrite(RLX,HIGH);
  }
   
  if(digitalRead(FCX2)==HIGH){
    digitalWrite(STFX,LOW);
  }else if(digitalRead(JKDER)==HIGH){
    digitalWrite(STFX,HIGH);
    digitalWrite(RLX,HIGH);
  }
  
  if(digitalRead(FCY1)==HIGH){
    digitalWrite(STRY,LOW);
  }else if(digitalRead(JKARR)==HIGH){
    digitalWrite(FRENO,HIGH);
    digitalWrite(STRY,HIGH);
    digitalWrite(RLY,HIGH);
  }
  
  
  if(digitalRead(FCY2)==HIGH){
    digitalWrite(STFY,LOW);
  }else if(digitalRead(JKABA)==HIGH){
    digitalWrite(FRENO, HIGH);
    digitalWrite(STFY,HIGH);
    digitalWrite(RLY,HIGH);
  }

  if(digitalRead(JKABA)==LOW && digitalRead(JKARR)==LOW && digitalRead(JKIZQ)==LOW && digitalRead(JKDER)==LOW){
    digitalWrite(FRENO,LOW);
    digitalWrite(STRY,LOW);
    digitalWrite(STRX,LOW);
    digitalWrite(STFY,LOW);
    digitalWrite(STFX,LOW);
    digitalWrite(RLY,LOW);
    digitalWrite(RLX,LOW);
  }
  /*
  if(digitalRead(JKARR)==HIGH){
    Serial.println("BAJA");
  }
  if(digitalRead(JKABA)==HIGH){
    Serial.println("SUBE");
  }
  */

  delay(50);

}

void alarmaX(){
  digitalWrite(PAX,HIGH);
}

void alarmaY(){
  digitalWrite(PAY,HIGH);
}

void emergencia(){
  
  digitalWrite(MARCHA, LOW); // Con el botón de emergencia se pone a LOW la salida MARCHA
  digitalWrite(FRENO,LOW);
  digitalWrite(RLY,LOW);
  digitalWrite(RLX,LOW);
  if(millis() > timeCounter + timeThreshold){
    timeCounter=millis();
  }
  
}

void finalCarrera(){
  // ACTIVAMOS EL FRENO
  digitalWrite(FRENO,LOW);
  // COMPROBACIONES
  
  if(digitalRead(FCX1)==HIGH && digitalRead(STFX)==LOW){
    digitalWrite(STRX,LOW);
    digitalWrite(RMX,LOW);
  }
  
  if(digitalRead(FCX2)==HIGH && digitalRead(STRX)==LOW){
    digitalWrite(STFX,LOW);
    digitalWrite(RMX,LOW);
    Serial.println("final carrera");
  }
  if(digitalRead(FCY1)==HIGH){
    digitalWrite(STRY,LOW);
    digitalWrite(RMY,LOW);
  }
  if(digitalRead(FCY2)==HIGH){
    digitalWrite(STFY,LOW);
    digitalWrite(RMY,LOW);
    contadory=0;
  } 
  // ELIMINACION DE RUIDO
  if(millis() > timeCounter + timeThreshold){
    timeCounter=millis();
  }
  
}

void contadorX(){
  if(digitalRead(FCX1)==HIGH){
    contadorx=contadorx+1;
  } else {
    contadorx=contadorx-1; 
  }
  Serial.println(contadorx);
}

void contadorY(){
 if(digitalRead(JKARR)==HIGH){
  contadory=contadory+1;
 } else {
  contadory=contadory-1;
 }
 Serial.println(contadory);
}


