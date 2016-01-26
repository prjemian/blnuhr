// blnuhr demo

#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT.h>

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

unsigned int ORANGE = RED + GREEN*7/10;
#define GRAPHICS_HEIGHT 36
#define LEFT_BORDER     8
#define LOOP_DELAY_MS   50

static unsigned int TS_MINX, TS_MAXX, TS_MINY, TS_MAXY;
 
//Touch Screen Co-ordinate mapping register
static unsigned int MapX1, MapX2, MapY1, MapY2;

// TODO: need a way to calibrate time_reference_ms to a known time
unsigned long time_reference_ms;
int minutes = -1;

TouchScreen ts = TouchScreen(17, A2, A1, 14, 300); 

void setup()
{
  int i;
  Tft.init();  //init TFT library
  Serial.begin(115200);

  // guide lines
  //for (i = 0; i < 220; i += 48)
  //  Tft.drawRectangle(i, 0, 48, 319, BLUE);

  startup_animation();

  time_reference_ms = millis();
}

void loop()
{
  int m;
  unsigned long t;
  Point p;
  p = getTouch();
  t = millis() - time_reference_ms;
  m = t/1000/60;
  if (m != minutes) {
    minutes = m;
    displayTime(m / 60, minutes);
    Serial.println(t);
  }
  draw_1s((t / 500) % 2);
  delay(LOOP_DELAY_MS);
}

Point getTouch() {
  // identify which object was touched (if at all)
  Point p = ts.getPoint();
  if (p.z > ts.pressureThreshhold) {
    p.x = 4 - map(p.x, 0, 1023, 0, 5);   // row
    if (p.x == 3) {
      p.y = map(p.y, 0, 1023, 0, 12);    // column
    } else {
      p.y = map(p.y, 0, 1023, 0, 4);     // column
    }
    Serial.print("  pressureThreshhold: ");
    Serial.print(ts.pressureThreshhold);
    Serial.print("   x: ");
    Serial.print(p.x);
    Serial.print("   y: ");
    Serial.print(p.y);
    Serial.print("   z: ");
    Serial.println(p.z);
  }
  return p;
}

void displayTime(int hours, int minutes) {
  int i;
  for (i = 0; i < 4; i++)
    draw_5h(i, hours >= (i+1)*5);
  for (i = 0; i < 4; i++)
    draw_1h(i, (hours % 5) > i);
  for (i = 0; i < 11; i++)
    draw_5m(i, minutes >= (i+1)*5);
  for (i = 0; i < 4; i++)
    draw_1m(i, (minutes % 5) > i);
}

void startup_animation() {
//  startup_animation1();
  startup_animation2();
}

void startup_animation1() {
  int i;
  for (i = 0; i < 5; i++)
    displayTime(0, i);
  for (i = 0; i < 60; i+=5)
    displayTime(0, i);
  for (i = 0; i < 5; i++)
    displayTime(i, 0);
  for (i = 0; i < 24; i+=5)
    displayTime(i, 0);
}

void startup_animation2() {
  int i;
  for (i = 0; i < 4; i++) {
    draw_1m(i, 1);
    delay(50);
    draw_1m(i, 0);
  }
  for (i = 0; i < 11; i++) {
    draw_5m(i, 1);
    delay(50);
    draw_5m(i, 0);
  }
  for (i = 0; i < 4; i++) {
    draw_1h(i, 1);
    delay(50);
    draw_1h(i, 0);
  }
  for (i = 0; i < 4; i++) {
    draw_5h(i, 1);
    delay(50);
    draw_5h(i, 0);
  }
}

void draw_5h(int column, int state) {
  if (column < 4) {
    unsigned int color = state ? RED : GRAY2;
    int width = 73;
    int border = 2;
    int row = 54;
    int x = MAX_Y - (LEFT_BORDER + width + column*(width + 2*border));
    Tft.fillRectangle(row,x, GRAPHICS_HEIGHT,width, color);
  }
}

void draw_1h(int column, int state) {
  if (column < 4) {
    unsigned int color = state ? RED : GRAY2;
    int width = 73;
    int border = 2;
    int row = 102;
    int x = MAX_Y - (LEFT_BORDER + width + column*(width + 2*border));
    Tft.fillRectangle(row,x, GRAPHICS_HEIGHT,width, color);
  }
}

void draw_5m(int column, int state) {
  if (column < 11) {
    unsigned int color = state ? ((column % 3 == 2) ? RED : ORANGE) : ((column % 3 == 2) ? GRAY2 : GRAY1);
    int width = 24;
    int border = 2;
    int row = 150;
    int x = MAX_Y - (LEFT_BORDER + width + column*(width + 2*border));
    Tft.fillRectangle(row,x, GRAPHICS_HEIGHT,width, color);
  }
}

void draw_1m(int column, int state) {
  if (column < 4) {
    unsigned int color = state ? ORANGE : GRAY1;
    int width = 73;
    int border = 2;
    int row = 198;
    int x = MAX_Y - (LEFT_BORDER + width + column*(width + 2*border));
    Tft.fillRectangle(row,x, GRAPHICS_HEIGHT,width, color);
  }
}

void draw_1s(int state) {
  unsigned int color = state ? ORANGE : GRAY1;
  int row = 24;
  Tft.fillCircle(row, MAX_Y/2, GRAPHICS_HEIGHT/2, color);
}

