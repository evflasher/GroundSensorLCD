#ifndef _DATETIME_CLASS
#define _DATETIME_CLASS
#include "Adafruit_ST7735.h" // Adafruit ST7735-Bibliothek
#include "RTClib.h"


class DateTimeClass
{
  RTC_DS1307 RTC; // Tiny RTC Modul
  Adafruit_ST7735* _tft;  

  //iarduino_RTC* _obTimer;
  unsigned long previosMillis;
  int updateInterval;
  boolean night_mode;
  
  public:
  char *currentTime;
  
  DateTime now;
  DateTime time_old;
  DateTime date_old;

  void init(Adafruit_ST7735* tft, int interval)
  {
    _tft = tft;

    RTC.begin();

    if (! RTC.isrunning()) {
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    updateInterval = interval;
  }

  void update(boolean _night_mode)
  {
    night_mode = _night_mode;
    unsigned long currentMillis = millis(); // текущее время в миллисекундах
    
    if(currentMillis - previosMillis >= updateInterval){ // если прошла 1 секунда
      previosMillis = currentMillis;  // запоминаем момент времени
      
      now = RTC.now();

      if(now.minute() != time_old.minute()){
        show_time(time_old,true);
        time_old = now;
        Serial.print(now.minute());
        Serial.print(" ");
        Serial.println(now.second());
        show_time(time_old,false);
      }
      if(now.day() != date_old.day()){
        show_date(date_old,true);
        date_old = now;
        Serial.print(now.day());
        Serial.print(" ");
        Serial.println(now.month());
        show_date(date_old,false);
      }
    }
  }
  
  String get_day_of_week(uint8_t dow){ 
    
    String dows="  ";
    switch(dow){
     case 0: dows="So"; break;
     case 1: dows="Mo"; break;
     case 2: dows="Tu"; break;
     case 3: dows="We"; break;
     case 4: dows="Th"; break;
     case 5: dows="Fr"; break;
     case 6: dows="Sa"; break;
    }
    
    return dows;
  }

  void show_time(DateTime now, boolean clear){
    
    int clearcolor = night_mode ? ST7735_BLACK : ST7735_WHITE;
    int textcolor = night_mode ? ST7735_WHITE : ST7735_BLACK;

    _tft->setTextColor(clear ? clearcolor : textcolor);
        
    _tft->setTextSize(3);
    _tft->setCursor(21,21);
    if(now.hour()<10)_tft->print(0);
    _tft->print(now.hour(),DEC);
    _tft->print(":");
    if(now.minute()<10)_tft->print(0);
    _tft->print(now.minute(),DEC);
  //    _tft->print(":");
  //    if(now.second()<10)_tft->print(0);
  //    _tft->print(now.second(),DEC);
  }

  void show_date(DateTime now,boolean clear){
  
    int clearcolor = night_mode ? ST7735_BLACK:ST7735_WHITE;
    int textcolor = night_mode ? ST7735_WHITE:ST7735_BLACK;

    _tft->setTextColor(clear ? clearcolor : textcolor);
    _tft->setTextSize(1);
    _tft->setCursor(24,47);  
    _tft->print(get_day_of_week(now.dayOfTheWeek()));
    _tft->print(", ");
    if(now.day()<10)_tft->print(0);
    _tft->print(now.day(),DEC);
    _tft->print(".");
    if(now.month()<10)_tft->print(0);
    _tft->print(now.month(),DEC);
    _tft->print(".");
    _tft->print(now.year(),DEC);
  }


};

#endif
