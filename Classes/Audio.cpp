/*
	Audio.cpp

	Joe O'Regan
	K00203642

	Game audio class, with singleton access
	Cross-platform, plays different type of sound file, depending on the platform compiled for
*/

#include "Audio.h"
//#include "string.h"

Audio* Audio::s_pInstance;																	// Singleton so only one instance of Audio exists in the game, for easy access


#define MAX_VOL 1.0f
#define MIN_VOL 0.0f
#define NUM_TRACKS 3

int currentTrack = 0;
const char* tracks[] = { "joe_riff1.wav" , "joe_riff2.wav", "BloodLevel-JOR-NEW.wav" };
std::string trackNames[] = { "Track 1: A song by Joe", "Track 2: Another song by Joe", "Track 3: Mostly Joe, guitar solo by Jimmy" };

void Audio::init() {
	m_currentVol = 1.0f;

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())						// If the background music isn't already playing
		SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[0], true);				// 20180202 Change background music

	// Preload the sound effects
	SimpleAudioEngine::getInstance()->preloadEffect(EXPLOSION_LARGE);
	SimpleAudioEngine::getInstance()->preloadEffect(EXPLOSION_PLAYER);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_SHIP);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_ENEMY1);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_ENEMY2);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_ENEMY4);
	SimpleAudioEngine::getInstance()->preloadEffect(BUTTON_FX);
	SimpleAudioEngine::getInstance()->preloadEffect(POWER_UP);
//#endif
}

/*
	Play the game effects
*/
void Audio::playFX(const char* effect) {
	SimpleAudioEngine::getInstance()->playEffect(effect);								// Play an effect
}
/*
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
void Audio::damagePlayerFX() {
	SimpleAudioEngine::getInstance()->playEffect(EXPLOSION_PLAYER, false, 0.5f);		// Raise pitch
	//SimpleAudioEngine::getInstance()->playEffect(EXPLOSION_PLAYER);							// Play explosion sound effect
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
*/

/*
	Play the music
*/
void Audio::play() {
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

/*
	Pause the music
*/
void Audio::pause() {
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

/*
	Play the previous track in the array
*/
void Audio::skipTrackForwards() {
	//if (currentTrack < NUM_TRACKS - 1)
	//	currentTrack++;
	//else currentTrack = 0;
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	(currentTrack < NUM_TRACKS - 1) ? currentTrack++ : currentTrack = 0;

	SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[currentTrack], true);		// 20180221 skip audio track
}

/*
	Play the next track
*/
void Audio::skipTrackBackwards() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	(currentTrack == 0) ? currentTrack = NUM_TRACKS - 1 : currentTrack--;

	SimpleAudioEngine::getInstance()->playBackgroundMusic(tracks[currentTrack], true);		// 20180221 skip audio track
}

/*
	Increase the music volume
*/
void Audio::musicVolIncrease() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	if (m_currentVol < MAX_VOL)
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_currentVol + 0.1f);
}

/*
	Decrease the music volume
*/
void Audio::musicVolDecrease() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	if (m_currentVol > MIN_VOL)
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_currentVol - 0.1f);
}

/*
	Increase the SFX volume
*/
void Audio::sfxVolIncrease() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	if (m_currentVol < MAX_VOL)
		SimpleAudioEngine::getInstance()->setEffectsVolume(m_currentVol + 0.1f);
}

/*
	Decrease the SFX volume
*/
void Audio::sfxVolDecrease() {
	SimpleAudioEngine::getInstance()->playEffect(BUTTON_FX);								// Play button sfx
	if (m_currentVol > MIN_VOL)
		SimpleAudioEngine::getInstance()->setEffectsVolume(m_currentVol - 0.1f);
}

/*
	Get the name of the track from the trackNames array
*/ 
std::string Audio::getTrackName() {
	return trackNames[currentTrack];														// Returns the name of the track
}