/*
	Audio.cpp

	Joe O'Regan
	K00203642

	Game audio class, with singleton access
	Cross-platform, plays different type of sound file, depending on the platform ot compile for
*/

#include "Audio.h"

Audio* Audio::s_pInstance;																	// Singleton so only one instance of Audio exists in the game, for easy access

// IOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	// WAV audio format
// Windows / Android
#define JOE_RIFF1 "joe_riff1.wav"
#define EXPLOSION_LARGE "explosion_large.wav"
#define LASER_SHIP "laser_ship.wav"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)												// CAF: Core audio format
#define SPACE_GAME "SpaceGame.caf"
#define EXPLOSION_LARGE "explosion_large.caf"
#define LASER_SHIP "laser_ship.caf"
#endif

void Audio::init() {
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	SimpleAudioEngine::getInstance()->playBackgroundMusic(JOE_RIFF1, true);					// 20180202 Change background music
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