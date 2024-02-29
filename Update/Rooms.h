#ifndef Rooms_h
#define Rooms_h
#include "Arduino.h"

class Rooms
{

private:
    String room_name;
    String time;
    String time_alert;
    String room_message;

public:
    Rooms(String room_nameNewValue, String timeNewValue, String time_alertNewValue, String room_messageNewValue)
    {
        setRoom_name(room_nameNewValue);
        setTime(timeNewValue);
        setTime_alert(time_alertNewValue);
        setRoom_message(room_messageNewValue);
    }
    String getRoom_name()
    {
        return room_name;
    }
    String getTime()
    {
        return time;
    }
    String getTime_alert()
    {
        return time_alert;
    }
    String getRoom_message()
    {
        return room_message;
    }

    void setRoom_name(String room_nameNewValue)
    {
        room_name = room_nameNewValue;
    }
    void setTime(String timeNewValue)
    {
        time = timeNewValue;
    }
    void setTime_alert(String time_alertNewValue)
    {
        time_alert = time_alertNewValue;
    }
    void setRoom_message(String room_messageNewValue)
    {
        room_message = room_messageNewValue;
    }

    /* Rooms(String Room_name, String time, String time_alert, String Room_message, char letter)
    {
        this->Room_name = Room_name;
        this->time = time;
        this->time_alert = time_alert;
        this->Room_message = Room_message;
        this->letter = letter;
    }
    */
    void Active_sensor();
    void Detecion_alarm();
    void SMS_message(int Czas_m);
    void Time_read(char ledder, int Czas_m);
    void Update_command();
};

Rooms Kitchen("Kuchnia1", "", "", "");
Rooms Hall("Hall1", "", "", "");

#endif