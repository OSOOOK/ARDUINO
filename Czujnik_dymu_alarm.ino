#include <SoftwareSerial.h>

SoftwareSerial SIM800L(3,4); //Inicjalizacja wejść RX TX w SIM800L

const int buzzer = 16; // Inicjalizacja buzzera
const int czujnik = 6; // Inizjalizacja czujnika dymu
int stanAlarmu = 1; // wartość początkowo dla głównej funkcji sterującej urządzeniem
uint32_t Czas1 = 0; // zmienna pomocnicza do sterownia opóżnieniem
String Time = ""; // zmienna pomocnicza wykorzystywana w odczycie czasu 
String Czas_wydarzenia = ""; // Zmienna zawierająca czas wystąpienia pożaru
String pomieszczenie = ""; // miejsce wystąpienia pożaru
char litera = "";


// Funkcja odpowiadajaca za aktualizacje wysyłanych danych do modułu SIM800L
void Akualizacjia_komend(){
  delay(500);
  while(Serial.available())
  { 
    SIM800L.write(Serial.read());
  }
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }
}

// Funkcja odpowiadająca za odczytanie czasu rzeczywstego
void odczytanie_czasu(){
  while(1){
    delay(1); 
    litera = (char) SIM800L.read();
    Time += litera;
    if ( millis() - Czas1 > 1000 || Time.indexOf("OK") >= 0 ) {
    break;
    }
  }
}

// Funkcja odpowiadająca za wysłanie powiadomienia SMS z odczyname aktaulnego czasu
void Powiadomienie_SMS(){
  Czas1 = millis();
  SIM800L.println("AT");
  Akualizacjia_komend();
  SIM800L.println("AT+CCLK?\r");
  delay(10);
  odczytanie_czasu();
  Time.remove(5, 9);
  Time.replace("AT+CCCLK:", "");
  Time.replace("OK", "");\
  Time.replace("+04", "");
  Serial.println(Time); 
  Czas_wydarzenia = String(pomieszczenie + Time); 
  Akualizacjia_komend();
  SIM800L.println("AT+CMGF=1\r");  
  Akualizacjia_komend();  
  SIM800L.println("AT+CMGS=\"+48***\"\r");  
  Akualizacjia_komend();
  SIM800L.println(Czas_wydarzenia);       
  Akualizacjia_komend();
  SIM800L.write(26);
  Akualizacjia_komend();
  Serial.println("Wiadomosc wyslana");
 }



// Funckja główna inicjalizująca wyjścia i wejścia
void setup() {
  Serial.begin(9600);
  SIM800L.begin(9600);
  Serial.println("Initializing...");
  delay(1000);
  pinMode(czujnik, INPUT_PULLUP);
  digitalWrite(czujnik, HIGH);
  pinMode(buzzer, OUTPUT);
  Serial.println("---- TEST ALARMU ----");
}

// Głowna funkcja sterująca układem
void loop() {
switch(stanAlarmu)
{
  // Aktywacja systemu przeciwpozarowego
  case 1:;
    Serial.println("---- Aktywacja czujnika ----");
    delay(10000); // Odczekanie 10 sek. na konfiguracje modułu SIM800L
    SIM800L.println("AT+CMGF=1\r");
    Akualizacjia_komend();
    SIM800L.println("AT&W");
    Akualizacjia_komend();
    pomieszczenie = "Czas aktywacji czujnika w pokoju nr. 1: ";
    Time = ""; 
    Powiadomienie_SMS();
    Czas1 = 0;
    litera = "";
    stanAlarmu = 2;
  
  break;

  // Wykrycie zagrożenia
  case 2:
    Serial.begin(9600);
    SIM800L.begin(9600);
    Czas_wydarzenia = "";
    Time = "";
  // Wykrycie zagrozenia
    delay (10000);
   if(digitalRead(czujnik) == LOW)
     {
    Serial.println("Alarm aktywny");
    pomieszczenie = "UWAGA!!!, Pozar w pokoju nr.1: ";
    litera = "";
    Powiadomienie_SMS();
    stanAlarmu = 3;
  }

  break;

  // Uruchomienie buzzera
  case 3: // Uaktywnienie buzzerak
  tone(buzzer,2000);
  }
}

  

  
