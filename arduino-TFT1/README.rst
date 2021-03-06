===============================
Berlin Clock - hardware project
===============================

Basic display (R: red, O: orange)::

                  [O]
   [--R--]  [--R--] [--R--]  [--R--]
   [--R--]  [--R--] [--R--]  [--R--]
   [O][O][R][O][O][R][O][O][R][O][O]
   [--O--]  [--O--] [--O--]  [--O--]

* Need controls to set the current time.
  - could be just two buttons: mode & select
* and indication when the current time is not set.
* "Blinking 12:00" is the right metaphor.

Possible Implementations
========================

# LCD circuit
  - larger than touch screen
  - more expensive to produce custom circuit board
  - more labor to produce custom circuit board
  - design is closer to original implementation
  - I/O points require Arduino Mega (mega1280 or mega2560)

# Touch screen
  - faster to implement
  - compact design, easy to produce
  - smaller than LCD circuit
  - can use Arduino Uno
  - TFT screen graphics are primitive, needs more code
    + draw each graphic object using basic commands
	+ "clear" object by drawing background to that area
  - TFT touch interface is primitive
  - easier interface to set the time:
    + button to enter/exit mode to set the time
	+ touch the lights for the time configuration
	+ double-touch to clear the row

Arduino Real Time Clock Using DS3231 
====================================

Exactly what we want:
https://www.instructables.com/id/Real-time-clock-using-DS3231-EASY/

but the real code is here:
http://www.rinkydinkelectronics.com/library.php?id=73


https://github.com/Makuna/Rtc/wiki/RtcDS3231-object


Lunch Happened Bento Lunchbox
www.itzyritzy.com
no longer produced
search for "bento lunch boxes" for a wide range of possibilities

Seeedstudio TFT Touch Screen
============================

:main support page for v1.0 board:
  http://www.seeedstudio.com/wiki/TFT_Touch_Shield_V1.0

other support

* http://www.seeedstudio.com/wiki/TFT_Touch_Shield
* http://www.seeedstudio.com/depot/28-tft-touch-shield-p-864.html
* http://forum.arduino.cc/index.php?topic=117611.0

* display screen is 320x240
* need to map out the display
* 5 rows plus borders: 4 px wide, 40 px high colors
* 8 px border on left and right
* large rectangles: 40x73, 2 px border
* small rectangles: 40x24, 2 px border
* use "*" character to indicate "mode" button
* calibrate time to the minute by clicking the "seconds" blinker


ezLCD 3.5" Touchscreen with Arduino Uno R3
==========================================

https://earthlcd.com/products/educational-maker/arlcd/
https://github.com/earthmake/arLCD
http://www.mcmelectronics.com/product/83-15516?scode=DD1621&trk_msg=K8SM3TQT26GKJ42ODNPTHJT1PG&trk_contact=64H3CG1IQAOJBCELHDHA484HF4&utm_source=Listrak&utm_medium=Email&utm_term=83-15516&utm_campaign=DD1621_v2&utm_content=DD1621_v2
$39.99 (sale price: 2016-02-01) -- ONE DAY SALE - now out of stock

RaspberryPi with Touch Screen
=============================

Raspberry Pi
------------

RASPBERRY-PI  RASPBERRYPI-2-MODB-1GB.  SBC, RASPBERRY PI 2, MODEL B, 1GB

* http://www.newark.com/raspberry-pi/raspberrypi-2-modb-1gb/sbc-raspberry-pi-2-model-b-1gb/dp/38Y6467
* $35.00


Touch Screen
------------

RASPBERRY-PI  RASPBERRYPI-DISPLAY  Raspberry Pi 7" Touch Screen Display with 10 Finger Capacitive Touch 

* http://www.newark.com/raspberry-pi/raspberrypi-display/display-7-touch-screen-rpi-sbc/dp/49Y1712?ost=Raspberry+Pi+7"+Touch+Screen+Display
* 60.00

Enclosure
---------

MULTICOMP  CBRPP-TS-BLK/WHT  RASPBERRY PI TOUCHSCREEN ENCLOSURE 
* http://www.newark.com/multicomp/cbrpp-ts-blk-wht/raspberry-pi-touchscreen-enclosure/dp/55Y8475
DS
* $45.74
