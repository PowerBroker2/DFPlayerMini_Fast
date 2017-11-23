#include "DFPlayerMini_Fast.h"


void DFPlayerMini_Fast::begin(Stream &stream)
{
  _serial = &stream;
  
  return;
}


void DFPlayerMini_Fast::findChecksum(uint8_t commandValue, uint8_t paramMSB, uint8_t paramLSB, uint8_t& checksumMSB, uint8_t& checksumLSB)
{
  uint16_t checksum = (~(VER + LEN + commandValue + FEEDBACK + paramMSB + paramLSB)) + 1;

  checksumMSB = checksum >> 8;
  checksumLSB = checksum & 0xFF;
  
  return;
}


void DFPlayerMini_Fast::volume(uint8_t _sending[], uint8_t volume, uint8_t& commandValue, uint8_t& paramMSB, uint8_t& paramLSB)
{
  commandValue = VOLUME_COMMAND;
  paramMSB = 0;
  paramLSB = volume;
  
  findChecksum(commandValue, paramMSB, paramLSB, checksumMSB, checksumLSB);
  sendData(_sending[], commandValue, paramMSB, paramLSB, checksumMSB, checksumLSB);
  
  return;
}


void DFPlayerMini_Fast::loop(uint8_t _sending[], uint8_t fileNum, uint8_t& commandValue, uint8_t& paramMSB, uint8_t& paramLSB)
{
  commandValue = LOOP_COMMAND;
  paramMSB = 0;
  paramLSB = fileNum;
  
  findChecksum(commandValue, paramMSB, paramLSB, checksumMSB, checksumLSB);
  sendData(_sending[], commandValue, paramMSB, paramLSB, checksumMSB, checksumLSB);
  
  return;
}


void DFPlayerMini_Fast::play(uint8_t _sending[], uint8_t fileNum, uint8_t& commandValue, uint8_t& paramMSB, uint8_t& paramLSB)
{
  commandValue = PLAY_COMMAND;
  paramMSB = 0;
  paramLSB = fileNum;
  
  findChecksum(commandValue, paramMSB, paramLSB, checksumMSB, checksumLSB);
  sendData(_sending[], commandValue, paramMSB, paramLSB, checksumMSB, checksumLSB);
  
  return;
}


void DFPlayerMini_Fast::sendData(uint8_t _sending[], uint8_t commandValue, uint8_t paramMSB, uint8_t paramLSB, uint8_t checksumMSB, uint8_t checksumLSB)
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
