#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Rooms.h>
#include <Array.h>

SoftwareSerial SIM800L(3, 4); // Inicjalizacja wejść RX TX w SIM800L

const int buzzer = 16; // Inicjalizacja buzzera
const int sensor = 6;  // Inizjalizacja czujnika dymu    // zmienna pomocnicza do sterownia opóżnieniem
int AlarmState = 1;

void setup()
{
  Serial.begin(9600);
  SIM800L.begin(9600);
  Serial.println("Initializing...");
  delay(1000);
  pinMode(sensor, INPUT_PULLUP);
  digitalWrite(sensor, HIGH);
  pinMode(buzzer, OUTPUT);
  Serial.println("---- TEST ALARMU ----");
};

// Głowna funkcja sterująca układem
void loop()
{
  switch (AlarmState)
  {
  case 1:

    Kitchen.Active_sensor();
    break;

  case 2:
    Kitchen.Detecion_alarm();
    break;
  }
};

void Rooms::Update_command()
{
  delay(500);
  while (Serial.available())
  {
    SIM800L.write(Serial.read());
  }
  while (SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }
};

void Rooms::Time_read(char letter = ' ', int Czas_m = 0)
{

  while (1)
  {
    delay(1);
    letter == (char)SIM800L.read();
    Kitchen.getTime() += letter;
    if (millis() - Czas_m > 1000 || time.indexOf("OK") >= 0)
    {
      break;
    }
  }
};

void Rooms::SMS_message(int Czas_m = 0)
{

  Czas_m = millis();
  SIM800L.println("AT");
  Update_command();
  SIM800L.println("AT+CCLK?\r");
  delay(10);
  Time_read();
  time.remove(5, 9);
  time.replace("AT+CCCLK:", "");
  time.replace("OK", "");
  time.replace("+04", "");
  Serial.println(time);
  setTime_alert((Kitchen.getRoom_message() + Kitchen.getRoom_name() + Kitchen.getTime()));
  Update_command();
  SIM800L.println("AT+CMGF=1\r");
  Update_command();
  SIM800L.println("AT+CMGS=\"+48507289182\"\r");
  Update_command();
  SIM800L.println(time_alert);
  Update_command();
  SIM800L.write(26);
  Update_command();
  Serial.println("Wiadomosc wyslana");
}

void Rooms::Active_sensor()
{
  Serial.println("---- Aktywacja czujnika ----");
  delay(10000); // Odczekanie 10 sek. na konfiguracje modułu SIM800L
  SIM800L.println("AT+CMGF=1\r");
  Update_command();
  SIM800L.println("AT&W");
  Update_command();
  Kitchen.setRoom_message("Czas aktywacji czujnika w pokoju: " + Kitchen.getRoom_name());
  Update_command();
  SMS_message();
  AlarmState = 2;
};

void Rooms::Detecion_alarm()
{
  if (digitalRead(sensor) == LOW)
  {
    setTime_alert("");
    Kitchen.setRoom_name("");
    Serial.println("Alarm aktywny");
    Kitchen.setRoom_message("UWAGA!!!, Pozar w pokoju" + Kitchen.getRoom_name());
    SMS_message();
    tone(buzzer, 2000);
  }
};
