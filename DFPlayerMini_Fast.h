#include "Arduino.h"

#ifndef DFPlayerMini_Fast_cpp

  #define DFPlayerMini_Fast_cpp

  #define DFPLAYER_SEND_LENGTH 10
  #define SB 0x7E //start byte
  #define VER 0xFF //version
  #define LEN 0x6 //number of bytes after "LEN" (except for checksum data and EB)
  #define FEEDBACK 0 //no feedback requested
  #define EB 0xEF //end byte

  #define PLAY_COMMAND 0x03
  #define VOLUME_COMMAND 0x06
  #define EQ_COMMAND 0x07
  #define LOOP_COMMAND 0x08
  #define RESUME_COMMAND 0x0D
  #define PAUSE_COMMAND 0x0E

  #define EQ_NORMAL 0
  #define EQ_POP 1
  #define EQ_ROCK 2
  #define EQ_JAZZ 3
  #define EQ_CLASSIC 4
  #define EQ_Base 5

  class DFPlayerMini_Fast
  {
    public:
      Stream* _serial;
    
      uint8_t _sending[DFPLAYER_SEND_LENGTH] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
      uint8_t commandValue = 0;
      uint8_t paramMSB = 0;
      uint8_t paramLSB = 0;
      uint8_t checksumMSB = 0;
      uint8_t checksumLSB = 0;

      void begin(Stream& stream);
      void findChecksum();
      void volume(uint8_t volume);
      void loop(uint8_t fileNum);
      void play(uint8_t fileNum);
      void EQSelect(uint8_t setting);
      void pause();
      void resume();
      void sendData();
  };

#endif
