
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <FreeDefaultFonts.h>


#define MINPRESSURE 15
#define MAXPRESSURE 1000
#define NO_BUTTON_DOWN -1
#define LONG_PRESS_MIN_MS 2000
#define LONG_PRESS_TIMEOUT_MS 10000

#define PORTRAIT 0
#define LANDSCAPE 1
#define DISPLAY_ROTATION   LANDSCAPE

#define BACKGROUND 0x18e3


MCUFRIEND_kbv tft;
// calibrated for Velleman VMA412 2.8" 320x240 TouchScreen
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=930,TS_RT=117,TS_TOP=73,TS_BOT=900;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button ok_btn, cancel_btn, h10up_btn, h1up_btn, m10up_btn, m1up_btn;

// touch screen input
float touch_pressure = 0;
const float touch_smooth = 0.96;  // between 0 and 1
bool pressed_last = false;
int pixel_x, pixel_y;     //Touch_getXY() updates global vars

// support mode to SET time
bool set_mode = false;
uint32_t set_mode_start_time = NULL;
uint32_t press_waiting_start_time = NULL;


void setup() {
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    tft.begin(ID);
    tft.setRotation(DISPLAY_ROTATION);
    tft.fillScreen(TFT_BLACK);

    tft.fillRect(20, 20, 280, 200, BACKGROUND);
}


void loop() {
    if (set_mode) {
      do_set_mode();
    } else {
      do_operate_mode();
    }
    delay(5);
}


/*
 * do_operate_mode()
 * 
 * show the blnuhr clock
 */
void do_operate_mode() {
  
  // watch for user to choose mode to SET the time
  uint32_t  elapsed = longPressForSetMode();
  if (LONG_PRESS_MIN_MS <= elapsed && elapsed < LONG_PRESS_TIMEOUT_MS) {
    Serial.println("Initiating SET mode after longPress=" + String(elapsed) + " ms");
    set_mode = true;
    init_set_mode();
  }

  // TODO: show the clock
}


void init_set_mode() {
  int font_size = 2;
  tft.fillRect(20, 20, 280, 200, TFT_DARKGREY);
  ok_btn.initButtonUL(&tft, 20, 180, 100, 40, TFT_WHITE, TFT_LIGHTGREY, TFT_BLACK, "OK", font_size);
  cancel_btn.initButtonUL(&tft, 200, 180, 100, 40, TFT_WHITE, TFT_LIGHTGREY, TFT_BLACK, "Cancel", font_size);
  ok_btn.drawButton(false);
  cancel_btn.drawButton(false);

  // TODO: use actual hour and minute from RTC
  makeSpinButton(&h10up_btn,  80, 120, "0");
  makeSpinButton(&h1up_btn,  120, 120, "7");
  makeSpinButton(&m10up_btn, 170, 120, "2");
  makeSpinButton(&m1up_btn,  210, 120, "5");

  //  for (int i=70; i <110; i++) {
  //    for (int j=20; j <120; j++)
  //      Serial.print(String(h10up_btn.contains(i, j)));
  //    Serial.println();
  //  }
}


void makeSpinButton(Adafruit_GFX_Button *btn, int x, int y, char *digit) {
  tft.setFont(&FreeSevenSegNumFont);
  tft.setTextColor(TFT_WHITE, TFT_LIGHTGREY);
  tft.setTextSize(0);
  tft.setCursor(x, y);
  tft.print(digit);

  int sz = 24;
  x += 2;
  y -= 55 + sz;    // offset for top of button
  // invisible if all colors are the background
  btn->initButtonUL(&tft, x, y, sz, sz, TFT_DARKGREY, TFT_DARKGREY, TFT_DARKGREY, "", 2);
  btn->drawButton(false);
  // Serial.println("In/out ? " + String(btn->contains(x, y)) + " " + String(x) + " " + String(y));

  // overlap the button exactly ... as an up triangle
  y += sz - 3;
  sz -= 2;
  tft.fillTriangle(x, y, x+sz, y, x+sz/2, y-sz, TFT_LIGHTGREY);
}

/*
 * do_set_mode()
 * 
 * set the clock time and RTC
 */
void do_set_mode() {
  bool done = false;
  // TODO: replace dummy code below with actual function
  if (set_mode_start_time == NULL)
    set_mode_start_time = millis();
    
  TSPoint p = getTouchPoint();
  bool pressed = touch_pressure > MINPRESSURE && p.z > MINPRESSURE;
  if (pressed) {
    // Serial.println("touched ...");
    if (DISPLAY_ROTATION == PORTRAIT) {
      pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
      pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    } else {    // LANDSCAPE
      pixel_x = map(p.y, TS_TOP, TS_BOT, 0, tft.width());
      pixel_y = map(p.x, TS_RT, TS_LEFT, 0, tft.height());
    }
    // Serial.print(String(p.x) + ", " + String(p.y));
    // Serial.println("   " + String(pixel_x) + ", " + String(pixel_y));

    if (ok_btn.contains(pixel_x, pixel_y)) {
      ok_btn.drawButton(true);
      Serial.println("OK button pressed");
      // TODO: set RTC with new clock value
      delay(1000);    // gratuitous
      tft.fillRect(20, 20, 280, 200, TFT_GREEN);
      done = true;
    }
    if (cancel_btn.contains(pixel_x, pixel_y)) {
      cancel_btn.drawButton(true);
      Serial.println("CANCEL button pressed");
      // TODO: RTC unchanged
      delay(1000);    // gratuitous
      tft.fillRect(20, 20, 280, 200, TFT_WHITE);
      done = true;
    }

    if (h10up_btn.contains(pixel_x, pixel_y)) {
      Serial.println("h10up_btn pressed");
      set_mode_start_time = millis();     // extend the time
      // TODO: wait for button release before acting
    }
    if (h1up_btn.contains(pixel_x, pixel_y)) {
      Serial.println("h1up_btn pressed");
      set_mode_start_time = millis();     // extend the time
      // TODO: wait for button release before acting
    }
    if (m10up_btn.contains(pixel_x, pixel_y)) {
      Serial.println("m10up_btn pressed");
      set_mode_start_time = millis();     // extend the time
      // TODO: wait for button release before acting
    }
    if (m1up_btn.contains(pixel_x, pixel_y)) {
      Serial.println("m1up_btn pressed");
      set_mode_start_time = millis();     // extend the time
      // TODO: wait for button release before acting
    }
  }

  if (done || (millis()-set_mode_start_time > 60000)) {
    Serial.print("Leaving SET mode after ");
    Serial.print(millis()-set_mode_start_time);
    Serial.println(" ms");
    set_mode = false;
    set_mode_start_time = NULL;
    tft.fillRect(20, 20, 280, 200, BACKGROUND);
  }
}


/*
 * buttonIsPressed(point)
 * 
 * is the pressure in range to indicate point is pressed?
 */
bool buttonIsPressed(TSPoint p) {
  return(p.z > MINPRESSURE && p.z < MAXPRESSURE);
}


/*
 * getTouchPoint()
 * 
 * return a TSPoint object with the current TouchScreen event
 * 
 * also compute smooth ``touch_pressure`` value that debounces the unstable raw value
 */
TSPoint getTouchPoint() {
  TSPoint p = ts.getPoint();

  // restore pins shared with TFT
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);
  digitalWrite(XM, HIGH);

  //  if (p.z > 0) {
  //    Serial.print("getTouchPoint():");
  //    Serial.print(" x:" + String(p.x));
  //    Serial.print(" y:" + String(p.y));
  //    Serial.print(" z:" + String(p.z));
  //    Serial.println();
  //  }

  // global variables: touch_pressure, touch_smooth
  touch_pressure = touch_smooth*touch_pressure + p.z*(1-touch_smooth);   // debouncing
  
  return(p);
}

/*
 * longPressForSetMode()
 * 
 * User will press and hold touch screen to initiate time SET mode
 * keep holding   : press of less than LONG_PRESS_MIN_MS (~2s)
 * enter SET mode : press between LONG_PRESS_MIN_MS and LONG_PRESS_TIMEOUT_MS
 * show time      : press >= LONG_PRESS_TIMEOUT_MS (~10s)
 */
uint32_t longPressForSetMode() {
  uint32_t  elapsed = 0;
  TSPoint pt_now = getTouchPoint();
  bool pressed = touch_pressure > MINPRESSURE;

  if (pressed) {
    if (pressed != pressed_last) {
      press_waiting_start_time = millis();
      tft.fillRect(20, 20, 20, 20, TFT_YELLOW);
    } else {
      elapsed = millis() - press_waiting_start_time;
      // Serial.println("waiting ...  duration:" + String(elapsed) + " ms");
      if (LONG_PRESS_MIN_MS <= elapsed && elapsed < LONG_PRESS_TIMEOUT_MS) {
        tft.fillRect(20, 20, 20, 20, TFT_GREEN);
      }
      if (elapsed >= LONG_PRESS_TIMEOUT_MS) {
        tft.fillRect(20, 20, 20, 20, TFT_RED);
      }
    }
    elapsed = 0;  // still waiting, return value is zero now

  } else {  // !pressed
    if (pressed != pressed_last) {
      elapsed = millis() - press_waiting_start_time;
      // Serial.println("Just released.  duration:" + String(elapsed) + " ms");
      press_waiting_start_time = NULL;
      tft.fillRect(20, 20, 20, 20, BACKGROUND);
    }
  }

  pressed_last = pressed;
  return(elapsed);
}

