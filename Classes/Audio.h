/*
	Audio.h

	Joe O'Regan
	K00203642

	Game audio class, with singleton access
*/

#ifndef __AUDIO__
#define __AUDIO__

// IOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	// WAV audio format
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)	// WAV audio format
// Windows / Android
//#define JOE_RIFF1 "joe_riff1.wav"
#define JOE_RIFF1 "joe_riff1.ogg"
//#define JOE_RIFF2 "joe_riff2.wav"
#define JOE_RIFF2 "joe_riff2.ogg"					// Large file size google
//#define JOE_RIFF3 "BloodLevel-JOR-NEW.wav"
#define JOE_RIFF3 "BloodLevel-JOR-NEW.ogg"

// Remember to preload!!!!
#define BUTTON_FX "buttonClick.wav"
#define EXPLOSION_LARGE "explosion_large.wav"
#define DAMAGE_PLAYER "damage.wav"
#define EXPLOSION_PLAYER "explosion.wav"
#define LASER_SHIP "laser_ship.wav"
#define LASER_ENEMY1 "laserEnemy1.wav"
#define LASER_ENEMY2 "laserEnemy2.wav"
#define LASER_ENEMY3 "laserEnemy3.wav"
#define LASER_ENEMY4 "laserEnemy4.wav"
#define POWER_UP "powerup.wav"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)												// CAF: Core audio format
#define SPACE_GAME "SpaceGame.caf"
#define EXPLOSION_LARGE "explosion_large.caf"
#define LASER_SHIP "laser_ship.caf"
#endif


#include "SimpleAudioEngine.h"
#include <string>
using namespace CocosDenshion;			// Audio engine

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

	void playFX(const char* effect);	// Play an effect
	/*
	void explodeFX();					// Play explosion sound effect
	void explodePlayerFX();				// Play player explosion sound effect
	void damagePlayerFX();				// Play player damage sound effect
	void laserFX();						// Play laser sound effect
	void laserFXEnemy();				// Play laser sound effect for enemy ship
	void powerUpFX();					// Play power up sound effect
	void selectMenuOption();			// Play button sound
	*/
	void skipTrackForwards();			// Skip to the next track in the array, or first track if already at the last
	void skipTrackBackwards();			// Skip to the previous track in the array, or last track if already at the 1st
	void play();						// Play the background music
	void pause();						// Pause the background music

	// Volumes (Not working windows)
	void musicVolIncrease();			// Increase the music volume
	void musicVolDecrease();			// Decrease the music volume
	void sfxVolIncrease();				// Increase the FX volume
	void sfxVolDecrease();				// Decrease the FX volume

	std::string getTrackName();			// Get the name of the track
	bool isPaused();					// If music paused, show play button on music player

private:
	Audio() {};							// Constructor is private for use as a Singleton. Initializes the variables
	~Audio() {};						// Private destructor shuts down and cleans up the mixer API

	static Audio* s_pInstance;			// Single instance of Audio used as singleton so only one instance exists thoughout the game

	float m_currentVol;					// Current music volume
};

#endif // __AUDIO__