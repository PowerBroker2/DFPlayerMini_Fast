#include "DFPlayerMini_Fast.h"
#include "FireTimer.h"




bool DFPlayerMini_Fast::begin(Stream &stream, unsigned long threshold)
{
	_threshold = threshold;
	timoutTimer.begin(_threshold);

	_serial = &stream;

	sendStack.start_byte = dfplayer::SB;
	sendStack.version    = dfplayer::VER;
	sendStack.length     = dfplayer::LEN;
	sendStack.end_byte   = dfplayer::EB;

	recStack.start_byte  = dfplayer::SB;
	recStack.version     = dfplayer::VER;
	recStack.length      = dfplayer::LEN;
	recStack.end_byte    = dfplayer::EB;

	return true;
}




void DFPlayerMini_Fast::playNext()
{
	sendStack.commandValue  = dfplayer::NEXT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::playPrevious()
{
	sendStack.commandValue  = dfplayer::PREV;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::play(uint16_t trackNum)
{
	sendStack.commandValue  = dfplayer::PLAY;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = (trackNum >> 8) & 0xFF;
	sendStack.paramLSB = trackNum & 0xFF;

	findChecksum(&sendStack);
	sendData();
}


void DFPlayerMini_Fast::stop()
{
	sendStack.commandValue = dfplayer::STOP;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(&sendStack);
	sendData();
}


void DFPlayerMini_Fast::playFromMP3Folder(uint16_t trackNum)
{
	sendStack.commandValue  = dfplayer::USE_MP3_FOLDER;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = (trackNum >> 8) & 0xFF;
	sendStack.paramLSB = trackNum & 0xFF;

	findChecksum(&sendStack);
	sendData();
}


void DFPlayerMini_Fast::playAdvertisement(uint16_t trackNum)
{
	sendStack.commandValue  = dfplayer::INSERT_ADVERT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = (trackNum >> 8) & 0xFF;
	sendStack.paramLSB = trackNum & 0xFF;

	findChecksum(&sendStack);
	sendData();
}


void DFPlayerMini_Fast::stopAdvertisement()
{
	sendStack.commandValue  = dfplayer::STOP_ADVERT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(&sendStack);
	sendData();
}


void DFPlayerMini_Fast::incVolume()
{
	sendStack.commandValue  = dfplayer::INC_VOL;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::decVolume()
{
	sendStack.commandValue  = dfplayer::DEC_VOL;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::volume(uint8_t volume)
{
	if (volume <= 30)
	{
		sendStack.commandValue  = dfplayer::VOLUME;
		sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
		sendStack.paramMSB = 0;
		sendStack.paramLSB = volume;

		findChecksum(&sendStack);
		sendData();
	}
}




void DFPlayerMini_Fast::EQSelect(uint8_t setting)
{
	if (setting <= 5)
	{
		sendStack.commandValue  = dfplayer::EQ;
		sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
		sendStack.paramMSB = 0;
		sendStack.paramLSB = setting;

		findChecksum(&sendStack);
		sendData();
	}
}




void DFPlayerMini_Fast::loop(uint16_t trackNum)
{
  sendStack.commandValue  = dfplayer::PLAYBACK_MODE;
  sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
  sendStack.paramMSB = (trackNum >> 8) & 0xFF;
  sendStack.paramLSB = trackNum & 0xFF;
  
  findChecksum(&sendStack);
  sendData();
}




void DFPlayerMini_Fast::playbackSource(uint8_t source)
{
	if ((source > 0) && (source <= 5))
	{
		sendStack.commandValue  = dfplayer::PLAYBACK_SRC;
		sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
		sendStack.paramMSB = 0;
		sendStack.paramLSB = source;

		findChecksum(&sendStack);
		sendData();
	}
}




void DFPlayerMini_Fast::standbyMode()
{
	sendStack.commandValue  = dfplayer::STANDBY;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::normalMode()
{
	sendStack.commandValue  = dfplayer::NORMAL;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::reset()
{
	sendStack.commandValue  = dfplayer::RESET;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::resume()
{
	sendStack.commandValue  = dfplayer::PLAYBACK;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::pause()
{
	sendStack.commandValue  = dfplayer::PAUSE;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::playFolder(uint8_t folderNum, uint8_t trackNum)
{
	sendStack.commandValue  = dfplayer::SPEC_FOLDER;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = folderNum;
	sendStack.paramLSB = trackNum;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::volumeAdjustSet(uint8_t gain)
{
	if (gain <= 31)
	{
		sendStack.commandValue  = dfplayer::VOL_ADJ;
		sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
		sendStack.paramMSB = 0;
		sendStack.paramLSB = dfplayer::VOL_ADJUST + gain;

		findChecksum(&sendStack);
		sendData();
	}
}




void DFPlayerMini_Fast::startRepeatPlay()
{
	sendStack.commandValue  = dfplayer::REPEAT_PLAY;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = dfplayer::START_REPEAT;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::stopRepeatPlay()
{
	sendStack.commandValue  = dfplayer::REPEAT_PLAY;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = dfplayer::STOP_REPEAT;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::repeatFolder(uint16_t folder)
{
	sendStack.commandValue  = dfplayer::REPEAT_FOLDER;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = (folder >> 8) & 0xFF;
	sendStack.paramLSB = folder & 0xFF;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::randomAll()
{
	sendStack.commandValue  = dfplayer::RANDOM_ALL;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::startRepeat()
{
	sendStack.commandValue  = dfplayer::REPEAT_CURRENT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::stopRepeat()
{
	sendStack.commandValue  = dfplayer::REPEAT_CURRENT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::startDAC()
{
	sendStack.commandValue  = dfplayer::SET_DAC;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::stopDAC()
{
	sendStack.commandValue  = dfplayer::SET_DAC;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(&sendStack);
	sendData();
}




void DFPlayerMini_Fast::sleep()
{
	playbackSource(dfplayer::SLEEP);
}




void DFPlayerMini_Fast::wakeUp()
{
	playbackSource(dfplayer::TF);

	delay(100);
}




bool DFPlayerMini_Fast::isPlaying()
{
	int16_t result = query(dfplayer::GET_STATUS_);

	if (result != -1)
		return (result & 1);

	return result;
}




int16_t DFPlayerMini_Fast::currentVolume()
{
	return query(dfplayer::GET_VOL);
}




int16_t DFPlayerMini_Fast::currentEQ()
{
	return query(dfplayer::GET_EQ);
}




int16_t DFPlayerMini_Fast::currentMode()
{
	return query(dfplayer::GET_MODE);
}




int16_t DFPlayerMini_Fast::currentVersion()
{
	return query(dfplayer::GET_VERSION);
}




int16_t DFPlayerMini_Fast::numUsbTracks()
{
	return query(dfplayer::GET_TF_FILES);
}




int16_t DFPlayerMini_Fast::numSdTracks()
{
	return query(dfplayer::GET_U_FILES);
}




int16_t DFPlayerMini_Fast::numFlashTracks()
{
	return query(dfplayer::GET_FLASH_FILES);
}




int16_t DFPlayerMini_Fast::currentUsbTrack()
{
	return query(dfplayer::GET_TF_TRACK);
}




int16_t DFPlayerMini_Fast::currentSdTrack()
{
	return query(dfplayer::GET_U_TRACK);
}




int16_t DFPlayerMini_Fast::currentFlashTrack()
{
	return query(dfplayer::GET_FLASH_TRACK);
}




int16_t DFPlayerMini_Fast::numTracksInFolder(uint8_t folder)
{
	return query(dfplayer::GET_FOLDER_FILES, (folder >> 8) & 0xFF, folder & 0xFF);
}




int16_t DFPlayerMini_Fast::numFolders()
{
	return query(dfplayer::GET_FOLDERS);
}




void DFPlayerMini_Fast::setTimeout(unsigned long threshold)
{
	_threshold = threshold;
}




void DFPlayerMini_Fast::findChecksum(stack *_stack)
{
	uint16_t checksum = (~(_stack->version + _stack->length + _stack->commandValue + _stack->feedbackValue + _stack->paramMSB + _stack->paramLSB)) + 1;

	_stack->checksumMSB = checksum >> 8;
	_stack->checksumLSB = checksum & 0xFF;
}




void DFPlayerMini_Fast::sendData()
{
	_serial->write(sendStack.start_byte);
	_serial->write(sendStack.version);
	_serial->write(sendStack.length);
	_serial->write(sendStack.commandValue);
	_serial->write(sendStack.feedbackValue);
	_serial->write(sendStack.paramMSB);
	_serial->write(sendStack.paramLSB);
	_serial->write(sendStack.checksumMSB);
	_serial->write(sendStack.checksumLSB);
	_serial->write(sendStack.end_byte);
}




void DFPlayerMini_Fast::flush()
{
	while (_serial->available())
		_serial->read();
}




int16_t DFPlayerMini_Fast::query(uint8_t cmd, uint8_t msb, uint8_t lsb)
{
	flush();

	sendStack.commandValue  = cmd;
	sendStack.feedbackValue = dfplayer::FEEDBACK;
	sendStack.paramMSB = msb;
	sendStack.paramLSB = lsb;

	findChecksum(&sendStack);
	sendData();

	if (getStatus(cmd))
		return (recStack.paramMSB << 8) | recStack.paramLSB;

	return -1;
}




bool DFPlayerMini_Fast::getStatus(uint8_t cmd)
{
	timoutTimer.start();
	bool result = parseFeedback();

	if (!result)
		return false;

	while (recStack.commandValue != cmd)
	{
		if (timoutTimer.fire())
			return false;

		result = parseFeedback();

		if (!result)
			return false;
	}

	return true;
}




bool DFPlayerMini_Fast::parseFeedback()
{
	while (true)
	{
		if (_serial->available())
		{
			char recChar = _serial->read();

			switch (state)
			{
			case find_start_byte:
			{
				if (recChar == dfplayer::SB)
				{
					recStack.start_byte = recChar;
					state = find_ver_byte;
				}
				break;
			}
			case find_ver_byte:
			{
				if (recChar != dfplayer::VER)
					return false;

				recStack.version = recChar;
				state = find_len_byte;
				break;
			}
			case find_len_byte:
			{
				if (recChar != dfplayer::LEN)
					return false;

				recStack.length = recChar;
				state = find_command_byte;
				break;
			}
			case find_command_byte:
			{
				recStack.commandValue = recChar;
				state = find_feedback_byte;
				break;
			}
			case find_feedback_byte:
			{
				recStack.feedbackValue = recChar;
				state = find_param_MSB;
				break;
			}
			case find_param_MSB:
			{
				recStack.paramMSB = recChar;
				state = find_param_LSB;
				break;
			}
			case find_param_LSB:
			{
				recStack.paramLSB = recChar;
				state = find_checksum_MSB;
				break;
			}
			case find_checksum_MSB:
			{
				recStack.checksumMSB = recChar;
				state = find_checksum_LSB;
				break;
			}
			case find_checksum_LSB:
			{
				recStack.checksumLSB = recChar;
				state = find_end_byte;
				break;
			}
			case find_end_byte:
			{
				if (recChar != dfplayer::EB)
					return false;

				recStack.end_byte = recChar;
				state = find_start_byte;
				return true;
				break;
			}
			default:
				break;
			}
		}

		if (timoutTimer.fire())
			return false;
	}
}




void DFPlayerMini_Fast::printStack(stack _stack)
{
	Serial.println("Stack:");
	Serial.print(_stack.start_byte, HEX);    Serial.print(' ');
	Serial.print(_stack.version, HEX);       Serial.print(' ');
	Serial.print(_stack.length, HEX);        Serial.print(' ');
	Serial.print(_stack.commandValue, HEX);  Serial.print(' ');
	Serial.print(_stack.feedbackValue, HEX); Serial.print(' ');
	Serial.print(_stack.paramMSB, HEX);      Serial.print(' ');
	Serial.print(_stack.paramLSB, HEX);      Serial.print(' ');
	Serial.print(_stack.checksumMSB, HEX);   Serial.print(' ');
	Serial.print(_stack.checksumLSB, HEX);   Serial.print(' ');
	Serial.println(_stack.end_byte, HEX);
}
