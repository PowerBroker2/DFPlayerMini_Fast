#include <DFPlayerMini_Fast.h>

//create a class instance
DFPlayerMini_Fast myMP3;

void setup()
{
  //initialize serial port
  Serial.begin(9600);
  
  //initialize MP3 player
  myMP3.begin(Serial);
  
  //adjust the voluem
  myMP3.volume(30);
  
  //loop first file in folder - folder must be named "mp3"
  myMP3.loop(1);
}

void loop()
{
  //do nothing
}
