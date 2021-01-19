#include <Servo.h>
#include "SD.h"
#define SD_ChipSelectPin 53
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;


//color
#define enA 10
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
#define zelenaDioda 35
const int crveniButton = 27;  
const int plaviButton = 26; 
const int zutiButton = A0; 
const int crvenaDioda = 29; 


const int dioda1 = 32; 
const int dioda2 = 31; 
const int dioda3 = 33;
const int dioda4 = 34;  
const int svjetla = 36;  


Servo gornjiServo;
Servo donjiServo;

int frequency = 0;
int color = 0;
int crveniButtonState = 0;    
int plaviButtonState = 0; 
int zutiButtonState = 0; // variable for reading the pushbutton status

long int data;


 
//int LED = 13; // Led connected
long int password1 = 92;// ukljuci
long int password2 = 79; // iskljuci
long int password4 = 22; // play
long int password3 = 88; // stop
 

// motori
int motor1pin1 = 40;
int motor1pin2 = 41;

int motor2pin1 = 42;
int motor2pin2 = 43;

int motor3pin1 = 44;
int motor3pin2 = 45;

int motor4pin1 = 48;
int motor4pin2 = 47;

 //VRACARA
 int crveni_pin= 11;
int zeleni_pin = 12;
int plavi_pin = 13;
 
void setup(){
pinMode(enA, OUTPUT);

tmrpcm.speakerPin = 46;
Serial.begin(9600);
if(!SD.begin(SD_ChipSelectPin)){
  Serial.println("SD fail");
  return;
}
tmrpcm.setVolume(6);
   
//pinMode(LED, OUTPUT); 
//digitalWrite(LED, LOW);
Serial1.begin(9600);


// KOLOR
pinMode(crveniButton, INPUT);
pinMode(plaviButton, INPUT);
pinMode(zutiButton, INPUT);
pinMode(S0, OUTPUT);
pinMode(S1, OUTPUT);
pinMode(S2, OUTPUT);
pinMode(S3, OUTPUT);
pinMode(sensorOut, INPUT);

// Setting frequency-scaling to 20%
digitalWrite(S0, HIGH);
digitalWrite(S1, LOW);

gornjiServo.attach(7);
donjiServo.attach(8);

// motori
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(motor3pin1, OUTPUT);
  pinMode(motor3pin2, OUTPUT);
  pinMode(motor4pin1, OUTPUT);
  pinMode(motor4pin2, OUTPUT);

 // VRACARA
 pinMode(crveni_pin, OUTPUT);
 pinMode(zeleni_pin, OUTPUT);
 pinMode(plavi_pin, OUTPUT);

 // SHIFT


// DIODE
  pinMode(zelenaDioda, OUTPUT);
  pinMode(crvenaDioda, OUTPUT);
 pinMode(dioda1, OUTPUT);
 pinMode(dioda2, OUTPUT);
 pinMode(dioda3, OUTPUT);
 pinMode(dioda4, OUTPUT);
 pinMode(svjetla, OUTPUT);
 
}
 
void diodePali() {
    digitalWrite(dioda1, HIGH);
    digitalWrite(dioda2, HIGH);
     digitalWrite(dioda3, HIGH);
    digitalWrite(dioda4, HIGH);
    digitalWrite(svjetla, HIGH);
    

}


void diodeGasi() {
      digitalWrite(dioda1, LOW);
    digitalWrite(dioda2, LOW);
      digitalWrite(dioda3, LOW);
    digitalWrite(dioda4, LOW);
    digitalWrite(svjetla, LOW);

}

int readColor() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int R = frequency;
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(50);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int G = frequency;
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(50);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int B = frequency;
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(50);

  if(/*(R<60 & R>40 & G>50) ||*/ (G>100 & B>80) ){
    color = 1; // Crvena
  }
  if(R<80 & R>60 & G>90 &B>60){
    color = 2; // Roza
  }
  if(R> 95 &G>70){
    color = 3; // Plava
  }
  if(R>65 & R <90 & G>80 & G < 100 & B>80){
    color = 4; // Zuta
  }
  Serial.println(color);//printing name
  return color;  
}



void senzorZaBoje(int crveni, int plavi){
  donjiServo.write(40);
   crveniButtonState = crveni;
   plaviButtonState = plavi;
//Serial.print(crveniButtonState);
  gornjiServo.write(1115);
  delay(3000);
  
  for(int i = 115; i > 55; i--) {
    gornjiServo.write(i);
    delay(2);
  }
  delay(1000);
  
  color = readColor();
  color = readColor();
  color = readColor();
  color = readColor();
  
  delay(1000);  
  switch (color) {
    case 1: //crvena
    if (crveniButtonState == HIGH ){
      digitalWrite(zelenaDioda, HIGH);
      digitalWrite(crvenaDioda, LOW);
    }
    if (plaviButtonState == HIGH  ){
      digitalWrite(crvenaDioda, HIGH);
      digitalWrite(zelenaDioda, LOW);
    }
    donjiServo.write(40);
    break;
    case 2: //roza
    digitalWrite(crvenaDioda, HIGH);
    digitalWrite(zelenaDioda, LOW);
    donjiServo.write(75);
    break;
    case 3: // zuta
      digitalWrite(crvenaDioda, HIGH);
      digitalWrite(zelenaDioda, LOW);
    donjiServo.write(110);
    break;
    case 4: // plava
     if (plaviButtonState == HIGH){
    digitalWrite(zelenaDioda, HIGH);
    digitalWrite(crvenaDioda, LOW);
    }
    if (crveniButtonState == HIGH ){
    digitalWrite(crvenaDioda, HIGH);
    digitalWrite(zelenaDioda, LOW);
    }
    donjiServo.write(140);
    break;  
    case 0:
    digitalWrite(crvenaDioda, HIGH);
    digitalWrite(zelenaDioda, LOW);
    break;
  }
  delay(800);
  
  for(int i = 55; i > 20; i--) {
    gornjiServo.write(i);
    delay(2);
  } 
  delay(500);
  
  for(int i = 29; i < 115; i++) {
    gornjiServo.write(i);
    delay(2);
  }
  color=0;
  
}

void vrtiMotore(){
  //Controlling spin direction of motors:
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor3pin1, HIGH);
  digitalWrite(motor3pin2, LOW);
  
  digitalWrite(motor4pin1, HIGH);
  digitalWrite(motor4pin2, LOW);
}

void ugasiMotore(){
  //Controlling spin direction of motors:
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);
  
  digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, LOW);
}

void vracara(){
  RGB_color(255, 0, 0); // Red
  delay(500);
  RGB_color(0, 255, 0); // Green
  delay(500);
  RGB_color(0, 0, 255); // Blue
  delay(500);
  RGB_color(255, 255, 125); // Raspberry
  delay(500);
  RGB_color(0, 255, 255); // Cyan
  delay(500);
  RGB_color(255, 0, 255); // Magenta
  delay(500);
  RGB_color(255, 255, 0); // Yellow
  delay(500);
  RGB_color(255, 255, 255); // White
  delay(500);
}


void RGB_color(int crvena, int zelena, int plava) {
  analogWrite(crveni_pin, crvena);
  analogWrite(zeleni_pin, zelena);
  analogWrite(plavi_pin, plava);
}

void vracaraUgasi(){
  digitalWrite(crveni_pin, LOW);
  digitalWrite(zeleni_pin, LOW);
  digitalWrite(plavi_pin, LOW);
}

int zastavica = 0;
void loop(){


donjiServo.write(140);
/*
  //tmrpcm.play("test.wav");
  /*
   * 
   */
  //diodePali();
  //vracara();
  // 2 ferris 1 carousel 3 šolje 4 šolje do ulaznica
  //analogWrite(enA, 160); 
  //digitalWrite(motor4pin1, HIGH);
  //digitalWrite(motor4pin2, LOW);
    //digitalWrite(motor1pin1, HIGH);
  //digitalWrite(motor1pin2, LOW);
    //digitalWrite(motor2pin1, HIGH);
  //digitalWrite(motor2pin2, LOW);
    //digitalWrite(motor3pin1, LOW);
  //digitalWrite(motor3pin2, HIGH);
  //digitalWrite(zelenaDioda, HIGH);
  /*
     crveniButtonState = digitalRead(crveniButton);
   //zutiButtonState = digitalRead(zutiButton);
   plaviButtonState = digitalRead(plaviButton);
   //Serial.println(crveniButtonState);
   //Serial.println(plaviButtonState);
   //Serial.println(zutiButtonState);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (crveniButtonState == HIGH || zutiButtonState == HIGH || plaviButtonState == HIGH) {
    senzorZaBoje(crveniButtonState, zutiButtonState, plaviButtonState);
  }
  */Serial.println(crveniButtonState);
  
while(Serial1.available()==0) ;


if(Serial1.available()>0) {
data = Serial1.parseInt();
crveniButtonState = digitalRead(crveniButton);
plaviButtonState = digitalRead(plaviButton);
Serial.println(crveniButtonState);
//Serial.println(plaviButtonState);
if (crveniButtonState == HIGH || plaviButtonState == HIGH) {
    senzorZaBoje(crveniButtonState, plaviButtonState);
}
} 

delay(400);
//Serial.print(data);
 
if (data == 67){
  diodePali();
  zastavica = 0;
}
if (data == 76){
  diodeGasi();
  zastavica = 0;
}
if (data == 23){
  vracara();
  zastavica = 0;
}
if (data == 32){
  vracaraUgasi();
  zastavica = 0;
}
if (data == 56){
   analogWrite(enA, 160); 
  digitalWrite(motor4pin1, HIGH);
  digitalWrite(motor4pin2, LOW);
  zastavica = 0;
}
if (data == 65){
    digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, LOW);
  zastavica = 0;
}
if (data == 34){
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  zastavica = 0;
}
if (data == 43){
    digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  zastavica = 0;
}
if (data == 11){ 
  digitalWrite(motor3pin1, HIGH);
  digitalWrite(motor3pin2, LOW);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  zastavica = 0;
}
if (data == 21){
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);
    digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  zastavica = 0;
}
if (data == 91 & zastavica==0){ 
tmrpcm.play("test.wav");
zastavica = 1;
}
if (data == 19){
tmrpcm.pause();
zastavica = 0;
}
if (data == 89){ 
crveniButtonState = digitalRead(crveniButton);
plaviButtonState = digitalRead(plaviButton);
Serial.println(crveniButtonState);
//Serial.println(plaviButtonState);

if (crveniButtonState == HIGH || plaviButtonState == HIGH) {
    senzorZaBoje(crveniButtonState, plaviButtonState);
}
zastavica = 0;
}
if (data == 98){
 donjiServo.write(110);
 zastavica = 0;
}
if (data == 99){ 
   zastavica = 0;
crveniButtonState = digitalRead(crveniButton);
plaviButtonState = digitalRead(plaviButton);

if (crveniButtonState == HIGH || plaviButtonState == HIGH) {
    senzorZaBoje(crveniButtonState, plaviButtonState);
}
 diodePali();
   digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor3pin1, HIGH);
  digitalWrite(motor3pin2, LOW);
    digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
     analogWrite(enA, 160); 
  digitalWrite(motor4pin1, HIGH);
  digitalWrite(motor4pin2, LOW);
   vracara();
tmrpcm.play("test.wav");
zastavica = 1;
}
if (data == 88){
    diodeGasi();
 donjiServo.write(110);
    digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);
    digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
     analogWrite(enA, 160); 
  digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, LOW);
   vracaraUgasi();
 tmrpcm.pause();
 zastavica = 0;

}
  //vrtiMotore();
   //  crveniButtonState = digitalRead(crveniButton);
   //zutiButtonState = digitalRead(zutiButton);
   //plaviButtonState = digitalRead(plaviButton);
   //Serial.println(crveniButtonState);
   //Serial.println(zutiButtonState);
   //Serial.println(plaviButtonState);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  //if (crveniButtonState == HIGH || zutiButtonState == HIGH || plaviButtonState == HIGH) {
    //senzorZaBoje(crveniButtonState, zutiButtonState, plaviButtonState);
 // }
  //vracara();  
//if(Serial1.available()>0) {
/*data = Serial1.parseInt();
 if (data== password2){
  diodeGasi();
  ugasiMotore();
  vracaraUgasi();
  break;
 }
 if( data == password3){
tmrpcm.play("test.wav");
}
} 
}
   
if( data == password2){
    diodeGasi();
  ugasiMotore();
  vracaraUgasi();
}

if( data == password3){
tmrpcm.play("test.wav");
}

if( data == password4){
tmrpcm.pause();

}*/
 
}
