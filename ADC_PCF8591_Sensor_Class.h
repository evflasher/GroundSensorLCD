#ifndef _ADC_SENSOR_CLASS
#define _ADC_SENSOR_CLASS

#include <Wire.h>
#include "PCF8591.h"


class ADCSensorClass
{
  private:

    PCF8591 pcf;
    PCF8591 pcf2;
    Adafruit_ST7735* _tft;  

    unsigned long previosMillis;
    int updateInterval = 10000;
    boolean night_mode;

    int sensorValue = 0;

  public:
    int sensorValue1;
    int sensorValue2;
    
  //ADCSensorClass(uint8_t pin, uint8_t type):_dht(pin, type){}

  void init(Adafruit_ST7735* tft)
  {
    _tft = tft;
  
    pcf.begin(0);
    pcf2.begin(1);

    sensorValue1 = pcf.analogRead (0);
    sensorValue2 = pcf2.analogRead (0);

    Serial.print (sensorValue1, DEC);
    Serial.print (" ");

    Serial.print (sensorValue2, DEC);
    Serial.print (" ");
    Serial.println ();
    //updateInterval = interval;
  }

  void update(boolean _night_mode, boolean force = false)
  {
    night_mode = _night_mode;
    unsigned long currentMillis = millis(); // текущее время в миллисекундах
    
    if(force || currentMillis - previosMillis >= updateInterval){ // если прошла 1 секунда
      previosMillis = currentMillis;  // запоминаем момент времени
  
      sensorValue1 = pcf.analogRead (0);
      sensorValue2 = pcf2.analogRead (0);

      // Serial.print (sensorValue1, DEC);
      // Serial.print (" ");

      // Serial.print (sensorValue2, DEC);
      // Serial.print (" ");
      // Serial.println ();
    }
  }
  
  void set_text_display(int x,int y,String text,int color,int size){
  
    _tft->setTextSize(size);
    _tft->setCursor(x,y);
    _tft->setTextColor(color);
    _tft->print(text);
  }


};

#endif
