/*
	Audio.cpp

	Joe O'Regan
	K00203642

	Game audio class, with singleton access
	Cross-platform, plays different type of sound file, depending on the platform ot compile for
*/

#include "Audio.h"
//#include "string.h"

Audio* Audio::s_pInstance;																	// Singleton so only one instance of Audio exists in the game, for easy access

// IOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	// WAV audio format
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)	// WAV audio format
// Windows / Android
#define JOE_RIFF1 "joe_riff1.wav"
#define JOE_RIFF2 "joe_riff2.wav"
#define JOE_RIFF3 "BloodLevel-JOR-NEW.wav"

// Remember to preload!!!!
#define BUTTON_FX "buttonClick.wav"
#define EXPLOSION_LARGE "explosion_large.wav"
#define EXPLOSION_PLAYER "explosion.wav"
#define LASER_SHIP "laser_ship.wav"
#define LASER_ENEMY "laserEnemy.wav"
#define POWER_UP "powerup.wav"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)												// CAF: Core audio format
#define SPACE_GAME "SpaceGame.caf"
#define EXPLOSION_LARGE "explosion_large.caf"
#define LASER_SHIP "laser_ship.caf"
#endif

#define MAX_VOL 1.0f
#define MIN_VOL 0.0f
#define NUM_TRACKS 3

int currentTrack = 0;
const char* tracks[] = { "joe_riff1.wav" , "joe_riff2.wav", "BloodLevel-JOR-NEW.wav" };
std::string trackNames[] = { "Track 1: A song by Joe", "Track 2: Another song by Joe", "Track 3: Mostly Joe, guitar solo by Jimmy" };

void Audio::init() {
	currentVol = 1.0f;

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())						// If the background music isn't already playing
		SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[0], true);				// 20180202 Change background music

	// Preload the sound effects
	SimpleAudioEngine::getInstance()->preloadEffect(EXPLOSION_LARGE);
	SimpleAudioEngine::getInstance()->preloadEffect(EXPLOSION_PLAYER);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_SHIP);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_ENEMY);
	SimpleAudioEngine::getInstance()->preloadEffect(BUTTON_FX);
	SimpleAudioEngine::getInstance()->preloadEffect(POWER_UP);
//#endif
}
void Audio::selectMenuOption() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
}
bool Audio::isPaused() {
	return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

void Audio::explodeFX() {
	SimpleAudioEngine::getInstance()->playEffect(EXPLOSION_LARGE);							// Play explosion sound effect
}

void Audio::explodePlayerFX() {
	SimpleAudioEngine::getInstance()->playEffect(EXPLOSION_PLAYER);							// Play explosion sound effect
}
void Audio::laserFX() {
	SimpleAudioEngine::getInstance()->playEffect(LASER_SHIP);								// Play laser sound effect
}
void Audio::laserFXEnemy() {
	SimpleAudioEngine::getInstance()->playEffect(LASER_ENEMY);								// Play laser sound effect
}
void Audio::powerUpFX() {
	SimpleAudioEngine::getInstance()->playEffect(POWER_UP);									// Play power up sound effect
}

void Audio::play() {
	//SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[currentTrack], true);
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
void Audio::pause() {
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void Audio::skipTrackForwards() {
	//if (currentTrack < NUM_TRACKS - 1)
	//	currentTrack++;
	//else currentTrack = 0;
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	(currentTrack < NUM_TRACKS - 1) ? currentTrack++ : currentTrack = 0;

	SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[currentTrack], true);		// 20180221 skip audio track
}
void Audio::skipTrackBackwards() {
	//if (currentTrack > 0) currentTrack--;
	//else currentTrack = NUM_TRACKS - 1;
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	(currentTrack == 0) ? currentTrack = NUM_TRACKS - 1 : currentTrack--;

	SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[currentTrack], true);		// 20180221 skip audio track
}

void Audio::musicVolIncrease() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	if (currentVol < MAX_VOL)
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(currentVol + 0.1f);
}
void Audio::musicVolDecrease() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	if (currentVol > MIN_VOL)
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(currentVol - 0.1f);
}
void Audio::sfxVolIncrease() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	if (currentVol < MAX_VOL)
		SimpleAudioEngine::getInstance()->setEffectsVolume(currentVol + 0.1f);
}
void Audio::sfxVolDecrease() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	if (currentVol > MIN_VOL)
		SimpleAudioEngine::getInstance()->setEffectsVolume(currentVol - 0.1f);
}

std::string Audio::getTrackName() {
	return trackNames[currentTrack];
}