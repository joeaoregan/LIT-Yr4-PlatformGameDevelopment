/*
	Game.cpp

	Joe O'Regan
	K00203642

	18/02/2018

	Stuff common to all levels of the game
*/

#include "Game.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"						// For leaderboard and achievements
#endif

Game* Game::s_pInstance;											// Game Singleton

/*
	Update the score of the game
	Register score achievements as score increases
*/
void Game::updateScore(unsigned int set) { 
	m_score += set;													// Add the value passed as a parameter to the current score
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (checkHighScore() && !m_highScore) {
		sdkbox::PluginSdkboxPlay::unlockAchievement("High_Score");	// Achievement for setting a new high score
		m_highScore = true;
	}
	if (m_score >= 100 && !m_score100) {
		sdkbox::PluginSdkboxPlay::unlockAchievement("score100");	// Achievement for scoring 100 points
		m_score100 = true;
	}
	else if (m_score >= 250 && !m_score250) {
		sdkbox::PluginSdkboxPlay::unlockAchievement("score250");	// Achievement for scoring 250 points
		m_score250 = true;
	}
	else if (m_score >= 500 && !m_score500) {
		sdkbox::PluginSdkboxPlay::unlockAchievement("score500");	// Achievement for scoring 500 points
		m_score500 = true;
	}
	else if (m_score >= 1000 && !m_score1000) {
		sdkbox::PluginSdkboxPlay::unlockAchievement("score1000");	// Achievement for scoring 1,000 points
		m_score1000 = true;
	}
	else if (m_score >= 5000 && !m_score5000) {
		sdkbox::PluginSdkboxPlay::unlockAchievement("score5000");	// Achievement for scoring 5,000 pointss
		m_score5000 = true;
	}
	else if (m_score >= 10000 && !m_score10000) {
		sdkbox::PluginSdkboxPlay::unlockAchievement("score10000");	// Achievement for scoring 10,000 points
		m_score10000 = true;
	}
#endif
}				

bool Game::init() {
	// Set timer
	m_levelDuration = LEVEL_TIME_MED;

	if (m_difficulty == EASY) {
		m_levelDuration = LEVEL_TIME_EASY;
	}
	else if (m_difficulty == HARD) {
		m_levelDuration = LEVEL_TIME_HARD;							// Level start time
	}

	//CCLOG("Game: Level Duration =  %f", 
	//	m_levelDuration);

	m_endTime = getTimeTick() + m_levelDuration;					// Set the level finish time

	//CCLOG("Game Endtime: %f", m_endTime);

	m_time = m_levelDuration / 1000;								// Set the time for the countdown timer
	m_currentTime = 0;

	// Initial level values
	if (m_levelNum == 1) m_score = 0;								// Initialise the score value		xxxx resets the score each level
	//m_levelNum = 1;												// Initialise the level value
	m_gameOver = false;

	// Reset totals
	m_asteroidKills = 0;											// Number of asteroids destroyed
	m_enemyShipKills = 0;											// Number of enemy ships destroyed
	m_asteroidCount = 0;											// Number of asteroids spawned
	m_enemyShipCount = 0;											// Number of enemy ships spawnedawned

	//CCLOG("Level %d: Countdown Timer Initialised", 
	//	Game::Instance()->getLevel());
	
	return true;
}

/*
	Increment the number of player lives
*/
void Game::addLife() { 
	if (m_lives <= MAX_PLAYER_LIVES) 								// Increment the number of lives (Max 5)
		m_lives++;
	else if (m_health < 10)											// If the player has the max number of lives
		m_health = 10;												// Increase health

	CCLOG("Player Lives Incremented: %d", m_lives);
}

/*
	Check is the score value the layer achieved a high score
	Use User Defaults to get the current high score
	and compare to players score
*/
bool Game::checkHighScore() {
	// Save Score
	UserDefault* def = UserDefault::getInstance();
	unsigned int highScore = def->getIntegerForKey("Score1", 0);	// Get current saved high score

	if (getScore() > highScore) {									// Compare to players score
		return true;
	}

	return false;
}

/*
	Update the countdown timer
*/
void Game::updateTimer(float curTimeMillis) {
	if (curTimeMillis > m_currentTime) {
		m_currentTime = curTimeMillis + 1000.0f;
		m_time--;													// Decrement the number of seconds
		
		CCLOG("Countdown Timer: %d", m_time);
	}
}

/*
	Get the time since game started
*/
float Game::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) 
		+ (time.tv_usec / 1000);
	return (float)millisecs;
}

/*
// Used for single score saving

// When the game has ended, check if the current score is a high score and save it if it is
void Game::checkHighScore() {
	// Save Score
	UserDefault* def = UserDefault::getInstance();
	unsigned int highScore = def->getIntegerForKey("HIGHSCORE", 0);

	if (getScore() > highScore) {
		highScore = getScore();

		def->setIntegerForKey("HIGHSCORE", highScore);
	}

	def->flush();
}
*/