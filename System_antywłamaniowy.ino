#include "Keypad.h"
#include <SoftwareSerial.h>


SoftwareSerial mySerial(3, 4);
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {8, 7, 6, 5};  //Piny, do których podłączamy wyprowadzenia od rzędów
byte colPins[COLS] = {12, 11, 10, 9}; //Piny, do których kolumn wyprowadzenia od rzędów

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int buzzer = 16;
const int LED = 17;
const int czujnik = 2;
String password="1234";
String tempPassword="";
unsigned long aktualnyCzas = 0;
unsigned long zapamietanyCzasLED1 = 0;

int a = 0;



int stanLED1 = LOW;
unsigned long Czas;
unsigned long Czas1;
unsigned long Czas2;

 

int stanAlarmu = 1;


void setup() {
  Serial.begin(9600);        //inicjalizacja monitora szeregowego
  pinMode(czujnik, INPUT_PULLUP);   //ustawienie pinu Arduino jako wejście
  pinMode(LED, OUTPUT);       // ustawienie diody LED
  pinMode(buzzer, OUTPUT);
  digitalWrite(czujnik, LOW);// ustawienie buzzera

  Serial.println("---- TEST ALARMU ----"); 
  
}
void loop() {
  char klawisz = 0; //zmienna do przetrzymywania znakow z klawiatury

 klawisz = keypad.getKey();
 
  switch(stanAlarmu) { //Wykonywania akcji odpowiedniej dla danego stanu
    case 1:
      //Uzbrajanie    
      
      klawisz = keypad.getKey();
      if (klawisz == '#') { //Czy zubroic alarm?
      Serial.println("Uzbrajanie"); 
      Czas1 = millis();
      stanAlarmu = 2;  
        }
          
          
   
      
        
    break;
    //Czuwanie
    case 2:
     
     
      if (millis()- Czas1 > 10000){
      a = 0;
      Serial.println("Czuwanie"); 
      digitalWrite(LED, HIGH);
      if (digitalRead(czujnik) == HIGH) {
       Czas = millis();
        stanAlarmu = 3; 
      } else  {
        digitalWrite(LED, HIGH);
      }
      }
      
    break;
    
    case 3:
      //Rozbrajanie
    Serial.println("Rozbrajanie"); 
    Rozbrajanie();
    Miganie(); 
    if (millis() - Czas > 10000){
      stanAlarmu = 4;
    }
    break;
    
    case 4:
    // Alarm
   Serial.println("AlARM");
   tone(buzzer, 1000);
    Miganie();
    Rozbrajanie();
    if (a == 0){
    Wysylanie_SMS();
    a = a +1 ;
    }
     break;
  }
  }

  void Rozbrajanie(){
    char klawisz = 0;
    
  Serial.println("Rozbrajanie"); 
   klawisz = keypad.getKey(); 
  

   int k=0;
  
    if (klawisz != NO_KEY){
      if (klawisz == '0' || klawisz == '1' || klawisz == '2' || klawisz == '3' ||
      klawisz == '4' || klawisz == '5' || klawisz == '6' || klawisz == '7' ||
      klawisz == '8' || klawisz == '9' || k==5 ) {
      tempPassword += klawisz;
      k+=1;
      }
      else if (k == 5 || klawisz == '#'){  
        tempPassword = "";
        k=0;
       }
      
      else if ( klawisz == '*') {
        if ( tempPassword == password ) {
       tempPassword = "";  
      noTone(buzzer);
      Czas1 = millis();
      digitalWrite(LED, LOW);
      stanAlarmu = 2;
         }
        else if ( tempPassword != password  ) {
        
          tempPassword = "";
          stanAlarmu = 4;
      
  
      }
    
     }
   
  }
  }
  void Miganie(){
     Rozbrajanie();
    aktualnyCzas  = millis();
     if (aktualnyCzas - zapamietanyCzasLED1 >= 200) {
    zapamietanyCzasLED1 = aktualnyCzas;
    stanLED1 = !stanLED1;
    digitalWrite(LED, stanLED1);
  }
  }
 void Wysylanie_SMS(){
 
  mySerial.begin(9600);
  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  Serial.println("NUMER"); 
  mySerial.println("AT+CMGS=\"+48506259022\""); 
  updateSerial();
  Serial.println("ALARM"); 
  mySerial.print("ALARM"); 
  updateSerial();
  mySerial.write(26);
  } 

  void updateSerial()
{
  
  delay(500);   
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
   
   
}
  
  


  
