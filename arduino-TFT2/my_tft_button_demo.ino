
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
int pixel_x, pixel_y;     //Touch_getXY() updates global vars
enum TouchState { TOUCH_IDLE, TOUCH_HOLDING, TOUCH_WAIT_RELEASE };
TouchState touch_state = TOUCH_IDLE;
TSPoint touch_point;
bool touch_just_pressed = false;
bool touch_long_pressed = false;
uint32_t touch_start_time = 0;

// support mode to SET time
bool set_mode = false;
uint32_t set_mode_start_time = NULL;


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
  updateTouchState();
  if (touch_long_pressed) {
    Serial.println("Initiating SET mode after long press");
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
    
  updateTouchState();
  if (touch_just_pressed) {
    TSPoint p = touch_point;
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
 * is the raw pressure in range to indicate point is pressed?
 */
bool buttonIsPressed(TSPoint p) {
  return(p.z > MINPRESSURE && p.z < MAXPRESSURE);
}


/*
 * updateTouchState()
 *
 * Read the touchscreen once and turn it into press and long-press events.
 * A long press is reported once, then further input is ignored until release.
 */
void updateTouchState() {
  touch_point = getTouchPoint();
  bool pressed = buttonIsPressed(touch_point);
  touch_just_pressed = false;
  touch_long_pressed = false;

  if (touch_state == TOUCH_IDLE) {
    if (pressed) {
      touch_state = TOUCH_HOLDING;
      touch_start_time = millis();
      touch_just_pressed = true;
    }
  } else if (touch_state == TOUCH_HOLDING) {
    if (!pressed) {
      touch_state = TOUCH_IDLE;
    } else if (millis() - touch_start_time >= LONG_PRESS_MIN_MS) {
      touch_state = TOUCH_WAIT_RELEASE;
      touch_long_pressed = true;
    }
  } else if (touch_state == TOUCH_WAIT_RELEASE) {
    if (!pressed) {
      touch_state = TOUCH_IDLE;
    }
  }
}


/*
 * getTouchPoint()
 *
 * return a TSPoint object with the current TouchScreen event
 */
TSPoint getTouchPoint() {
  TSPoint p = ts.getPoint();

  // restore pins shared with TFT
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);
  digitalWrite(XM, HIGH);

  return(p);
}

