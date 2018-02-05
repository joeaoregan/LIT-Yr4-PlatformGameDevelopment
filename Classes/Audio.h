
#ifndef __AUDIO__
#define __AUDIO__

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

// IOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define SPACE_GAME "SpaceGame.caf"
#define EXPLOSION_LARGE "explosion_large.caf"
#define LASER_SHIP "laser_ship.caf"
#else
// Windows / Android
#define JOE_RIFF1 "joe_riff1.wav"
#define SPACE_GAME "SpaceGame.wav"
#define EXPLOSION_LARGE "explosion_large.wav"
#define LASER_SHIP "laser_ship.wav"
#endif

class Audio {
public:	
	static Audio* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Audio();
			return s_pInstance;
		}
		return s_pInstance;
	}

	void init(){
		//SimpleAudioEngine::getInstance()->playBackgroundMusic(SPACE_GAME, true);
		SimpleAudioEngine::getInstance()->playBackgroundMusic(JOE_RIFF1, true);			// 20180202 Change background music
		SimpleAudioEngine::getInstance()->preloadEffect(EXPLOSION_LARGE);
		SimpleAudioEngine::getInstance()->preloadEffect(LASER_SHIP);	
	};

	void explodeFX() {
		SimpleAudioEngine::getInstance()->playEffect(EXPLOSION_LARGE);					// Play explosion sound effect
	}

	void laserFX() {
		SimpleAudioEngine::getInstance()->playEffect(LASER_SHIP);						// Play laser sound effect
	}

private:
	Audio() {};																			// Constructor is private for use as a Singleton. Initializes the variables
	~Audio() {};																		// Private destructor shuts down and cleans up the mixer API

	static Audio* s_pInstance;															// Single instance of Audio used as singleton so only one instance exists thoughout the game

};

#endif // __AUDIO__