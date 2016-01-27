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

#define BUFFER_LENGTH          40
#define COMMAND_LENGTH         16
#define EOS_TERMINATOR_CHAR    '\n'
#define GRAPHICS_HEIGHT        36
#define LEFT_BORDER            8
#define LOOP_DELAY_MS          50
#define UNDEFINED              -1

unsigned int ORANGE = RED + GREEN*7/10;
char inputString[BUFFER_LENGTH+1];
char strPtr;
boolean stringComplete;
long arg1;
long arg2;
char baseCmd[COMMAND_LENGTH+1];

static unsigned int TS_MINX, TS_MAXX, TS_MINY, TS_MAXY;
 
//Touch Screen Co-ordinate mapping register
static unsigned int MapX1, MapX2, MapY1, MapY2;

unsigned long time_reference_ms;  // calibrateTime(h, m) to set to h:m
int minutes = -1;

TouchScreen ts = TouchScreen(17, A2, A1, 14, 300); 

void setup()
{
  int i;
  resetBuffer();
  Tft.init();  //init TFT library
  Serial.begin(115200);
  Serial.println(F("blnuhr started"));

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
  readBuffer();
  processCmd();
  t = millis() - time_reference_ms;
  m = (t/1000/60) % (60*24);
  if (m != minutes) {
    minutes = m;
    displayTime(m / 60, minutes % 60);
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
  if (hours < 24 && minutes < 60) {
    for (i = 0; i < 4; i++)
      draw_5h(i, hours >= (i+1)*5);
    for (i = 0; i < 4; i++)
      draw_1h(i, (hours % 5) > i);
    for (i = 0; i < 11; i++)
      draw_5m(i, minutes >= (i+1)*5);
    for (i = 0; i < 4; i++)
      draw_1m(i, (minutes % 5) > i);
  } else {
    Serial.print("Incorrect time specified: ");
    Serial.print(hours);
    Serial.print(":");
    Serial.println(minutes);
  }
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


void readBuffer() {
  while (Serial.available()) {
    if (strPtr == BUFFER_LENGTH) {
      Serial.println(F("ERROR_BUFFER_OVERFLOW"));
      resetBuffer();    // discard the buffer contents
    } else {
      char inChar = (char)Serial.read();
      if (inChar == EOS_TERMINATOR_CHAR) {
        stringComplete = true;
        break;
      }
      inputString[strPtr++] = inChar;
      inputString[strPtr] = 0;
    }
  }
}

void resetBuffer() {
  inputString[0] = 0;    // discard the buffer contents
  strPtr = 0;
  stringComplete = false;
  baseCmd[0] = 0;
  arg1 = UNDEFINED;
  arg2 = UNDEFINED;
}

void processCmd() {
  if (stringComplete) {
    executeCommand();    // process the command
    resetBuffer();       // clear for the next command
  }
}

void executeCommand() {
  // parse the inputString into "h m" integers
  char *cmd;
  char buf[BUFFER_LENGTH+1];
  long _h, _m;
  bool success = false;
  
  strcpy(buf, inputString);  // copy locally so we don't modify source
  cmd = strtok(buf, " ");
  if (cmd) {
    _h = atoi(cmd) % 24;
    cmd = strtok(NULL, " ");
    if (cmd) {
      _m = atoi(cmd) % 60;
      cmd = strtok(NULL, " ");
      if (!cmd)
        success = true;
    }
  }
  if (success) {
    calibrateTime(int(_h), int(_m));
  } else {
    Serial.print(F("set the time by sending two integers: \"h m\\n\""));
  }
  resetBuffer();
}

void calibrateTime(int h, int m) {
  unsigned long _h=h, _m=m;
  Serial.print("calibrateTime(");
  Serial.print(h);
  Serial.print(", ");
  Serial.print(m);
  Serial.println(")");
  time_reference_ms = millis() - (_h*60 + _m)*60*1000;
}

