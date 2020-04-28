//=============================================================LIBRARIES==================================================================
#include <SdFat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Adafruit_NeoPixel.h>


SdFat sd;
SdFile file;
//=========================================================PIN FOR SD CARD READING=======================================================================================
const uint8_t chipSelect = 30; // PIN TO READ SD CARD 


int incomingByte = 0; // for incoming serial data

int framerate = 25;     
int frame_duration = 1000 / framerate; // 1 SECOND- EVERY 1 SECOND UPDATE THE VALUES BEING READ FROM THE SD CARD
int nextFrame; // CURRENT TIME + 1 SECOND IS THE TIME WHEN THE NEXT FRAME IS GOING TO START

const char s = ',';  // SPLIT THE STRING OF CHARACTERS WHERE THE COMMAS ARE

long previousMillis;
//=======================================================LED PARAMETERS==========================================================================
#define NUM_LEDS60  60       // number of pixels

#define DATA_PINSun1    8  //25 //8
#define DATA_PINSun2    9  //23
#define DATA_PINSky1    32 //24
#define DATA_PINSunSpotDay    34 //24
#define DATA_PINSunSpoteEve   36 //24
#define DATA_PINCreepyMan 26
char ledData[NUM_LEDS60 *20]; // create an array of 60 LEDS x 4 rgbw 

Adafruit_NeoPixel strip1Sun = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSun1, NEO_GRBW + NEO_KHZ800); // Sun shader :Strip 1 and 2 will have the same values , so no need to send data twice from cinema
Adafruit_NeoPixel strip2Sun = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSun2, NEO_GRBW + NEO_KHZ800); // Sun shader :Strip 1 and 2 will have the same values , so no need to send data twice from cinema
Adafruit_NeoPixel strip1Sky = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSky1, NEO_GRBW + NEO_KHZ800); // Sun shader :Str
Adafruit_NeoPixel strip1creepyMan = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINCreepyMan, NEO_GRBW + NEO_KHZ800); // Sun shader :Str
Adafruit_NeoPixel strip1SunSpotDay = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSunSpotDay, NEO_GRBW + NEO_KHZ800); // Sun shader :Str
Adafruit_NeoPixel strip1SunSpotEve = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSunSpoteEve, NEO_GRBW + NEO_KHZ800); // Sun shader :Str

uint8_t globalBrightnessSun = 253;
uint8_t globalBrightnessSky = 253;


//========================================================SD CARD CSV DATA LINE LENGTHS=============================================================================
// Maximum line length plus space for zero byte.
const size_t LINE_DIM = 2700; //667;
char line[LINE_DIM]; // defining a character array that is 667 characters long to store 1 frame of rgbw values
size_t n;

//=============================================================================================================================================

//=================================================TIME PARAMETERS FOR FILE==========================================================================================================

bool mainlightsTrigger1 = false;   // boolean for turning bird sound on 

bool mainlightsplaying = false;
long mainlightsStartTime = 0;

void mainlightsStart() 
{
  mainlightsStartTime = millis();
}

long mainlightsMillis() 
{
  return millis() - mainlightsStartTime;
}
int ledDataIndex = 0;

void resetIdCounter()  // set counter to 0
{
  ledDataIndex = 0;
}

void main_lightSetup() 
{
  mainlightsStartTime = 0;
    //SUN 1 STRIP
  strip1Sun.begin(); // Setup LED strip and set brightness
  strip1Sun.show(); // Initialize all pixels to 'off'
  strip1Sun.setBrightness(globalBrightnessSun);


  //SUN 2 STRIP
  strip2Sun.begin(); // Setup LED strip and set brightness
  strip2Sun.show(); // Initialize all pixels to 'off'
  strip2Sun.setBrightness(globalBrightnessSun);

  //SKY 1 STRIP 
  strip1Sky.begin(); // Setup LED strip and set brightness
  strip1Sky.show(); // Initialize all pixels to 'off'
  strip1Sky.setBrightness(globalBrightnessSky);

  // CREEPY MAN STRIP 
  strip1creepyMan.begin(); // Setup LED strip and set brightness
  strip1creepyMan.show(); // Initialize all pixels to 'off'
  strip1creepyMan.setBrightness(globalBrightnessSun);

   //SUN SPOT DAY STRIP 
  strip1SunSpotDay.begin(); // Setup LED strip and set brightness
  strip1SunSpotDay.show(); // Initialize all pixels to 'off'
  strip1SunSpotDay.setBrightness(globalBrightnessSun);

   //SUN SPOT EVE STRIP 
  strip1SunSpotEve.begin(); // Setup LED strip and set brightness
  strip1SunSpotEve.show(); // Initialize all pixels to 'off'
  strip1SunSpotEve.setBrightness(globalBrightnessSun);

  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();
  
  if (!file.open("contentIPC21.hex", O_READ)); // OPEN THE CSV FILE WITH THE CINEMA 4D LIGHT DATA

}


bool main_lightSequence() 
{
  //Serial.println("begin lights");
  mainlightsStartTime = 0;
  long t = mainlightsMillis();

  if (t >= nextFrame)
  {
    nextFrame = t + frame_duration;

    while (true)
    {
      int incomingByte = file.read();
      if ( incomingByte == 255) // break if the int is 255, signals the start of new frame 
      {
        break;
      }
    }
    // store the incoming int as an array of 240 ints 60leds x4 rgbw
    int j = 0;
    for (j = 0; j < NUM_LEDS60 * 20; j++)
    {
      incomingByte = file.read();// read data from SD
      ledData[j] = incomingByte;
    }
     
     int i = 0;
     for(i = 0; i< NUM_LEDS60; i++) // - LEDS
     {
       
        uint8_t r = ledData[20*i + 0]; // go to the first element of the array,then count 4 elements up and so on
        uint8_t r2 = ledData[20*i + 1]; // go to second element of the array,then count 4 elements up and so on
        uint8_t r3 = ledData[20*i + 2]; // go to second element of the array,then count 4 elements up and so on
        uint8_t r4 = ledData[20*i + 3]; // go to second element of the array,then count 4 elements up and so on
        uint8_t r5 = ledData[20*i + 4]; // go to second element of the array,then count 4 elements up and so on
        
        uint8_t g = ledData[20*i + 5]; // got the 3rd element of the array,then count 4 elements up and so on
        uint8_t g2 = ledData[20*i + 6]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t g3 = ledData[20*i + 7]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t g4 = ledData[20*i + 8]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t g5 = ledData[20*i + 9]; // got to 4th element of the array,then count 4 elements up and so on
        
        uint8_t b = ledData[20*i + 10]; // go to the first element of the array,then count 4 elements up and so on
        uint8_t b2 = ledData[20*i + 11]; // go to second element of the array,then count 4 elements up and so on
        uint8_t b3 = ledData[20*i + 12]; // go to second element of the array,then count 4 elements up and so on
        uint8_t b4 = ledData[20*i + 13]; // go to second element of the array,then count 4 elements up and so on
        uint8_t b5 = ledData[20*i + 14]; // go to second element of the array,then count 4 elements up and so on
        
        uint8_t w = ledData[20*i + 15]; // got the 3rd element of the array,then count 4 elements up and so on
        uint8_t w2 = ledData[20*i + 16]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t w3 = ledData[20*i + 17]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t w4 = ledData[20*i + 18]; // got to 4th element of the array,then count 4 elements up and so 
        uint8_t w5 = ledData[20*i + 19]; // got to 4th element of the array,then count 4 elements up and so 0


        strip1Sky.setPixelColor(i,strip1Sky.Color(r2,g2,b2,w2));
     
        strip1creepyMan.setPixelColor(i,strip1creepyMan.Color(r3,g3,b3,w3));
        strip1Sun.setPixelColor(i,strip1Sun.Color(r,g,b,w));
        strip2Sun.setPixelColor(i,strip2Sun.Color(r,g,b,w));  
        strip1SunSpotDay.setPixelColor(i,strip1SunSpotDay.Color(r4,g4,b4,w4));
        strip1SunSpotEve.setPixelColor(i,strip1SunSpotEve.Color(r5,g5,b5,w5));
        /*
        strip1Sky.setPixelColor(i,strip1Sky.Color( pgm_read_byte(&gamma8[r2]), pgm_read_byte(&gamma8[g2]), pgm_read_byte(&gamma8[b2]), pgm_read_byte(&gamma8[w2])));
     
        strip1creepyMan.setPixelColor(i,strip1creepyMan.Color(pgm_read_byte(&gamma8[r3]),pgm_read_byte(&gamma8[g3]),pgm_read_byte(&gamma8[b3]),pgm_read_byte(&gamma8[w3])));
        strip1Sun.setPixelColor(i,strip1Sun.Color(pgm_read_byte(&gamma8[r]),pgm_read_byte(&gamma8[g]),pgm_read_byte(&gamma8[b]),pgm_read_byte(&gamma8[w])));
        strip2Sun.setPixelColor(i,strip2Sun.Color(pgm_read_byte(&gamma8[r]),pgm_read_byte(&gamma8[g]),pgm_read_byte(&gamma8[b]),pgm_read_byte(&gamma8[w])));  
        strip1SunSpotDay.setPixelColor(i,strip1SunSpotDay.Color(pgm_read_byte(&gamma8[r4]),pgm_read_byte(&gamma8[g4]),pgm_read_byte(&gamma8[b4]),pgm_read_byte(&gamma8[w4])));
        strip1SunSpotEve.setPixelColor(i,strip1SunSpotEve.Color(pgm_read_byte(&gamma8[r5]),pgm_read_byte(&gamma8[g5]),pgm_read_byte(&gamma8[b5]),pgm_read_byte(&gamma8[w5])));
        */
      
     }

     strip1Sun.show(); 
     strip2Sun.show();
     strip1Sky.show();
     strip1creepyMan.show();
     strip1SunSpotDay.show();
     strip1SunSpotEve.show();
    }
    else
  {
    //Serial.println("new frame");
    resetIdCounter(); // reset the array to 0 to clear it for the new frame
  }
    
   // check if we've reached then end, i.e. if there is nothing more to read
  if (file.available() == false)
  {
    //Serial.println("end of sequence");
    file.rewind();
    return true;
  }
  return false;
}
