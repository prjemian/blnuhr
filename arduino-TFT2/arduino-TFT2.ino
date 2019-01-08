// blnuhr: arduino-TFT2


// TODO: use Ts (TouchScreen) to identify and control time setting mode.
// Touch_shield_new has some helpful code

#include "Adafruit_GFX.h"       // Hardware-specific library
#include <Adafruit_TFTLCD.h>    // touch-screen library
#include <FreeDefaultFonts.h>
#include <MCUFRIEND_kbv.h>
#include <RtcDS3231.h>
#include <TouchScreen.h>
#include <Wire.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin


#define COMM_SPEED  9600
#define BORDER  12
#define UPDATE_INTERVAL_MS 100 // one-tenth second
#define REPORTING_INTERVAL_S 10

#define PORTRAIT  0
#define LANDSCAPE 1

#define TOUCH_ORIENTATION  LANDSCAPE


// Assign colors we'll use:
#define RED_OFF     0x8148
#define ORANGE_OFF  0x82A8
uint16_t background;

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif


//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
MCUFRIEND_kbv tft;
uint16_t  display_width, display_height, row_height;
int toggler = 1;
long micros_offset = 0;
RtcDS3231<TwoWire> Rtc(Wire);
int first_flag = 1;

const int XP=6,XM=A2,YP=A1,YM=7; //ID=0x9341
const int TS_LEFT=907,TS_RT=136,TS_TOP=942,TS_BOT=139;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MINPRESSURE 200
#define MAXPRESSURE 1000


void setup() {
  uint32_t time_0 = millis(), time_startup;
  uint32_t time_00 = time_0;

  Serial.begin(COMM_SPEED);
  if (!Serial) delay(5000);           //allow some time for startup
  time_startup = millis() - time_0;
  Serial.println("blnuhr: arduino-TFT2");
  Serial.println("(c) 2019, Pete R. Jemian");
  Serial.println();
  Serial.println("Serial took " + String(time_startup) + "ms to start");

  uint16_t ID = tft.readID();
  Serial.print("TouchScreen TFT ID = 0x");
  Serial.println(ID, HEX);
  if (ID == 0xD3D3) ID = 0x9481; // write-only shield
  time_0 = millis();
  tft.begin(ID);
  time_startup = millis() - time_0;
  Serial.println("TouchScreen took " + String(time_startup) + "ms to start");

  tft.setRotation(TOUCH_ORIENTATION);
  
  display_width = tft.width() - BORDER*2;
  display_height = tft.height() - BORDER*2;
  row_height = (display_height - BORDER*4)/5;
  background = tft.color565(30, 30, 30);
  tft.fillRect(0, 0, tft.width(), tft.height(), TFT_BLACK);
  tft.fillRect(BORDER/2, BORDER/2, display_width+BORDER, display_height+BORDER, background);
  
  time_0 = millis();
  Rtc.Begin();
  time_startup = millis() - time_0;
  Serial.println("RealTimeClock took " + String(time_startup) + "ms to start");
  setup_RTC(Rtc);
  
  Serial.println("setup() took " + String(millis()-time_00) + "ms to complete");
}


void setLights(RtcDateTime now) {
  int strobe = 0;
  long mu;
  int pwm;
  int h, m;
      
  // strobe the seconds LED on/off, 2s cycle
  mu = micros() - micros_offset;
  strobe = long(mu/1000000.) % 2;

  drawSeconds(strobe);

  // this is the blnuhr display protocol
  drawHour(now.Hour());
  drawMinute(now.Minute());
}


void drawSeconds(int visible) {
  uint16_t h, v, radius, color;
  radius = row_height/2;
  v = BORDER + radius;
  h = tft.width()/2;
  color = (visible) ? TFT_ORANGE : ORANGE_OFF;
  tft.fillCircle(h, v, radius, color);
}


void drawHour(int hr) {
  drawH15(hr);
  drawH1(hr);
}


void drawMinute(int m) {
  drawM15(m);
  drawM1(m);
}


void drawH15(int hr) {
  uint16_t top, left, h, v, radius, color;
  hr = (hr == 0) ? 24 : hr;     // show all lights on, for nightowls
  radius = BORDER/4;
  v = row_height;
  top = BORDER + 1*(v + BORDER);
  left = BORDER;
  h = (display_width - BORDER*3)/4;
  for (int t = 5; t<25; t+=5) {
    color = (hr >= t) ? TFT_RED : RED_OFF;
    tft.fillRoundRect(left, top, h, v, radius, color);
    left += BORDER + h;
  }
}


void drawH1(int hr) {
  uint16_t top, left, h, v, radius, color;
  hr = (hr == 0) ? 24 : hr;     // show all lights on, for nightowls
  radius = BORDER/4;
  v = row_height;
  top = BORDER + 2*(v + BORDER);
  left = BORDER;
  h = (display_width - BORDER*3)/4;
  for (int t = 0; t<5; t++) {
    color = ((hr % 5) > t) ? TFT_RED : RED_OFF;
    tft.fillRoundRect(left, top, h, v, radius, color);
    left += BORDER + h;
  }
}


void drawM15(int m) {
  uint16_t top, left, h, v, radius, color, color_on, color_off;
  radius = BORDER/4;
  v = row_height;
  top = BORDER + 3*(v + BORDER);
  left = BORDER;
  h = (display_width - BORDER*10)/11;
  for (int t = 5; t<59; t+=5) {
    color_on  = ((t%15) == 0) ? TFT_RED : TFT_ORANGE;
    color_off = ((t%15) == 0) ? RED_OFF : ORANGE_OFF;
    color = (m >= t) ? color_on : color_off;
    tft.fillRoundRect(left, top, h, v, radius, color);
    left += BORDER + h;
  }
}


void drawM1(int m) {
  uint16_t top, left, h, v, radius, color;
  radius = BORDER/4;
  v = row_height;
  top = BORDER + 4*(v + BORDER);
  left = BORDER;
  h = (display_width - BORDER*3)/4;
  for (int t = 0; t<5; t++) {
    color = ((m % 5) > t) ? TFT_ORANGE : ORANGE_OFF;
    tft.fillRoundRect(left, top, h, v, radius, color);
    left += BORDER + h;
  }
}


void loop() {
  TSPoint tp;
  // FIXME: tp = ts.getPoint();   //tp.x, tp.y are ADC values
  //if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
  //  Serial.println("tp: x=" + String(tp.x) + " y=" + String(tp.y) + " z=" + String(tp.z));
  //}
  
  if (!Rtc.IsDateTimeValid()) {
      // Common Causes:
      //    1) the battery on the device is low or even missing and the power line was disconnected
      Serial.println("RTC lost confidence in the DateTime!");
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now.Second() % REPORTING_INTERVAL_S == 0) {
    if (first_flag) {
      printDateTime(now);
      // Serial.println();
      Serial.print(" ");
  
      RtcTemperature temp = Rtc.GetTemperature();
      temp.Print(Serial);
      Serial.println("C");
      // you may also get the temperature as a float and print it
      // Serial.print(temp.AsFloatDegC());
      // Serial.println("C");

      tft.fillRect(BORDER, BORDER, (display_width-row_height)/2, row_height+BORDER/2, background);
      tft.setCursor(BORDER, BORDER+row_height);
      tft.setFont(&FreeSmallFont);
      tft.setTextColor(TFT_BLACK, background);
      tft.setTextSize(2);
      tft.print(String(temp.AsFloatDegC()) + "C");

      first_flag = 0;

      if (micros_offset == 0) {
        micros_offset = micros();
      }
    }
  } else {
    first_flag = 1;
  }

  //printDiagnosticBlnuhr(now);
  setLights(now);

  delay(UPDATE_INTERVAL_MS);
  toggler = toggler ? 0 : 1;
}


#define countof(a) (sizeof(a) / sizeof(a[0]))
#define ISO8601_FORMAT          "%04u-%02u-%02uT%02u:%02u:%02u"
#define ISO8601_FORMAT_RELAXED  "%04u-%02u-%02u %02u:%02u:%02u"


void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR(ISO8601_FORMAT_RELAXED),
            dt.Year(),
            dt.Month(),
            dt.Day(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}


void setup_RTC(RtcDS3231<TwoWire> Rtc) {
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    Serial.print("compiled: ");
    printDateTime(compiled);
    Serial.println("");
    
    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");

        // following line sets the RTC to the date & time this sketch was compiled
        // it will also reset the valid flag internally unless the Rtc device is
        // having an issue

        Rtc.SetDateTime(compiled);
        setLights(compiled);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

