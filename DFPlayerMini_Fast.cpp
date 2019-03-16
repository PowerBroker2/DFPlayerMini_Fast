#include "DFPlayerMini_Fast.h"


void DFPlayerMini_Fast::begin(Stream &stream)
{
  _serial = &stream;
  
  return;
}


void DFPlayerMini_Fast::findChecksum()
{
  uint16_t checksum = (~(VER + LEN + commandValue + FEEDBACK + paramMSB + paramLSB)) + 1;

  checksumMSB = checksum >> 8;
  checksumLSB = checksum & 0xFF;
  
  return;
}


void DFPlayerMini_Fast::volume(uint8_t volume)
{
  if(volume <= 30)
  {
    commandValue = VOLUME_COMMAND;
    paramMSB = 0;
    paramLSB = volume;
    
    findChecksum();
    sendData();
  }
  
  return;
}


void DFPlayerMini_Fast::loop(uint16_t fileNum)
{
  commandValue = LOOP_COMMAND;
  paramMSB = (fileNum >> 8) & 0xFF;
  paramLSB = fileNum & 0xFF;
  
  findChecksum();
  sendData();
  
  return;
}


void DFPlayerMini_Fast::play(uint16_t fileNum)
{
  commandValue = PLAY_COMMAND;
  paramMSB = (fileNum >> 8) & 0xFF;
  paramLSB = fileNum & 0xFF;
  
  findChecksum();
  sendData();
  
  return;
}


void DFPlayerMini_Fast::EQSelect(uint8_t setting)
{
  commandValue = EQ_COMMAND;
  paramMSB = 0;
  paramLSB = setting;
  
  findChecksum();
  sendData();
  
  return;
}
  

void DFPlayerMini_Fast::pause()
{
  commandValue = PAUSE_COMMAND;
  paramMSB = 0;
  paramLSB = 0;
  
  findChecksum();
  sendData();
  
  return;
}
  

void DFPlayerMini_Fast::resume()
{
  commandValue = RESUME_COMMAND;
  paramMSB = 0;
  paramLSB = 0;
  
  findChecksum();
  sendData();
  
  return;
}


void DFPlayerMini_Fast::sendData()
{
  _sending[0] = SB;
  _sending[1] = VER;
  _sending[2] = LEN;
  _sending[3] = commandValue;
  _sending[4] = FEEDBACK;
  _sending[5] = paramMSB;
  _sending[6] = paramLSB;
  _sending[7] = checksumMSB;
  _sending[8] = checksumLSB;
  _sending[9] = EB;
  
  _serial->write(_sending, DFPLAYER_SEND_LENGTH);
  
  return;
}
