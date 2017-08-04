// BMP085, DHT11, TinyRTC (DS1307) und 1.8 Zoll TFT-Farb-Display (HY-1.8 SPI)
//https://forum.arduino.cc/index.php?topic=165170.0
//http://arduino.ru/forum/apparatnye-voprosy/initializing-sd-cardinitialization-failed-mega2560-sd-modul

#include <SPI.h>
#include <SD.h>
#include "DateTime_Class.h"
#include "DHT_Class.h"
#include "Buzzer_Class.h"
#include "ADC_PCF8591_Sensor_Class.h"


#include "Adafruit_GFX.h"    // Adafruit Grafik-Bibliothek
#include "Adafruit_ST7735.h" // Adafruit ST7735-Bibliothek


//#include <Wire.h>
//#include "Adafruit_BMP085.h" // Adafruit BMP085-Bibliothek

//#include "RTClib.h"

#define BTN_PIN A1

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

#define DHT_PIN 2
#define DHT_TYPE DHT21

TemperatureClass temperature(DHT_PIN, DHT_TYPE);

ADCSensorClass sensor;

Buzzer buzzer;

//Adafruit_BMP085 bmp;   // BMP085


int activeScreen = 1;
int totalScreen = 4;

boolean night_mode = true;
int tmp = 0;
int buttonValue;

void setup(void) {
  Serial.begin(9600);

  pinMode(BTN_PIN, INPUT_PULLUP);
  //digitalWrite(BTN_PIN, INPUT_PULLUP);

  buzzer.init();


  // Initialisiere RTC
  //Wire.begin();
  DTC.init(&tft, 1000);

  temperature.init(&tft);

  buzzer.welcome_beep();
  
//  bmp.begin();  // BMP085 starten
  
  //dht.begin();  // DHT starten

  sensor.init(&tft);
  
  // Display
  tft.initR(INITR_BLACKTAB);     // ST7735-Chip initialisieren
  tft.setRotation(2);
  
  display_show();
  temperature.update(night_mode, true);
}



int32_t pressure=1000;

int32_t min_pressure=1000000;
int32_t max_pressure=-1000;

byte switchState = 1;
byte lastState = 1;
word shortDuration = 500;
word longDuration = 2000;
unsigned long switchTime = 0;

int current;         // Current state of the button
                     // (LOW is pressed b/c i'm using the pullup resistors)
long millis_held;    // How long the button was held (milliseconds)
long secs_held;      // How long the button was held (seconds)
long prev_secs_held; // How long the button was held in the previous check
byte previous = HIGH;
unsigned long firstTime; // how long since the button was first pressed 

void loop() {

  // Каждый раз обновляем таймер времени
  DTC.update(night_mode);

  // Читаем температуру и влажность
  temperature.update(night_mode);

  // Читаем датчики PCF8591
  sensor.update(night_mode);
  
  //Serial.println(analogRead(BTN_PIN));
  

//  int32_t p=bmp.readPressure();
//  if(p!=pressure){
//   show_pressure(pressure,true);
//   pressure=p;
//   if(min_pressure>pressure)min_pressure=pressure;
//   if(max_pressure<pressure)max_pressure=pressure;
//   show_pressure(pressure,false);
//  }
  
  //buttonValue = analogRead(BTN_PIN); 

  current = digitalRead(BTN_PIN);

  // if the button state changes to pressed, remember the start time 
  if (current == LOW && previous == HIGH && (millis() - firstTime) > 200) {
    firstTime = millis();
  }

  millis_held = (millis() - firstTime);
  secs_held = millis_held / 1000;

  // This if statement is a basic debouncing tool, the button must be pushed for at least
  // 100 milliseconds in a row for it to be considered as a push.
  if (millis_held > 50) {

    if (current == LOW && secs_held > prev_secs_held) {
    }

    // check if the button was released since we last checked
    if (current == HIGH && previous == LOW) {
      // HERE YOU WOULD ADD VARIOUS ACTIONS AND TIMES FOR YOUR OWN CODE
      // ===============================================================================

      // Button pressed for less than 1 second, one long LED blink
      if (secs_held <= 0) {
        buzzer.button_beep();

        if(activeScreen == totalScreen)
          activeScreen = 0;

        activeScreen++;

        changeScreen();
      }

      // If the button was held for 3-6 seconds blink LED 10 times
      if (secs_held >= 1 && secs_held < 3) {
        buzzer.button_beep();
        delay(500);
        buzzer.button_beep();
      }

      // Button held for 1-3 seconds, print out some info
      if (secs_held >= 3) {
        buzzer.welcome_beep();
      }
      // ===============================================================================
    }
  }

  previous = current;
  prev_secs_held = secs_held;

  set_text(1,125, String((int)tmp-1) , ST7735_BLACK,2);  
  set_text(1,125, String((int)tmp) , ST7735_WHITE,2);  
  tmp++;
  if(tmp >= 1000)
    tmp = 0;

  //delay(1000);  
 
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

void changeScreen(){

  //Serial.println(activeScreen);

  if(activeScreen == 1){
    display_show();
  }
  else if(activeScreen == 2){
    String str = String(sensor.sensorValue1);
    tft.fillScreen(night_mode?ST7735_BLACK:ST7735_WHITE);
    set_text(4,55,str, ST7735_GREEN,4);
  } 
  else if(activeScreen == 3){
    String str = String(sensor.sensorValue2);
    tft.fillScreen(night_mode?ST7735_BLACK:ST7735_WHITE);
    set_text(4,55,str, ST7735_GREEN,4);
  } 
  else {
    tft.fillScreen(night_mode?ST7735_BLACK:ST7735_WHITE);
  }
}

