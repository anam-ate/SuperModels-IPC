
//=============================================================LIBRARIES==================================================================

#include "sensor.h"
#include "button_Trigger.h"
#include <Wire.h>
#include <SPI.h>
#include "mainlights.h"

#include "oldChapelLights.h"

#define DEBUG


//=============================SEQUENCE STUFF ======================================================
int state = 0;          // keeps track of what part of the sequence is currently running
// 0 - waiting for sequence to begin
// 1 - wait a few seconds
// 2- play church sound and lights
// 3 - end sequence


long timer = 0;         // use this to store times for timers
long timer2 = 0;         // use this to store times for timers

int buttonWait = 5;     // enables the sensor for 5 seconds after button was pressed
int initialWait = 2;    // wait 2 seconds in the beginning after trigger before opening the model
int waitTime = 5;       // time in seconds to wait in between opening and closing sequence for test
unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
bool buttonPushed = false;
int chapelState = 0;
int bellsSoundDuration = 7;

int sensorState = 0;

//Lights(int _numberofPixels, int _dataPinLight, int _rVal, int _gVal, int _bVal, int _wVal, int _pixelfadeSpeed)

inline void debugPrint(const char* str)
{
#ifdef DEBUG
    Serial.println(F(str));
#endif
}

inline void debugPrint(int i)
{
#ifdef DEBUG
    Serial.println(i);
#endif
}

inline void debugPrint(float f)
{
#ifdef DEBUG
    Serial.println(f);
#endif
}

inline void wireTransfer(byte b)
{
    Wire.beginTransmission(20);
    Wire.write(b);
    Wire.endTransmission();
}

void setup()
{
    Wire.begin();
    Serial.begin(115200);
    setupDistanceSensor();
    buttonTriggerSetup();
    main_lightSetup();
    main_lightSetup2();
    debugPrint("Ready...");
    debugPrint("Ready to Reset");
}

void loop()
{
    //
    // State 0 - Wait for sensor to trigger
    // when i triggers, set the bird starting time
    if (state == 0)
    {

        if (buttonTriggerSensor() == true)
        {
            buttonPushed = true;
            debugPrint("button pushed");

        }
        // start main sequence
        else if (buttonPushed == true && chapelState == 0 && sensorState == 0)
        {
            //if timer has run out, then do the following...
            buttonPushed = false;
            debugPrint("button pushed timer run out on chapel ");
            debugPrint("Starting sequence 1");
            state = 1;

        }

        else if (chapelState == 0 && buttonTriggerSensor()  == false) // if the bells haven't started and the sensor reads
        {
            mainlightsStart2();
            chapelState = 1;
            debugPrint("start default chapel mode");

        }

        else if (chapelState == 1) // else if timer run out and bellstate == 1 ...
        {
            if (buttonPushed == true)
            {
                debugPrint("get out and go to main sequence");
                bool chapelLightsDone = main_lightSequence2();
                state = 1;
                chapelState = 0;
            }
            else
            {

                bool chapelLightsDone = main_lightSequence2();
                //SENSOR MODE
                if (readDistance() < 50 && sensorState == 0) // IF SENSOR TRIGGERED WHILST CHAPEL LIGHTS ON PLAY CHAPEL SOUND
                {
                    wireTransfer(3);      // play bell track
                    debugPrint("chapel sound playing");
                    timer = millis();
                    sensorState = 1;
                }
                else if (sensorState == 1 && millis() >= timer + 10 * 1000) // IF 10 SECONDS IS UP STOP PLAYING CHAPEL SOUND
                {

                    debugPrint("chapel sound finished");
                    sensorState = 0;
                    state = 0;
                    chapelState = 0;
                }
                else if (sensorState == 1 && millis() <= timer + 10 * 1000 && buttonTriggerSensor() == true) //IF 10 SECONDS NOT UP BUT BUTTON TRIGGERED END SOUND AND CHAPEL PLAY LIGHTS
                {

                    debugPrint(" chapel sound finished");
                    state = 1;
                    sensorState = 0;
                    chapelState = 0;
                }
                if (chapelLightsDone == true) // IF CHAPEL
                {
                    debugPrint("play default chapel mode");
                    chapelState = 0;
                    state = 0;
                    chapelState = 0;
                }
            }
        }
        else
        {
            chapelState = 0;
        }
    }
    else if (state == 1 )
    {
        wireTransfer(1);      // play bird track
        debugPrint("sent church to play");
        debugPrint("starting lights");
        state = 2;

    }

    else if (state == 2)
    {
        mainlightsStart();
        debugPrint("start light sequence");
        state = 3;
    }
    else if (state == 3)
    {

        bool mainLightsDone = main_lightSequence();

        //Serial.println(" begin sun light");
        //Serial.println(millis());

        if (mainLightsDone == true )
        {
            debugPrint("morning & evening sun and light path done");
            debugPrint("Starting sequence 0");
            debugPrint("end church sound");
            wireTransfer(2);  // send trigger to stop bird track
            timer = millis();
            //reset the arduino
            debugPrint("resetting");
            //__DSB;
            //SCB->AIRCR = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk);//software reset
            //RSTC->RSTC_CR = RSTC_CR_KEY(0xA5) | RSTC_CR_PERRST | RSTC_CR_PROCRST;
            //NVIC_SystemReset();
            state = 0;


        }
    }
    else
    {
        state = 0;
    }
}
