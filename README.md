# DFPlayer Mini Fast
[![GitHub version](https://badge.fury.io/gh/PowerBroker2%2FDFPlayerMini_Fast.svg)](https://badge.fury.io/gh/PowerBroker2%2FDFPlayerMini_Fast) [![arduino-library-badge](https://www.ardu-badge.com/badge/DFPlayerMini_Fast.svg?)](https://www.ardu-badge.com/DFPlayerMini_Fast)<br /><br />
Fast and easy to understand Arduino library to use the DFPlayer Mini MP3 module from DFRobot.com. This is a huge improvement (both in terms of execution speed and simplicity) to the standard library provided by DFRobot.com. 

[> See the documentation <](https://powerbroker2.github.io/DFPlayerMini_Fast/html/index.html)


## Important Notes
If you use the playFromMP3Folder() or playAdvertisement() functions, the files to be played must be organised in an extremely precise manner. The "MP3 Folder" must be in the root of the storage device (such as a MicroSD card) and it must be called "mp3". The folder name is probably not case-sensitive. Likewise, the "Advertisement" folder used for short interruptions to main audio playback must also be in the root of the storage device, and it must be called "advert". In addition, audio filenames must be prepended by a zero-padded 4-digit number if the files are in the root or "mp3" folders. If the files are played from folders other than root or "mp3", the folder names must be zero-padded 2-digit numbers while their contents must be audio files with names prepended by a zero-padded 3-digit number.

If you are using multiple DFPlayers with SoftwareSerial, it is necessary to make your SoftwareSerial instance listen (i.e. SoftwareSerial.listen()) before calling queries such as .isPlaying() or .currentVolume().

## Library API:
```c++
bool begin(Stream& stream, bool debug, unsigned long threshold=100);

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
void playLargeFolder(uint8_t folderNum, uint16_t trackNum);
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
void findChecksum(stack& _stack);
void sendData();
void flush();
int16_t query(uint8_t cmd, uint8_t msb=0, uint8_t lsb=0);
bool parseFeedback();

void printStack(stack _stack);
void printError();
```

## DFPlayer Mini Pinout:
![550px-Miniplayer_pin_map](https://user-images.githubusercontent.com/20977405/54732437-2623ae80-4b6a-11e9-9005-768ae5a92281.png)

## DFPlayer Mini Pin Description:
![Pin_map_desc_en](https://user-images.githubusercontent.com/20977405/54732438-26bc4500-4b6a-11e9-8969-3d2927a5d513.png)

## Example Wiring Diagram:
![550px-PlayerMini](https://user-images.githubusercontent.com/20977405/54732436-2623ae80-4b6a-11e9-91a7-fe4cce416eaa.png)

For more info: https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299
