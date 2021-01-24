/*!
 * @file DFPlayerMini_Fast.h
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
 * Written by Power_Broker as a hobby.
 *
 * None.
 *
 */

#pragma once
#include "Arduino.h"
#include "FireTimer.h"




 /**************************************************************************/
 /*!
	 @brief  Namespace for constants
 */
 /**************************************************************************/
namespace dfplayer
{
	/** Packet Values */
	const uint8_t STACK_SIZE      = 10;   // total number of bytes in a stack/packet (same for cmds and queries)
	const uint8_t SB              = 0x7E; // start byte
	const uint8_t VER             = 0xFF; // version
	const uint8_t LEN             = 0x6;  // number of bytes after "LEN" (except for checksum data and EB)
	const uint8_t FEEDBACK        = 1;    // feedback requested
	const uint8_t NO_FEEDBACK     = 0;    // no feedback requested
	const uint8_t EB              = 0xEF; // end byte

	/** Control Command Values */
	const uint8_t NEXT            = 0x01;
	const uint8_t PREV            = 0x02;
	const uint8_t PLAY            = 0x03;
	const uint8_t INC_VOL         = 0x04;
	const uint8_t DEC_VOL         = 0x05;
	const uint8_t VOLUME          = 0x06;
	const uint8_t EQ              = 0x07;
	const uint8_t PLAYBACK_MODE   = 0x08;
	const uint8_t PLAYBACK_SRC    = 0x09;
	const uint8_t STANDBY         = 0x0A;
	const uint8_t NORMAL          = 0x0B;
	const uint8_t RESET           = 0x0C;
	const uint8_t PLAYBACK        = 0x0D;
	const uint8_t PAUSE           = 0x0E;
	const uint8_t SPEC_FOLDER     = 0x0F;
	const uint8_t VOL_ADJ         = 0x10;
	const uint8_t REPEAT_PLAY     = 0x11;
	const uint8_t USE_MP3_FOLDER  = 0x12;
	const uint8_t INSERT_ADVERT   = 0x13;
	const uint8_t SPEC_TRACK_3000 = 0x14;
	const uint8_t STOP_ADVERT     = 0x15;
	const uint8_t STOP            = 0x16;
	const uint8_t REPEAT_FOLDER   = 0x17;
	const uint8_t RANDOM_ALL      = 0x18;
	const uint8_t REPEAT_CURRENT  = 0x19;
	const uint8_t SET_DAC         = 0x1A;

	/** Query Command Values */
	const uint8_t SEND_INIT        = 0x3F;
	const uint8_t RETRANSMIT       = 0x40;
	const uint8_t REPLY            = 0x41;
	const uint8_t GET_STATUS_      = 0x42;
	const uint8_t GET_VOL          = 0x43;
	const uint8_t GET_EQ           = 0x44;
	const uint8_t GET_MODE         = 0x45;
	const uint8_t GET_VERSION      = 0x46;
	const uint8_t GET_TF_FILES     = 0x47;
	const uint8_t GET_U_FILES      = 0x48;
	const uint8_t GET_FLASH_FILES  = 0x49;
	const uint8_t KEEP_ON          = 0x4A;
	const uint8_t GET_TF_TRACK     = 0x4B;
	const uint8_t GET_U_TRACK      = 0x4C;
	const uint8_t GET_FLASH_TRACK  = 0x4D;
	const uint8_t GET_FOLDER_FILES = 0x4E;
	const uint8_t GET_FOLDERS      = 0x4F;

	/** EQ Values */
	const uint8_t EQ_NORMAL       = 0;
	const uint8_t EQ_POP          = 1;
	const uint8_t EQ_ROCK         = 2;
	const uint8_t EQ_JAZZ         = 3;
	const uint8_t EQ_CLASSIC      = 4;
	const uint8_t EQ_BASE         = 5;

	/** Mode Values */
	const uint8_t REPEAT          = 0;
	const uint8_t FOLDER_REPEAT   = 1;
	const uint8_t SINGLE_REPEAT   = 2;
	const uint8_t RANDOM          = 3;

	/** Playback Source Values */
	const uint8_t U               = 1;
	const uint8_t TF              = 2;
	const uint8_t AUX             = 3;
	const uint8_t SLEEP           = 4;
	const uint8_t FLASH           = 5;

	/** Base Volume Adjust Value */
	const uint8_t VOL_ADJUST      = 0x10;

	/** Repeat Play Values */
	const uint8_t STOP_REPEAT     = 0;
	const uint8_t START_REPEAT    = 1;
}




/**************************************************************************/
/*!
	@brief  Class for interacting with DFPlayerMini MP3 player
*/
/**************************************************************************/
class DFPlayerMini_Fast
{
public:
	Stream* _serial;
    
	/** Struct to store entire serial datapacket used for MP3 config/control */
	struct stack {
		uint8_t start_byte;
		uint8_t version;
		uint8_t length;
		uint8_t commandValue;
		uint8_t feedbackValue;
		uint8_t paramMSB;
		uint8_t paramLSB;
		uint8_t checksumMSB;
		uint8_t checksumLSB;
		uint8_t end_byte;
	} sendStack, recStack;

	bool _debug;




	bool begin(Stream& stream, bool debug=false, unsigned long threshold=100);

	void playNext();
	void playPrevious();
	void play(uint16_t trackNum);
	void stop();
	void playFromMP3Folder(uint16_t trackNum);
	void playAdvertisement(uint16_t trackNum);
	void stopAdvertisement();
	void incVolume();
	void decVolume();
	void volume(uint8_t volume);
	void EQSelect(uint8_t setting);
	void loop(uint16_t trackNum);
	void playbackSource(uint8_t source);
	void standbyMode();
	void normalMode();
	void reset();
	void resume();
	void pause();
	void playFolder(uint8_t folderNum, uint8_t trackNum);
	void volumeAdjustSet(uint8_t gain);
	void startRepeatPlay();
	void stopRepeatPlay();
	void repeatFolder(uint16_t folder);
	void randomAll();
	void startRepeat();
	void stopRepeat();
	void startDAC();
	void stopDAC();
	void sleep();
	void wakeUp();

	bool isPlaying();
	int16_t currentVolume();
	int16_t currentEQ();
	int16_t currentMode();
	int16_t currentVersion();
	int16_t numUsbTracks();
	int16_t numSdTracks();
	int16_t numFlashTracks();
	int16_t currentUsbTrack();
	int16_t currentSdTrack();
	int16_t currentFlashTrack();
	int16_t numTracksInFolder(uint8_t folder);
	int16_t numFolders();
	
	void setTimeout(unsigned long threshold);
	void findChecksum(stack *_stack);
	void sendData();
	void flush();
	int16_t query(uint8_t cmd, uint8_t msb=0, uint8_t lsb=0);
	bool parseFeedback();

	void printStack(stack _stack);




private:
	FireTimer timoutTimer;
	unsigned long _threshold;

	/** MP3 response packet parsing states */
	enum fsm {
		find_start_byte,
		find_ver_byte,
		find_len_byte,
		find_command_byte,
		find_feedback_byte,
		find_param_MSB,
		find_param_LSB,
		find_checksum_MSB,
		find_checksum_LSB,
		find_end_byte
	};
	fsm state = find_start_byte;
};
