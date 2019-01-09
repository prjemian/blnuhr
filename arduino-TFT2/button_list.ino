/* an alternative approach.   swap the #if 1 / 0 values to try it
 * 
 */
#if 1

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
// const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
/*
  TouchScreen.h GFX Calibration
  Making all control and bus pins INPUT_PULLUP
  Typical 30k Analog pullup with corresponding pin
  would read low when digital is written LOW
  e.g. reads ~25 for 300R X direction
  e.g. reads ~30 for 500R Y direction
  
  Testing : (A2, D8) = 25
  Testing : (A3, D9) = 27
  Diagnosing as:-
  XM,XP:  (A2, D8) = 25
  YP,YM:  (A3, D9) = 27
  ID = 0x9341
  
  cx=161 cy=106 cz=574 LEFT, TOP, Pressure
  cx=532 cy=90 cz=578 LEFT, MIDH, Pressure
  cx=896 cy=103 cz=650 LEFT, BOT, Pressure
  cx=148 cy=486 cz=413 MIDW, TOP, Pressure
  cx=896 cy=490 cz=561 MIDW, BOT, Pressure
  cx=146 cy=876 cz=223 RT, TOP, Pressure
  cx=529 cy=875 cz=341 RT, MIDH, Pressure
  cx=900 cy=876 cz=476 RT, BOT, Pressure
  MCUFRIEND_kbv ID=0x9341  240 x 320
  
  const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
  const int TS_LEFT=930,TS_RT=117,TS_TOP=73,TS_BOT=900;
  PORTRAIT CALIBRATION     240 x 320
  x = map(p.x, LEFT=930, RT=117, 0, 240)
  y = map(p.y, TOP=73, BOT=900, 0, 320)
  Touch Pin Wiring XP=8 XM=A2 YP=A3 YM=9
  LANDSCAPE CALIBRATION    320 x 240
  x = map(p.y, LEFT=73, RT=900, 0, 320)
  y = map(p.x, TOP=117, BOT=930, 0, 240)
 */
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=930,TS_RT=117,TS_TOP=73,TS_BOT=900;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button on_btn, off_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    // tft.setRotation(1);            //LANDSCAPE
    tft.fillScreen(BLACK);
    on_btn.initButton(&tft,  60, 200, 100, 40, WHITE, CYAN, BLACK, "ON", 2);
    off_btn.initButton(&tft, 180, 200, 100, 40, WHITE, CYAN, BLACK, "OFF", 2);
    on_btn.drawButton(false);
    off_btn.drawButton(false);
    tft.fillRect(40, 80, 160, 80, RED);
}

/*  
 * updating multiple buttons from a list
 * 
 * anything more than two buttons gets repetitive
 * 
 * you can place button addresses in separate lists
 * e.g. for separate menu screens
 */

// Array of button addresses to behave like a list
Adafruit_GFX_Button *buttons[] = {&on_btn, &off_btn, NULL};

/* update the state of a button and redraw as reqd
 *
 * main program can use isPressed(), justPressed() etc
 */
bool update_button(Adafruit_GFX_Button *b, bool down)
{
    b->press(down && b->contains(pixel_x, pixel_y));
    if (b->justReleased())
        b->drawButton(false);
    if (b->justPressed())
        b->drawButton(true);
    return down;
}

/* most screens have different sets of buttons
 * life is easier if you process whole list in one go
 */
bool update_button_list(Adafruit_GFX_Button **pb)
{
    bool down = Touch_getXY();
    for (int i = 0 ; pb[i] != NULL; i++) {
        update_button(pb[i], down);
    }
    return down;
}

/* compare the simplicity of update_button_list()
 */
void loop(void)
{
    update_button_list(buttons);  //use helper function
    if (on_btn.justPressed()) {
        tft.fillRect(40, 80, 160, 80, GREEN);
    }
    if (off_btn.justPressed()) {
        tft.fillRect(40, 80, 160, 80, RED);
    }
}
#endif

