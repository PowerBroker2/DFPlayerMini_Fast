#include "DFPlayerMini_Fast.h"




bool DFPlayerMini_Fast::begin(Stream &stream)
{
	_serial = &stream;
	return true;
}




void DFPlayerMini_Fast::playNext()
{
	commandValue = NEXT_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::playPrevious()
{
	commandValue = PREV_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::play(uint16_t trackNum)
{
	commandValue = PLAY_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = (trackNum >> 8) & 0xFF;
	paramLSB = trackNum & 0xFF;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::incVolume()
{
	commandValue = INC_VOL_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::decVolume()
{
	commandValue = DEC_VOL_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::volume(uint8_t volume)
{
	if (volume <= 30)
	{
		commandValue = VOLUME_COMMAND;
		feedbackValue = NO_FEEDBACK;
		paramMSB = 0;
		paramLSB = volume;
  
		findChecksum();
		sendData();
	}
}




void DFPlayerMini_Fast::EQSelect(uint8_t setting)
{
	if (setting <= 5)
	{
		commandValue = EQ_COMMAND;
		feedbackValue = NO_FEEDBACK;
		paramMSB = 0;
		paramLSB = setting;

		findChecksum();
		sendData();
	}
}




void DFPlayerMini_Fast::playbackMode(uint8_t mode)
{
	if (mode <= 5)
	{
		commandValue = PLAYBACK_MODE_COMMAND;
		feedbackValue = NO_FEEDBACK;
		paramMSB = 0;
		paramLSB = mode;

		findChecksum();
		sendData();
	}
}




void DFPlayerMini_Fast::playbackSource(uint8_t source)
{
	if ((source > 0) && (source <= 5))
	{
		commandValue = PLAYBACK_SRC_COMMAND;
		feedbackValue = NO_FEEDBACK;
		paramMSB = 0;
		paramLSB = source;

		findChecksum();
		sendData();
	}
}




void DFPlayerMini_Fast::standbyMode()
{
	commandValue = STANDBY_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::normalMode()
{
	commandValue = NORMAL_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::reset()
{
	commandValue = RESET_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::resume()
{
	commandValue = PLAYBACK_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::pause()
{
	commandValue = PAUSE_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = 1;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::playFolder(uint8_t folderNum, uint8_t trackNum)
{
	commandValue = SPEC_FOLDER_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = folderNum;
	paramLSB = trackNum;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::volumeAdjustSet(uint8_t gain)
{
	if (gain <= 31)
	{
		commandValue = VOL_ADJ_COMMAND;
		feedbackValue = NO_FEEDBACK;
		paramMSB = 0;
		paramLSB = VOL_ADJUST + gain;

		findChecksum();
		sendData();
	}
}




void DFPlayerMini_Fast::startRepeatPlay()
{
	commandValue = REPEAT_PLAY_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = START_REPEAT;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::stopRepeatPlay()
{
	commandValue = REPEAT_PLAY_COMMAND;
	feedbackValue = NO_FEEDBACK;
	paramMSB = 0;
	paramLSB = STOP_REPEAT;

	findChecksum();
	sendData();
}




void DFPlayerMini_Fast::sleep()
{
	playbackSource(SLEEP);
}




void DFPlayerMini_Fast::wakeUp()
{
	playbackSource(TF);

	delay(100);
}




void DFPlayerMini_Fast::loop(uint16_t trackNum)
{
  commandValue = PLAYBACK_MODE_COMMAND;
  feedbackValue = NO_FEEDBACK;
  paramMSB = (trackNum >> 8) & 0xFF;
  paramLSB = trackNum & 0xFF;
  
  findChecksum();
  sendData();
}




bool DFPlayerMini_Fast::trackIsPlaying()
{
	uint8_t i = 0;
	uint8_t recChar;
	bool status = false;

	// clear out old data from the buffer
	while (_serial->available())
		_serial->read();

	commandValue = GET_STATUS_COMMAND;
	feedbackValue = FEEDBACK;
	paramMSB = 0;
	paramLSB = 0;

	findChecksum();
	sendData();

	delay(100); // this delay is required for some mysterious reason...if not present, only 2 bytes of serial data will be recieved, instad of the expected 32 bytes
	while (_serial->available())
	{
		recChar = _serial->read();
		if (i == 6)
			if (recChar == 1)
				status = true;
		i += 1;
	}

	return status;
}




/*
 ************************************* TODO *************************************
*/
uint8_t DFPlayerMini_Fast::currentTrack()
{
	uint8_t recChar;

	commandValue = GET_TF_TRACK_COMMAND;
	feedbackValue = FEEDBACK;
	paramMSB = 0;
	paramLSB = 0;

	findChecksum();
	sendData();

	delay(100);
	while (_serial->available())
	{
		recChar = _serial->read();
	}

	return -1;
}




void DFPlayerMini_Fast::findChecksum()
{
	uint16_t checksum = (~(VER + LEN + commandValue + feedbackValue + paramMSB + paramLSB)) + 1;

	checksumMSB = checksum >> 8;
	checksumLSB = checksum & 0xFF;
}




void DFPlayerMini_Fast::sendData()
{
	_sending[0] = SB;
	_sending[1] = VER;
	_sending[2] = LEN;
	_sending[3] = commandValue;
	_sending[4] = feedbackValue;
	_sending[5] = paramMSB;
	_sending[6] = paramLSB;
	_sending[7] = checksumMSB;
	_sending[8] = checksumLSB;
	_sending[9] = EB;
  
	_serial->write(_sending, DFPLAYER_SEND_LENGTH);
}
