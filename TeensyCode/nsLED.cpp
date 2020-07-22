#include "nsLED.h"

#include <arduino.h>


#define ns_led_01 2
#define ns_led_02 8
#define ns_led_03 30
#define ns_led_04 16
#define ns_led_05 17
#define ns_led_06 21
#define ns_led_07 20 

nsLED::nsLED()
{
}

void    nsLED::init(void)
    {
    pinMode(ns_led_01, OUTPUT);
    pinMode(ns_led_02, OUTPUT);
    pinMode(ns_led_03, OUTPUT);
    pinMode(ns_led_04, OUTPUT);  //non-pWM
    pinMode(ns_led_05, OUTPUT);  //non-pWM
    pinMode(ns_led_06, OUTPUT);
    pinMode(ns_led_07, OUTPUT);

    mBrightness[0]=0;
    mBrightness[1]=0;
    mBrightness[2]=0;
    mBrightness[3]=0;
    mBrightness[4]=0;
    mBrightness[5]=0;
    mBrightness[6]=0;
    mBrightness[7]=0;

    mLastUpdate = millis();
    mCount = 1;
    }
    
void    nsLED::setBrightness(int led, int bright)
    {
    if(led<8)
      mBrightness[led]=bright;
    update();
    }

void    nsLED::update(void)
    {
    digitalWrite(ns_led_01, (mBrightness[0]>0)?HIGH:LOW);   // set the LED on
    digitalWrite(ns_led_02, (mBrightness[1]>0)?HIGH:LOW);   // set the LED on
    digitalWrite(ns_led_03, (mBrightness[2]>0)?HIGH:LOW);   // set the LED on
    digitalWrite(ns_led_04, (mBrightness[3]>0)?HIGH:LOW);   // set the LED on
    digitalWrite(ns_led_05, (mBrightness[4]>0)?HIGH:LOW);   // set the LED on
    digitalWrite(ns_led_06, (mBrightness[5]>0)?HIGH:LOW);   // set the LED on
    digitalWrite(ns_led_07, (mBrightness[6]>0)?HIGH:LOW);   // set the LED on

      /*
    unsigned long thisTime = millis();
    if( thisTime-mLastUpdate>500)
        {
        digitalWrite(ns_led_01, (mCount==1)?HIGH:LOW);   // set the LED on
        digitalWrite(ns_led_02, (mCount==2)?HIGH:LOW);   // set the LED on
        digitalWrite(ns_led_03, (mCount==3)?HIGH:LOW);   // set the LED on
        digitalWrite(ns_led_04, (mCount==4)?HIGH:LOW);   // set the LED on
        digitalWrite(ns_led_05, (mCount==5)?HIGH:LOW);   // set the LED on
        digitalWrite(ns_led_06, (mCount==6)?HIGH:LOW);   // set the LED on
        digitalWrite(ns_led_07, (mCount==7)?HIGH:LOW);   // set the LED on


        if(mCount>7) 
        {
            float i;
            i=(mCount-8)/17.0;
            i*=256.0 * 0.25;

            digitalWrite(ns_led_04, LOW);   
            digitalWrite(ns_led_05, LOW);   
            
            analogWrite(ns_led_01,i);
            analogWrite(ns_led_02,i);
            analogWrite(ns_led_06,i);
            analogWrite(ns_led_07,i);
        }

        mLastUpdate = millis();
        mCount++;

        if(mCount>27)
          { 
            mCount=1;
            analogWrite(ns_led_01,0);
            analogWrite(ns_led_02,0);
            analogWrite(ns_led_06,0);
            analogWrite(ns_led_07,0);

          }
        }*/
    }
