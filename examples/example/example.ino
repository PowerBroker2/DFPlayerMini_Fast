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
  myMP3.begin(mySerial);
#else
  Serial1.begin(9600);
  myMP3.begin(Serial1);
#endif
  
  Serial.println("Setting volume to max");
  myMP3.volume(30);
  delay(20);
  
  Serial.println("Playing track 1 for 5 sec");
  myMP3.play(1);
  delay(5000);

  Serial.println("Sleeping for 5 sec");
  myMP3.sleep();
  delay(5000);

  Serial.println("Waking up");
  myMP3.wakeUp();

  Serial.println("Looping track 1");
  myMP3.loop(1);
}

void loop()
{
  //do nothing
}
