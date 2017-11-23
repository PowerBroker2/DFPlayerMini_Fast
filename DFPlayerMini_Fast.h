#include "Arduino.h"

#ifndef DFPlayerMini_Fast_cpp

  #define DFPlayerMini_Fast_cpp

  #define SB 0x7E //start byte
  #define VER 0xFF //version
  #define LEN 0x6 //number of bytes after "LEN" (except for checksum data and EB)
  #define FEEDBACK 0 //no feedback requested
  #define EB 0xEF //end byte
  #define VOLUME_COMMAND 6
  #define LOOP_COMMAND 8
  #define PLAY_COMMAND 3

  class DFPlayerMini_Fast
  {
    public:
      Stream* _serial;
  
      uint8_t commandValue = 0;
      uint8_t paramMSB = 0;
      uint8_t paramLSB = 0;
      uint8_t checksumMSB = 0;
      uint8_t checksumLSB = 0;

      void begin(Stream& stream);
      void findChecksum(uint8_t commandValue, uint8_t paramMSB, uint8_t paramLSB, uint8_t& checksumMSB, uint8_t& checksumLSB);
      void volume(uint8_t volume, uint8_t& commandValue, uint8_t& paramMSB, uint8_t& paramLSB);
      void loop(uint8_t fileNum, uint8_t& commandValue, uint8_t& paramMSB, uint8_t& paramLSB);
      void play(uint8_t fileNum, uint8_t& commandValue, uint8_t& paramMSB, uint8_t& paramLSB);
      void sendData(uint8_t commandValue, uint8_t paramMSB, uint8_t paramLSB, uint8_t checksumMSB, uint8_t checksumLSB);
  };

#endif
