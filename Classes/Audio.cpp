/*
	Audio.cpp

	Joe O'Regan
	K00203642

	Game audio class, with singleton access
	Cross-platform, plays different type of sound file, depending on the platform ot compile for
*/

#include "Audio.h"
#include "string.h"

Audio* Audio::s_pInstance;																	// Singleton so only one instance of Audio exists in the game, for easy access

// IOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	// WAV audio format
// Windows / Android
#define JOE_RIFF1 "joe_riff1.wav"
#define JOE_RIFF2 "joe_riff2.wav"
#define JOE_RIFF3 "JoeBloodLevel.wav"
#define EXPLOSION_LARGE "explosion_large.wav"
#define LASER_SHIP "laser_ship.wav"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)												// CAF: Core audio format
#define SPACE_GAME "SpaceGame.caf"
#define EXPLOSION_LARGE "explosion_large.caf"
#define LASER_SHIP "laser_ship.caf"
#endif

#define MAX_VOL 1.0f
#define MIN_VOL 0.0f
#define NUM_TRACKS 3

int currentTrack = 0;
const char* tracks[] = { "joe_riff1.wav" , "joe_riff2.wav", "JoeBloodLevel.wav" };

void Audio::init() {
	currentVol = 1.0f;

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())						// If the background music isn't already playing
		SimpleAudioEngine::getInstance()->playBackgroundMusic(JOE_RIFF1, true);				// 20180202 Change background music

	// Preload the sound effects
	SimpleAudioEngine::getInstance()->preloadEffect(EXPLOSION_LARGE);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_SHIP);
//#endif
}

void Audio::explodeFX() {
	SimpleAudioEngine::getInstance()->playEffect(EXPLOSION_LARGE);							// Play explosion sound effect
}

void Audio::laserFX() {
	SimpleAudioEngine::getInstance()->playEffect(LASER_SHIP);								// Play laser sound effect
}

void Audio::skipTrackForwards() {
	if (currentTrack < NUM_TRACKS - 1)
		currentTrack++;
	else currentTrack = 0;

	SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[currentTrack], true);		// 20180221 skip audio track
}
void Audio::skipTrackBackwards() {
	if (currentTrack > 0) currentTrack--;
	else currentTrack = NUM_TRACKS - 1;

	SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[currentTrack], true);		// 20180221 skip audio track
}

void Audio::musicVolIncrease() {
	if (currentVol <= MAX_VOL)
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(currentVol + 0.1f);
}
void Audio::musicVolDecrease() {
	if (currentVol >= MIN_VOL)
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(currentVol - 0.1f);
}
void Audio::sfxVolIncrease() {
	if (currentVol <= MAX_VOL)
		SimpleAudioEngine::getInstance()->setEffectsVolume(currentVol + 0.1f);
}
void Audio::sfxVolDecrease() {
	if (currentVol >= MIN_VOL)
		SimpleAudioEngine::getInstance()->setEffectsVolume(currentVol - 0.1f);
}