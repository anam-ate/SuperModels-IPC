#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <FreeStack.h> 
#include <SFEMP3Shield.h>

//#define DEBUG

// mp3 variables
SFEMP3Shield MP3player;
// sd card instantiation
SdFat sd;
 //SOFTWARE RESET ARDUINO
void(* resetFunc) (void) = 0; //declare reset function @ address 0
// ==========================================church song stuff==============================================================================
#define Churchsong 0 //
int durationTrackChurch = 347;  //duration of bird sound
bool churchSoundTrigger1 = false;   // boolean for turning bird sound on 
bool churchSoundTrigger2 = false;  // boolean for turning bird sound off
long church1startTime;  // start millis() for sequence 
bool churchplaying = false;

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

// restart the time for the bird sequence
void SoundChurchStart() 
{
  church1startTime = millis();
}

long SoundChurch1Millis() 
{
  return millis() - church1startTime;
}
// ==========================================churchbells song stuff==============================================================================
#define Bellsong 2 // bird content
int durationTrackBell = 10;  //duration of bird sound
bool bellSoundTrigger1 = false;   // boolean for turning bird sound on 
bool bellSoundTrigger2 = false;  // boolean for turning bird sound off
long bellstartTime = 0;  // start millis() for sequence 
bool bellplaying = false;
// restart the time for the bird sequence
void SoundBellStart() 
{
  bellstartTime = millis();
}

long SoundBellMillis() 
{
  return millis() - bellstartTime;
}

//=========================================== main setup ===================================================================================



void setup() 
{
  Wire.begin(20);                // join i2c bus with address 
  Wire.onReceive(receiveEvent); // register event from Due to Touchboard receive trigger data
  Serial.begin(115200);           // start serial for output
  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt(); // sound stuff 
  MP3player.begin();   
  MP3player.setVolume(10,10); // volume control for later fade this up and down 
  church1startTime = 0; // reset the start time
  bellstartTime = 0;
}

//===================================main trigger loop for sounds ==============================================================
void loop() 
{
 

}

//receive data from the due, to trigger the mp3 files
void receiveEvent(int howMany)   // special event function for wire 
{
  while (1 < Wire.available()) 
  { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
  }
  int x = Wire.read();    // receive byte as an integer
  debugPrint("Received Trigger: ");
  debugPrint(x);         // print the integer
  
  // trigger church song from Due==================================================================
  if (x == 1)
  {
    MP3player.stopTrack();
    delay(5);
    MP3player.playTrack(Churchsong); 
    debugPrint("X is 1");
    debugPrint("sent church to play");

  }
  
  //stop church song from Due
  if (x == 2)
  {
    MP3player.stopTrack();
    debugPrint("X is 2");
  
  }

  // trigger bell song from Due==================================================================
  if (x == 3)
  {
    MP3player.playTrack(Bellsong);
    debugPrint("X is 3");
   

  }

  
  //stop bell song from Due
  /*if (x == 4)
  {
    Serial.println("X is 4");
    bellSoundTrigger2 = true; 
  }*/

}
