// BMP085, DHT11, TinyRTC (DS1307) und 1.8 Zoll TFT-Farb-Display (HY-1.8 SPI)
//https://forum.arduino.cc/index.php?topic=165170.0
//http://arduino.ru/forum/apparatnye-voprosy/initializing-sd-cardinitialization-failed-mega2560-sd-modul

#include <SPI.h>
#include <SD.h>
#include "DateTime_Class.h"
#include "DHT_Class.h"


#include "Adafruit_GFX.h"    // Adafruit Grafik-Bibliothek
#include "Adafruit_ST7735.h" // Adafruit ST7735-Bibliothek


#include <Wire.h>
//#include "Adafruit_BMP085.h" // Adafruit BMP085-Bibliothek

//#include "RTClib.h"

// TFT-Display
#define CS   10 // Arduino-Pin an Display CS   
#define DC   9  // Arduino-Pin an Display A0
//#define RST  0  // Arduino Reset-Pin Use RESET on board
//#define SD_CS    4  // Chip select line for SD card
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!

// Faster
// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC);  // Display-Bibliothek Setup
//Lower
// Constructor when using software SPI.  All output pins are configurable.
//Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, TFT_MOSI, TFT_SCLK, RST);

//RTC_DS1307 RTC; // Tiny RTC Modul
DateTimeClass DTC;
TemperatureClass temperature;

//Adafruit_BMP085 bmp;   // BMP085

boolean night_mode = true;
int tmp = 0;

void setup(void) {
  Serial.begin(9600);

  // Initialisiere RTC
  Wire.begin();
  DTC.init(&tft, 1000);

  temperature.init(&tft);
  
//  bmp.begin();  // BMP085 starten
  
  //dht.begin();  // DHT starten
  
  // Display
  tft.initR(INITR_BLACKTAB);     // ST7735-Chip initialisieren
  tft.setRotation(2);
  
  display_show();
}



int32_t pressure=1000;

int32_t min_pressure=1000000;
int32_t max_pressure=-1000;


void loop() {

  // Каждый раз обновляем таймер времени
  DTC.update(night_mode);
  
  float t;
  /*
  t=dht.readTemperature();
  if(isnan(t)){}
  else if((int)t!=(int)temp){
   show_temp(temp,true);
   temp=t;
   if(min_temp>temp)min_temp=temp;
   if(max_temp<temp)max_temp=temp;
   show_temp(temp,false);
  }

  
  float h = dht.readHumidity();
  if(isnan(h)){}
  else if(h!=hum){
   show_hum(hum,true);
   hum=h;
   if(min_humidity>hum)min_humidity=hum;
   if(max_humidity<hum)max_humidity=hum;
   show_hum(hum,false);
  }
  */
  
  
  //File dir = SD.open("/");
  //dir.rewindDirectory();

  /*while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     // Print the 8.3 name
     Serial.print(entry.name());
     // Recurse for directories, otherwise print the file size
     if (entry.isDirectory()) {
       Serial.println("/");
     } 
     else{
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
        //if(bmpDraw(entry.name(),0,0))delay(5000);
      }  
     entry.close();
   }

   delay(1000);*/

//  int32_t p=bmp.readPressure();
//  if(p!=pressure){
//   show_pressure(pressure,true);
//   pressure=p;
//   if(min_pressure>pressure)min_pressure=pressure;
//   if(max_pressure<pressure)max_pressure=pressure;
//   show_pressure(pressure,false);
//  }
  

  // set_text(24,75, String((int)tmp-1) , ST7735_BLACK,3);  
  // set_text(24,75, String((int)tmp) , ST7735_RED,3);  
  tmp++;
  if(tmp >= 1000)
    tmp = 0;

  delay(1000);  
 
}


void show_pressure(float pressure,boolean clear){
  
  int clearcolor=night_mode?ST7735_BLACK:ST7735_WHITE;
  int textcolor=night_mode?ST7735_WHITE:ST7735_BLACK;
  
  byte xs=12;
  byte ys=130;
  
  set_text(xs,ys,String((int32_t)pressure)+"Pa",clear?clearcolor:textcolor,1);
  
  if(max_pressure>=100000)xs-=8; 
  set_text(xs+68,ys-5,String((int32_t)max_pressure)+"Pa",clear?clearcolor:ST7735_CYAN,1);
  set_text(xs+68,ys+5,String((int32_t)min_pressure)+"Pa",clear?clearcolor:ST7735_MAGENTA,1);//
}

void set_text(int x,int y,String text,int color,int size){
  
  tft.setTextSize(size);
  tft.setCursor(x,y);
  tft.setTextColor(color);
  tft.print(text);
}

void display_show(){

  tft.fillScreen(night_mode?ST7735_BLACK:ST7735_WHITE); 
  
  set_text(2,4,"Weather station",ST7735_BLUE,1);  
  set_text(14,147,"flasher@kar.kz",ST7735_GREEN,1);
  
  DTC.show_time(DTC.now, false);
  DTC.show_date(DTC.now, false);

  //time_old=date_old=RTC.now();
  
  //show_time(time_old,false);
  //show_date(date_old,false);  
}


