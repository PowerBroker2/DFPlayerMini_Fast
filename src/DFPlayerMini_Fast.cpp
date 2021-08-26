/*!
 * @file DFPlayerMini_Fast.cpp
 *
 * @mainpage DFPlayerMini_Fast
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for the YX5200-24SS MP3 player driver code for the
 * Arduino platform.  It is designed specifically to work with the
 * <a href="https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299">
 * DFPlayerMini breakout</a>
 *
 * These MP3 players use UART to communicate, 2 pins (TX + RX) are required
 * to interface with the breakout. An external power supply and output amp
 * improves the MP3 player's functionality. Also, see <a href="https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299">
 * the wiki</a> for more detailed wiring instructions.
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a href="https://github.com/PowerBroker2/FireTimer">
 * FireTimer.h</a> being present on your system. Please make sure you have
 * installed the latest version before using this library.
 *
 * @section author Author
 *
 * Written by Power_Broker as a hobby.
 *
 * @section license License
 *
 * None.
 *
 */

#include "DFPlayerMini_Fast.h"
#include "FireTimer.h"




 /**************************************************************************/
 /*!
	 @brief  Configure the class.
	 @param    stream
			   A reference to the Serial instance (hardware or software)
			   used to communicate with the MP3 player.
	 @param    debug
			   Boolean used to specify if debug prints should be automatically
			   printed to the serial monitor.
	 @param    threshold
			   Number of ms allowed for the MP3 player to respond (timeout)
			   to a query.
	 @return True.
 */
 /**************************************************************************/
bool DFPlayerMini_Fast::begin(Stream &stream, bool debug, unsigned long threshold)
{
	_threshold = threshold;
	timoutTimer.begin(_threshold);

	_serial = &stream;

	_debug = debug;

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




/**************************************************************************/
 /*!
	 @brief  Play the next song in chronological order.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::playNext()
{
	sendStack.commandValue  = dfplayer::NEXT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Play the previous song in chronological order.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::playPrevious()
{
	sendStack.commandValue  = dfplayer::PREV;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Play a specific track.
	 @param    trackNum
			   The track number to play.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::play(uint16_t trackNum)
{
	sendStack.commandValue  = dfplayer::PLAY;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = (trackNum >> 8) & 0xFF;
	sendStack.paramLSB = trackNum & 0xFF;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Stop the current playback
 */
 /**************************************************************************/
void DFPlayerMini_Fast::stop()
{
	sendStack.commandValue = dfplayer::STOP;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Play a specific track in the folder named "MP3".
	 @param    trackNum
			   The track number to play.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::playFromMP3Folder(uint16_t trackNum)
{
	sendStack.commandValue  = dfplayer::USE_MP3_FOLDER;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = (trackNum >> 8) & 0xFF;
	sendStack.paramLSB = trackNum & 0xFF;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Interrupt the current track with a new track.
	 @param    trackNum
			   The track number to play.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::playAdvertisement(uint16_t trackNum)
{
	sendStack.commandValue  = dfplayer::INSERT_ADVERT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = (trackNum >> 8) & 0xFF;
	sendStack.paramLSB = trackNum & 0xFF;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Stop the interrupting track.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::stopAdvertisement()
{
	sendStack.commandValue  = dfplayer::STOP_ADVERT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Increment the volume by 1 out of 30.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::incVolume()
{
	sendStack.commandValue  = dfplayer::INC_VOL;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Decrement the volume by 1 out of 30.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::decVolume()
{
	sendStack.commandValue  = dfplayer::DEC_VOL;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Set the volume to a specific value out of 30.
	 @param    volume
			   The volume level (0 - 30).
 */
 /**************************************************************************/
void DFPlayerMini_Fast::volume(uint8_t volume)
{
	if (volume <= 30)
	{
		sendStack.commandValue  = dfplayer::VOLUME;
		sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
		sendStack.paramMSB = 0;
		sendStack.paramLSB = volume;

		findChecksum(sendStack);
		sendData();
	}
}




/**************************************************************************/
 /*!
	 @brief  Set the EQ mode.
	 @param    setting
			   The desired EQ ID.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::EQSelect(uint8_t setting)
{
	if (setting <= 5)
	{
		sendStack.commandValue  = dfplayer::EQ;
		sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
		sendStack.paramMSB = 0;
		sendStack.paramLSB = setting;

		findChecksum(sendStack);
		sendData();
	}
}




/**************************************************************************/
 /*!
	 @brief  Loop a specific track.
	 @param    trackNum
			   The track number to play.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::loop(uint16_t trackNum)
{
  sendStack.commandValue  = dfplayer::PLAYBACK_MODE;
  sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
  sendStack.paramMSB = (trackNum >> 8) & 0xFF;
  sendStack.paramLSB = trackNum & 0xFF;
  
  findChecksum(sendStack);
  sendData();
}




/**************************************************************************/
 /*!
	 @brief  Specify the playback source.
	 @param    source
			   The playback source ID.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::playbackSource(uint8_t source)
{
	if ((source > 0) && (source <= 5))
	{
		sendStack.commandValue  = dfplayer::PLAYBACK_SRC;
		sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
		sendStack.paramMSB = 0;
		sendStack.paramLSB = source;

		findChecksum(sendStack);
		sendData();
	}
}




/**************************************************************************/
 /*!
	 @brief  Put the MP3 player in standby mode (this is NOT sleep mode).
 */
 /**************************************************************************/
void DFPlayerMini_Fast::standbyMode()
{
	sendStack.commandValue  = dfplayer::STANDBY;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Pull the MP3 player out of standby mode.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::normalMode()
{
	sendStack.commandValue  = dfplayer::NORMAL;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Reset all settings to factory default.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::reset()
{
	sendStack.commandValue  = dfplayer::RESET;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Resume playing current track.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::resume()
{
	sendStack.commandValue  = dfplayer::PLAYBACK;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Pause playing current track.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::pause()
{
	sendStack.commandValue  = dfplayer::PAUSE;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Play a specific track from a specific folder.
	 @param    folderNum
			   The folder number.
	 @param    trackNum
			   The track number to play.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::playFolder(uint8_t folderNum, uint8_t trackNum)
{
	sendStack.commandValue  = dfplayer::SPEC_FOLDER;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = folderNum;
	sendStack.paramLSB = trackNum;

	findChecksum(sendStack);
	sendData();
}

/**************************************************************************/
 /*!
	 @brief  Play a specific track from a specific folder, where the track
			 names are numbered 4 digit (e.g. 1234-mysong.mp3) and can be 
			 up to 3000. Only 15 folders ("01" to "15") are supported in this
			 mode. 
	 @param    folderNum
			   The folder number.
	 @param    trackNum
			   The track number to play.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::playLargeFolder(uint8_t folderNum, uint16_t trackNum)
{
	const uint16_t arg = (((uint16_t)folderNum) << 12) | (trackNum & 0xfff);

	sendStack.commandValue	= dfplayer::SPEC_TRACK_3000;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = arg >> 8;
	sendStack.paramLSB = arg & 0xff;

	findChecksum(sendStack);
	sendData();
}

/**************************************************************************/
 /*!
	 @brief  Specify volume gain.
	 @param    gain
			   The specified volume gain.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::volumeAdjustSet(uint8_t gain)
{
	if (gain <= 31)
	{
		sendStack.commandValue  = dfplayer::VOL_ADJ;
		sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
		sendStack.paramMSB = 0;
		sendStack.paramLSB = dfplayer::VOL_ADJUST + gain;

		findChecksum(sendStack);
		sendData();
	}
}




/**************************************************************************/
 /*!
	 @brief  Play all tracks.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::startRepeatPlay()
{
	sendStack.commandValue  = dfplayer::REPEAT_PLAY;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = dfplayer::START_REPEAT;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Stop repeat play.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::stopRepeatPlay()
{
	sendStack.commandValue  = dfplayer::REPEAT_PLAY;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = dfplayer::STOP_REPEAT;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Play all tracks in a given folder.
	 @param    folderNum
			   The folder number.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::repeatFolder(uint16_t folder)
{
	sendStack.commandValue  = dfplayer::REPEAT_FOLDER;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = (folder >> 8) & 0xFF;
	sendStack.paramLSB = folder & 0xFF;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Play all tracks in a random order.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::randomAll()
{
	sendStack.commandValue  = dfplayer::RANDOM_ALL;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Repeat the current track.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::startRepeat()
{
	sendStack.commandValue  = dfplayer::REPEAT_CURRENT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Stop repeat play of the current track.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::stopRepeat()
{
	sendStack.commandValue  = dfplayer::REPEAT_CURRENT;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Turn on DAC.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::startDAC()
{
	sendStack.commandValue  = dfplayer::SET_DAC;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 0;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Turn off DAC.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::stopDAC()
{
	sendStack.commandValue  = dfplayer::SET_DAC;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = 0;
	sendStack.paramLSB = 1;

	findChecksum(sendStack);
	sendData();
}




/**************************************************************************/
 /*!
	 @brief  Put the MP3 player into sleep mode.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::sleep()
{
	playbackSource(dfplayer::SLEEP);
}




/**************************************************************************/
 /*!
	 @brief  Pull the MP3 player out of sleep mode.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::wakeUp()
{
	playbackSource(dfplayer::TF);
}




/**************************************************************************/
 /*!
	 @brief  Determine if a track is currently playing.
	 @return True if a track is currently playing, false if not, -1 if error.
 */
 /**************************************************************************/
bool DFPlayerMini_Fast::isPlaying()
{
	int16_t result = query(dfplayer::GET_STATUS_);

	if (result != -1)
		return (result & 1);

	return 0;
}




/**************************************************************************/
 /*!
	 @brief  Determine the current volume setting.
	 @return Volume level (0-30), -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::currentVolume()
{
	return query(dfplayer::GET_VOL);
}




/**************************************************************************/
 /*!
	 @brief  Determine the current EQ setting.
	 @return EQ setting, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::currentEQ()
{
	return query(dfplayer::GET_EQ);
}




/**************************************************************************/
 /*!
	 @brief  Determine the current mode.
	 @return Mode, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::currentMode()
{
	return query(dfplayer::GET_MODE);
}




/**************************************************************************/
 /*!
	 @brief  Determine the current firmware version.
	 @return Firmware version, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::currentVersion()
{
	return query(dfplayer::GET_VERSION);
}




/**************************************************************************/
 /*!
	 @brief  Determine the number of tracks accessible via USB.
	 @return Number of tracks accessible via USB, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::numUsbTracks()
{
	return query(dfplayer::GET_TF_FILES);
}




/**************************************************************************/
 /*!
	 @brief  Determine the number of tracks accessible via SD card.
	 @return Number of tracks accessible via SD card, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::numSdTracks()
{
	return query(dfplayer::GET_U_FILES);
}




/**************************************************************************/
 /*!
	 @brief  Determine the number of tracks accessible via flash.
	 @return Number of tracks accessible via flash, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::numFlashTracks()
{
	return query(dfplayer::GET_FLASH_FILES);
}




/**************************************************************************/
 /*!
	 @brief  Determine the current track played via USB.
	 @return Current track played via USB, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::currentUsbTrack()
{
	return query(dfplayer::GET_TF_TRACK);
}




/**************************************************************************/
 /*!
	 @brief  Determine the current track played via SD card.
	 @return Current track played via SD card, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::currentSdTrack()
{
	return query(dfplayer::GET_U_TRACK);
}




/**************************************************************************/
 /*!
	 @brief  Determine the current track played via flash.
	 @return Current track played via flash, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::currentFlashTrack()
{
	return query(dfplayer::GET_FLASH_TRACK);
}




/**************************************************************************/
 /*!
	 @brief  Determine the number of tracks in the specified folder.
	 @param    folder
			   The folder number.
	 @return Number of tracks in the specified folder, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::numTracksInFolder(uint8_t folder)
{
	return query(dfplayer::GET_FOLDER_FILES, (folder >> 8) & 0xFF, folder & 0xFF);
}




/**************************************************************************/
 /*!
	 @brief  Determine the number of folders available.
	 @return Number of folders available, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::numFolders()
{
	return query(dfplayer::GET_FOLDERS);
}




/**************************************************************************/
 /*!
	 @brief  Set the timout value for MP3 player query responses.
	 @param    threshold
			   Number of ms allowed for the MP3 player to respond (timeout)
			   to a query.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::setTimeout(unsigned long threshold)
{
	_threshold = threshold;
	timoutTimer.begin(_threshold);
}




/**************************************************************************/
 /*!
	 @brief  Determine and insert the checksum of a given config/command
	         packet into that same packet struct.
	 @param    _stack
			   Reference to a struct containing the config/command packet
			   to calculate the checksum over.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::findChecksum(stack& _stack)
{
	uint16_t checksum = (~(_stack.version + _stack.length + _stack.commandValue + _stack.feedbackValue + _stack.paramMSB + _stack.paramLSB)) + 1;

	_stack.checksumMSB = checksum >> 8;
	_stack.checksumLSB = checksum & 0xFF;
}




/**************************************************************************/
 /*!
	 @brief  Send a config/command packet to the MP3 player.
 */
 /**************************************************************************/
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

	if (_debug)
	{
		Serial.print(F("Sent "));
		printStack(sendStack);
		Serial.println();
	}
}




/**************************************************************************/
 /*!
	 @brief  Clear out the serial input buffer connected to the MP3 player.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::flush()
{
	while (_serial->available())
		_serial->read();
}




/**************************************************************************/
 /*!
	 @brief  Query the MP3 player for specific information.
	 @param    cmd
			   The command/query ID.
	 @param    msb
			   The payload/parameter MSB.
	 @param    lsb
			   The payload/parameter LSB.
	 @return Query response, -1 if error.
 */
 /**************************************************************************/
int16_t DFPlayerMini_Fast::query(uint8_t cmd, uint8_t msb, uint8_t lsb)
{
	flush();

	sendStack.commandValue  = cmd;
	sendStack.feedbackValue = dfplayer::NO_FEEDBACK;
	sendStack.paramMSB = msb;
	sendStack.paramLSB = lsb;

	findChecksum(sendStack);
	sendData();
	timoutTimer.start();

	if (parseFeedback())
		if (recStack.commandValue != 0x40)
			return (recStack.paramMSB << 8) | recStack.paramLSB;

	return -1;
}




/**************************************************************************/
 /*!
	 @brief  Parse MP3 player query responses.
	 @return True if success, false if error.
 */
 /**************************************************************************/
bool DFPlayerMini_Fast::parseFeedback()
{
	while (true)
	{
		if (_serial->available())
		{
			uint8_t recChar = _serial->read();

			if (_debug)
			{
				Serial.print(F("Rec: "));
				Serial.println(recChar, HEX);
				Serial.print(F("State: "));
			}

			switch (state)
			{
			case find_start_byte:
			{
				if (_debug)
					Serial.println(F("find_start_byte"));

				if (recChar == dfplayer::SB)
				{
					recStack.start_byte = recChar;
					state = find_ver_byte;
				}
				break;
			}
			case find_ver_byte:
			{
				if (_debug)
					Serial.println(F("find_ver_byte"));

				if (recChar != dfplayer::VER)
				{
					if (_debug)
						Serial.println(F("ver error"));

					state = find_start_byte;
					return false;
				}

				recStack.version = recChar;
				state = find_len_byte;
				break;
			}
			case find_len_byte:
			{
				if (_debug)
					Serial.println(F("find_len_byte"));

				if (recChar != dfplayer::LEN)
				{
					if (_debug)
						Serial.println(F("len error"));

					state = find_start_byte;
					return false;
				}

				recStack.length = recChar;
				state = find_command_byte;
				break;
			}
			case find_command_byte:
			{
				if (_debug)
					Serial.println(F("find_command_byte"));

				recStack.commandValue = recChar;
				state = find_feedback_byte;
				break;
			}
			case find_feedback_byte:
			{
				if (_debug)
					Serial.println(F("find_feedback_byte"));

				recStack.feedbackValue = recChar;
				state = find_param_MSB;
				break;
			}
			case find_param_MSB:
			{
				if (_debug)
					Serial.println(F("find_param_MSB"));

				recStack.paramMSB = recChar;
				state = find_param_LSB;
				break;
			}
			case find_param_LSB:
			{
				if (_debug)
					Serial.println(F("find_param_LSB"));

				recStack.paramLSB = recChar;
				state = find_checksum_MSB;
				break;
			}
			case find_checksum_MSB:
			{
				if (_debug)
					Serial.println(F("find_checksum_MSB"));

				recStack.checksumMSB = recChar;
				state = find_checksum_LSB;
				break;
			}
			case find_checksum_LSB:
			{
				if (_debug)
					Serial.println(F("find_checksum_LSB"));

				recStack.checksumLSB = recChar;

				int recChecksum  = (recStack.checksumMSB << 8) | recStack.checksumLSB;
				findChecksum(recStack);
				int calcChecksum = (recStack.checksumMSB << 8) | recStack.checksumLSB;

				if (recChecksum != calcChecksum)
				{
					if (_debug)
					{
						Serial.println(F("checksum error"));
						Serial.print(F("recChecksum: 0x"));
						Serial.println(recChecksum, HEX);
						Serial.print(F("calcChecksum: 0x"));
						Serial.println(calcChecksum, HEX);
						Serial.println();
					}

					state = find_start_byte;
					return false;
				}
				else
					state = find_end_byte;
				break;
			}
			case find_end_byte:
			{
				if (_debug)
					Serial.println(F("find_end_byte"));

				if (recChar != dfplayer::EB)
				{
					if (_debug)
						Serial.println(F("eb error"));

					state = find_start_byte;
					return false;
				}

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
		{
			if (_debug)
				Serial.println(F("timeout error"));

			state = find_start_byte;
			return false;
		}
	}
}




/**************************************************************************/
 /*!
	 @brief  Print the entire contents of the specified config/command
	         packet for debugging purposes.
	 @param    _stack
			   Struct containing the config/command packet
			   to print.
 */
 /**************************************************************************/
void DFPlayerMini_Fast::printStack(stack _stack)
{
	Serial.println(F("Stack:"));
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




/**************************************************************************/
 /*!
	 @brief  Print the error description if an error has been received
 */
 /**************************************************************************/
void DFPlayerMini_Fast::printError()
{
	if (recStack.commandValue == 0x40)
	{
		switch (recStack.paramLSB)
		{
		case 0x1:
		{
			Serial.println(F("Module busy (this info is returned when the initialization is not done)"));
			break;
		}
		case 0x2:
		{
			Serial.println(F("Currently sleep mode(supports only specified device in sleep mode)"));
			break;
		}
		case 0x3:
		{
			Serial.println(F("Serial receiving error(a frame has not been received completely yet)"));
			break;
		}
		case 0x4:
		{
			Serial.println(F("Checksum incorrect"));
			break;
		}
		case 0x5:
		{
			Serial.println(F("Specified track is out of current track scope"));
			break;
		}
		case 0x6:
		{
			Serial.println(F("Specified track is not found"));
			break;
		}
		case 0x7:
		{
			Serial.println(F("Insertion error(an inserting operation only can be done when a track is being played)"));
			break;
		}
		case 0x8:
		{
			Serial.println(F("SD card reading failed(SD card pulled out or damaged)"));
			break;
		}
		case 0xA:
		{
			Serial.println(F("Entered into sleep mode"));
			break;
		}
		default:
		{
			Serial.print(F("Unknown error: "));
			Serial.println(recStack.paramLSB);
			break;
		}
		}
	}
	else
		Serial.println(F("No error"));
}
