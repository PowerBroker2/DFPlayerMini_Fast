#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>


SoftwareSerial mySerial(10, 11); // RX, TX
DFPlayerMini_Fast myDFPlayer;


void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);
  
  myDFPlayer.begin(mySerial);
}


void loop()
{
  myDFPlayer.play(1); //play mp3 file with leading identifier "0001"

  myDFPlayer.volume(1); //set volume low

  delay(100); //wait 100ms

  myDFPlayer.volume(30); //crank that stuff!

  delay(100); //wait another 100ms

  myDFPlayer.EQSelect(1); //set EQ as pop

  delay(100); //wait another 100ms

  myDFPlayer.EQSelect(5); //set EQ as bass

  delay(100); //wait another 100ms

  myDFPlayer.pause(); //pause player

  delay(100); //wait another 100ms

  myDFPlayer.resume(); //resume player

  delay(100); //wait another 100ms

  myDFPlayer.loop(2); //loop mp3 file with leading identifier "0002"

  while(1); //halt
}
