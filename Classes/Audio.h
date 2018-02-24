/*
	Audio.h

	Joe O'Regan
	K00203642

	Game audio class, with singleton access
*/

#ifndef __AUDIO__
#define __AUDIO__

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;			// Audio engine

#include <string>

class Audio {
public:	
	static Audio* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Audio();
			return s_pInstance;
		}
		return s_pInstance;				// Return singleton instance
	}

	void init();						// Play music and preload effects

	void explodeFX();					// Play explosion sound effect
	void laserFX();						// Play laser sound effect

	void skipTrackForwards();
	void play();
	void pause();
	void skipTrackBackwards();
	void musicVolIncrease();
	void musicVolDecrease();
	void sfxVolIncrease();
	void sfxVolDecrease();
	std::string getTrackName();
	bool isPaused();					// If music paused, show play button on music player

private:
	Audio() {};							// Constructor is private for use as a Singleton. Initializes the variables
	~Audio() {};						// Private destructor shuts down and cleans up the mixer API

	static Audio* s_pInstance;			// Single instance of Audio used as singleton so only one instance exists thoughout the game

	float currentVol;					// Current music volume
};

#endif // __AUDIO__