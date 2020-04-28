//=============================================================LIBRARIES==================================================================
#include <SdFat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Adafruit_NeoPixel.h>


SdFat sd2;
SdFile file2;
//=========================================================PIN FOR SD CARD READING=======================================================================================
const uint8_t chipSelect2 = 30; // PIN TO READ SD CARD 


int framerate2 = 25;     
int frame_duration2 = 1000 / framerate; // 1 SECOND- EVERY 1 SECOND UPDATE THE VALUES BEING READ FROM THE SD CARD
int nextFrame2; // CURRENT TIME + 1 SECOND IS THE TIME WHEN THE NEXT FRAME IS GOING TO START

const char s2 = ',';  // SPLIT THE STRING OF CHARACTERS WHERE THE COMMAS ARE

long previousMillis2;
//=======================================================LED PARAMETERS==========================================================================
#define NUM_LEDS260  60       // number of pixels

#define DATA_PIN2Sun1    8  //25
#define DATA_PIN2Sun2    9  //23
#define DATA_PIN2Sky1    32 //24
#define DATA_PIN2SunSpotDay    34 //24
#define DATA_PIN2SunSpoteEve   36 //24
#define DATA_PIN2CreepyMan 26
char ledData2[NUM_LEDS260 *20]; // create an array of 60 LEDS x 4 rgbw 

Adafruit_NeoPixel strip1Sun2 = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSun1, NEO_GRBW + NEO_KHZ800); // Sun shader :Strip 1 and 2 will have the same values , so no need to send data twice from cinema
Adafruit_NeoPixel strip2Sun2 = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSun2, NEO_GRBW + NEO_KHZ800); // Sun shader :Strip 1 and 2 will have the same values , so no need to send data twice from cinema
Adafruit_NeoPixel strip1Sky2 = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSky1, NEO_GRBW + NEO_KHZ800); // Sun shader :Str
Adafruit_NeoPixel strip1creepyMan2 = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINCreepyMan, NEO_GRBW + NEO_KHZ800); // Sun shader :Str
Adafruit_NeoPixel strip1SunSpotDay2 = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSunSpotDay, NEO_GRBW + NEO_KHZ800); // Sun shader :Str
Adafruit_NeoPixel strip1SunSpotEve2 = Adafruit_NeoPixel(NUM_LEDS60, DATA_PINSunSpoteEve, NEO_GRBW + NEO_KHZ800); // Sun shader :Str

uint8_t globalBrightnessSun2 = 253;
uint8_t globalBrightnessSky2 = 253;


//========================================================SD CARD CSV DATA LINE LENGTHS=============================================================================
// Maximum line length plus space for zero byte.
const size_t LINE_DIM2 = 2500; //667;
char line2[LINE_DIM2]; // defining a character array that is 667 characters long to store 1 frame of rgbw values
size_t n2;

//=============================================================================================================================================

//=================================================TIME PARAMETERS FOR FILE==========================================================================================================

bool mainlightsTrigger2 = false;   // boolean for turning bird sound on 

bool mainlightsplaying2 = false;
long mainlightsStartTime2 = 0;

void mainlightsStart2() 
{
  mainlightsStartTime2 = millis();
}

long mainlightsMillis2() 
{
  return millis() - mainlightsStartTime2;
}


void main_lightSetup2() 
{
  mainlightsStartTime2 = 0;
    //SUN 1 STRIP
  strip1Sun2.begin(); // Setup LED strip and set brightness
  strip1Sun2.show(); // Initialize all pixels to 'off'
  strip1Sun2.setBrightness(globalBrightnessSun2);


  //SUN 2 STRIP
  strip2Sun2.begin(); // Setup LED strip and set brightness
  strip2Sun2.show(); // Initialize all pixels to 'off'
  strip2Sun2.setBrightness(globalBrightnessSun2);

  //SKY 1 STRIP 
  strip1Sky2.begin(); // Setup LED strip and set brightness
  strip1Sky2.show(); // Initialize all pixels to 'off'
  strip1Sky2.setBrightness(globalBrightnessSky2);

  // CREEPY MAN STRIP 
  strip1creepyMan2.begin(); // Setup LED strip and set brightness
  strip1creepyMan2.show(); // Initialize all pixels to 'off'
  strip1creepyMan2.setBrightness(globalBrightnessSun2);

   //SUN SPOT DAY STRIP 
  strip1SunSpotDay2.begin(); // Setup LED strip and set brightness
  strip1SunSpotDay2.show(); // Initialize all pixels to 'off'
  strip1SunSpotDay2.setBrightness(globalBrightnessSun2);

   //SUN SPOT EVE STRIP 
  strip1SunSpotEve2.begin(); // Setup LED strip and set brightness
  strip1SunSpotEve2.show(); // Initialize all pixels to 'off'
  strip1SunSpotEve2.setBrightness(globalBrightnessSun);

  if (!sd2.begin(chipSelect2, SPI_HALF_SPEED)) sd.initErrorHalt();
  
  if (!file2.open("ipcContentChapel001.csv", O_READ)); // OPEN THE CSV FILE WITH THE CINEMA 4D LIGHT DATA

}


bool main_lightSequence2() 
{
  //Serial.println("begin lights");
  mainlightsStartTime2 = 0;
  long t2 = mainlightsMillis2();
  if (t2 >= nextFrame2)
  {
   
    nextFrame2 = t2 + frame_duration2;
    n2 = file2.fgets(line2, sizeof(line2)); //read a new line from the csv
    
    if(n2>0)
      {

        char *token2; // declare a char pointer called token
        token2 = strtok(line2, &s2); //go through every element in the string of characters and get rid of ","        
        int a2 = 0;
        while( token2 != NULL )  //if the character is not a null object  then
        {    
          
          ledData2[a2] = atoi(token2);  // for each LED pixel convert the Cchar value to an int #
          
          a2++; // do for all in the char array(string)
          token2 = strtok(NULL, &s2); 
        }

     
     int i = 0;
     for(i = 0; i< NUM_LEDS260; i++) // 103 LEDS
     {
       
        uint8_t r = ledData2[20*i + 0]; // go to the first element of the array,then count 4 elements up and so on
        uint8_t r2 = ledData2[20*i + 1]; // go to second element of the array,then count 4 elements up and so on
        uint8_t r3 = ledData2[20*i + 2]; // go to second element of the array,then count 4 elements up and so on
        uint8_t r4 = ledData2[20*i + 3]; // go to second element of the array,then count 4 elements up and so on
        uint8_t r5 = ledData2[20*i + 4]; // go to second element of the array,then count 4 elements up and so on
        
        uint8_t g = ledData2[20*i + 5]; // got the 3rd element of the array,then count 4 elements up and so on
        uint8_t g2 = ledData2[20*i + 6]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t g3 = ledData2[20*i + 7]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t g4 = ledData2[20*i + 8]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t g5 = ledData2[20*i + 9]; // got to 4th element of the array,then count 4 elements up and so on
        
        uint8_t b = ledData2[20*i + 10]; // go to the first element of the array,then count 4 elements up and so on
        uint8_t b2 = ledData2[20*i + 11]; // go to second element of the array,then count 4 elements up and so on
        uint8_t b3 = ledData2[20*i + 12]; // go to second element of the array,then count 4 elements up and so on
        uint8_t b4 = ledData2[20*i + 13]; // go to second element of the array,then count 4 elements up and so on
        uint8_t b5 = ledData2[20*i + 14]; // go to second element of the array,then count 4 elements up and so on
        
        uint8_t w = ledData2[20*i + 15]; // got the 3rd element of the array,then count 4 elements up and so on
        uint8_t w2 = ledData2[20*i + 16]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t w3 = ledData2[20*i + 17]; // got to 4th element of the array,then count 4 elements up and so on
        uint8_t w4 = ledData2[20*i + 18]; // got to 4th element of the array,then count 4 elements up and so 
        uint8_t w5 = ledData2[20*i + 19]; // got to 4th element of the array,then count 4 elements up and so 
        
   
        strip1Sky2.setPixelColor(i,strip1Sky2.Color(r2,g2,b2,w2));
     
        strip1creepyMan2.setPixelColor(i,strip1creepyMan2.Color(r3,g3,b3,w3));
        strip1Sun2.setPixelColor(i,strip1Sun2.Color(r,g,b,w));
        strip2Sun2.setPixelColor(i,strip2Sun2.Color(r,g,b,w));  
        strip1SunSpotDay2.setPixelColor(i,strip1SunSpotDay2.Color(r4,g4,b4,w4));
        strip1SunSpotEve2.setPixelColor(i,strip1SunSpotEve2.Color(r5,g5,b5,w5));
     }

     strip1Sun2.show(); 
     strip2Sun2.show();
     strip1Sky2.show();
     strip1creepyMan2.show();
     strip1SunSpotDay2.show();
     strip1SunSpotEve2.show();
    }
    else
    {
      //Serial.println("end of sequence");
      file2.rewind();
      mainlightsTrigger2 = false;
      return true;
    }
    
  } 
  return false;  
}
