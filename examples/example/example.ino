#include <DFPlayerMini_Fast.h>

#if !defined(UBRR1H)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#endif

DFPlayerMini_Fast myMP3;

void setup()
{
  Serial.begin(115200);

#if !defined(UBRR1H)
  mySerial.begin(9600);
  myMP3.begin(mySerial, true);
#else
  Serial1.begin(9600);
  myMP3.begin(Serial1, true);
#endif
  
  delay(1000);
  
  Serial.println("Setting volume to max");
  myMP3.volume(30);
  
  Serial.println("Looping track 1");
  myMP3.loop(1);
}

void loop()
{
  //do nothing
}
