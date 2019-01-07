
// file: blnuhr_led



// CONNECTIONS
// -----------
// DS3231 SDA --> SDA
// DS3231 SCL --> SCL
// DS3231 VCC --> 3.3v or 5v
// DS3231 GND --> GND


// LIBRARIES
// ---------
// https://github.com/Makuna/Rtc/wiki/RtcDS3231-object

#include <Wire.h> 
#include <RtcDS3231.h>

#define REPORTING_INTERVAL_S 10

//#define UPDATE_INTERVAL_MS 10000 // ten seconds
//#define UPDATE_INTERVAL_MS 2000 // two seconds
#define UPDATE_INTERVAL_MS 100 // one-tenth second
//#define UPDATE_INTERVAL_MS 1000 // one second

#define PIN_SECONDS_PWM   6
#define PIN_MINUTES_1    22
#define PIN_MINUTES_2    23
#define PIN_MINUTES_3    24
#define PIN_MINUTES_4    25
#define PIN_MINUTES_5    26
#define PIN_MINUTES_10   27
#define PIN_MINUTES_15   28
#define PIN_MINUTES_20   29
#define PIN_MINUTES_25   30
#define PIN_MINUTES_30   31
#define PIN_MINUTES_35   32
#define PIN_MINUTES_40   33
#define PIN_MINUTES_45   34
#define PIN_MINUTES_50   35
#define PIN_MINUTES_55   36
#define PIN_HOURS_1      38
#define PIN_HOURS_2      39
#define PIN_HOURS_3      40
#define PIN_HOURS_4      41
#define PIN_HOURS_5      42
#define PIN_HOURS_10     43
#define PIN_HOURS_15     44
#define PIN_HOURS_20     45

int first_flag = 1;
long micros_offset = 0;
RtcDS3231<TwoWire> Rtc(Wire);

void setup()
{
    pinMode(PIN_SECONDS_PWM, OUTPUT);
    pinMode(PIN_MINUTES_1, OUTPUT);
    pinMode(PIN_MINUTES_2, OUTPUT);
    pinMode(PIN_MINUTES_3, OUTPUT);
    pinMode(PIN_MINUTES_4, OUTPUT);
    pinMode(PIN_MINUTES_5, OUTPUT);
    pinMode(PIN_MINUTES_10, OUTPUT);
    pinMode(PIN_MINUTES_15, OUTPUT);
    pinMode(PIN_MINUTES_20, OUTPUT);
    pinMode(PIN_MINUTES_25, OUTPUT);
    pinMode(PIN_MINUTES_30, OUTPUT);
    pinMode(PIN_MINUTES_35, OUTPUT);
    pinMode(PIN_MINUTES_40, OUTPUT);
    pinMode(PIN_MINUTES_45, OUTPUT);
    pinMode(PIN_MINUTES_50, OUTPUT);
    pinMode(PIN_MINUTES_55, OUTPUT);
    pinMode(PIN_HOURS_1, OUTPUT);
    pinMode(PIN_HOURS_2, OUTPUT);
    pinMode(PIN_HOURS_3, OUTPUT);
    pinMode(PIN_HOURS_4, OUTPUT);
    pinMode(PIN_HOURS_5, OUTPUT);
    pinMode(PIN_HOURS_10, OUTPUT);
    pinMode(PIN_HOURS_15, OUTPUT);
    pinMode(PIN_HOURS_20, OUTPUT);
    
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    
    // Setup Serial connection
    Serial.begin(115200);

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

    setLights(now);
}

void loop () 
{
    if (!Rtc.IsDateTimeValid()) 
    {
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
}


void printDiagnosticBlnuhr(RtcDateTime now) {
  // diagnostic output in blnuhr format
  Serial.println("-----------------");
  Serial.print(now.Hour());
  Serial.print(" ");
  Serial.print(now.Hour()>4);
  Serial.print(now.Hour()>9);
  Serial.print(now.Hour()>14);
  Serial.print(now.Hour()>19);
  Serial.println();

  Serial.print(now.Hour());
  Serial.print(" ");
  Serial.print(now.Hour()%5>0);
  Serial.print(now.Hour()%5>1);
  Serial.print(now.Hour()%5>2);
  Serial.print(now.Hour()%5>3);
  Serial.println();
  Serial.print(now.Minute());
  Serial.print(" ");

  Serial.print(now.Minute()>4);
  Serial.print(now.Minute()>9);
  Serial.print(now.Minute()>14);
  Serial.print(now.Minute()>19);
  Serial.print(now.Minute()>24);
  Serial.print(now.Minute()>29);
  Serial.print(now.Minute()>34);
  Serial.print(now.Minute()>39);
  Serial.print(now.Minute()>44);
  Serial.print(now.Minute()>49);
  Serial.print(now.Minute()>54);
  Serial.println();

  Serial.print(now.Minute());
  Serial.print(" ");
  Serial.print(now.Minute()%5>0);
  Serial.print(now.Minute()%5>1);
  Serial.print(now.Minute()%5>2);
  Serial.print(now.Minute()%5>3);
  Serial.println();
}

void setLights(RtcDateTime now) {
  int strobe = 0;
  long mu;
  int pwm;
  int h, m;
      
  // strobe the seconds LED on/off, 2s cycle
  mu = micros() - micros_offset;
  strobe = long(mu/1000000.) % 2;

  // 255 is off, 0 is on
  // strobe on/off, saw-tooth when on, brightest at end of each minute
  pwm = 255 - strobe*int(now.Second()*255.0/60.);  // saw-tooth

  analogWrite(PIN_SECONDS_PWM, pwm);
  // digitalWrite(PIN_SECONDS, now.Second()%2);

  // this is the blnuhr display protocol
  h = now.Hour();
  m = now.Minute();
  digitalWrite(PIN_HOURS_5,    !(h>4));
  digitalWrite(PIN_HOURS_10,   !(h>9));
  digitalWrite(PIN_HOURS_15,   !(h>14));
  digitalWrite(PIN_HOURS_20,   !(h>19));

  digitalWrite(PIN_HOURS_1,    !(h%5>1));
  digitalWrite(PIN_HOURS_2,    !(h%5>1));
  digitalWrite(PIN_HOURS_3,    !(h%5>2));
  digitalWrite(PIN_HOURS_4,    !(h%5>3));

  digitalWrite(PIN_MINUTES_5,  !(m>4));
  digitalWrite(PIN_MINUTES_10, !(m>9));
  digitalWrite(PIN_MINUTES_15, !(m>14));
  digitalWrite(PIN_MINUTES_20, !(m>19));
  digitalWrite(PIN_MINUTES_25, !(m>24));
  digitalWrite(PIN_MINUTES_30, !(m>29));
  digitalWrite(PIN_MINUTES_35, !(m>34));
  digitalWrite(PIN_MINUTES_40, !(m>39));
  digitalWrite(PIN_MINUTES_45, !(m>44));
  digitalWrite(PIN_MINUTES_50, !(m>49));
  digitalWrite(PIN_MINUTES_55, !(m>54));

  digitalWrite(PIN_MINUTES_1,  !(m%5>0));
  digitalWrite(PIN_MINUTES_2,  !(m%5>1));
  digitalWrite(PIN_MINUTES_3,  !(m%5>2));
  digitalWrite(PIN_MINUTES_4,  !(m%5>3));
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            //PSTR("%04u-%02u-%02uT%02u:%02u:%02u"),        // strict ISO8601
            PSTR("%04u-%02u-%02u %02u:%02u:%02u"),          // relaxed ISO8601
            dt.Year(),
            dt.Month(),
            dt.Day(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}
