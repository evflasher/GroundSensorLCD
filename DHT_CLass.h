#ifndef _DHT_CLASS
#define _DHT_CLASS

#include "Adafruit_ST7735.h" // Adafruit ST7735-Bibliothek
#include <DHT.h>

extern int activeScreen;

class TemperatureClass
{
  private:

    DHT _dht; 
    Adafruit_ST7735* _tft;  

    unsigned long previosMillis;
    int updateInterval = 10000;
    boolean night_mode;

    //float temp =          1000;
    //float hum  =          1000;
    int min_temp =      1000;
    int max_temp =     -1000;
    int min_humidity =  1000;
    int max_humidity = -1000;


  public:
    //char *currentTime;
    int temp = 0;
    int hum = 0;

  TemperatureClass(uint8_t pin, uint8_t type):_dht(pin, type){}

  void init(Adafruit_ST7735* tft)
  {
    _tft = tft;
  
    
    //DHT dht(DHT_PIN, DHT_TYPE); 
    //_dht = dht;

    _dht.begin();
    //updateInterval = interval;
  }

  void update(boolean _night_mode, boolean force = false)
  {
    night_mode = _night_mode;
    unsigned long currentMillis = millis(); // текущее время в миллисекундах
    
    if(force || currentMillis - previosMillis >= updateInterval){ // если прошла 1 секунда
      previosMillis = currentMillis;  // запоминаем момент времени
  
      int t;

      t = _dht.readTemperature();
      if(isnan(t)){}
      else if((int)t!=(int)temp){
       show_temp(temp,true);
       temp = t;
       if(min_temp>temp)min_temp=temp;
       if(max_temp<temp)max_temp=temp;
       show_temp(temp,false);
      }

      
      int h = _dht.readHumidity();
      if(isnan(h)){}
      else if(h!=hum){
       show_hum(hum,true);
       hum = h;
       if(min_humidity>hum)min_humidity=hum;
       if(max_humidity<hum)max_humidity=hum;
       show_hum(hum,false);
      }

    }
  }
  
  void show_temp(float temp, boolean clear){
  
    if(activeScreen != 1)
      return;

    int clearcolor = night_mode?ST7735_BLACK:ST7735_WHITE;
    int textcolor = night_mode?ST7735_WHITE:ST7735_BLACK;
    
    byte xs=12;
    byte ys=66;
    String htemp=String((int)temp);
    //byte xss=(temp<10?:temp
    
    set_text_display(xs,ys,htemp,clear?clearcolor:textcolor,3);
    set_text_display(xs+(3*htemp.length()*8),ys,"",(clear?clearcolor:textcolor),2);
    _tft->write(247); // das °-Zeichen  
    _tft->print("C");
    set_text_display(xs+81,ys+1,String((int)max_temp),clear?clearcolor:ST7735_RED,1);
    _tft->write(247);_tft->print("C");
    set_text_display(xs+81,ys+11,String((int)min_temp),clear?clearcolor:ST7735_BLUE,1);
    _tft->write(247);_tft->print("C");
  }

  void show_hum(float hum, boolean clear){
    if(activeScreen != 1)
      return;

    int clearcolor = night_mode?ST7735_BLACK:ST7735_WHITE;
    int textcolor = night_mode?ST7735_WHITE:ST7735_BLACK;

    byte xs=12;
    byte ys=96;
    
    set_text_display(xs,ys,String((int)hum)+"%",clear?clearcolor:textcolor,3);
    set_text_display(xs+81,ys+1,String((int)max_humidity)+"%",clear?clearcolor:ST7735_GREEN,1);
    set_text_display(xs+81,ys+11,String((int)min_humidity)+"%",clear?clearcolor:ST7735_YELLOW,1);
  }

  void set_text_display(int x,int y,String text,int color,int size){
  
    _tft->setTextSize(size);
    _tft->setCursor(x,y);
    _tft->setTextColor(color);
    _tft->print(text);
  }


};

#endif
