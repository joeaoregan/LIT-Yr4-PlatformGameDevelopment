/*
	Game.cpp

	Joe O'Regan
	K00203642

	18/02/2018

	Stuff common to all levels of the game
*/

#include "Game.h"

Game* Game::s_pInstance;							// Game Singleton

bool Game::init() {
	// Set timer
	m_levelDuration = LEVEL_TIME_MED;


	if (m_difficulty == EASY) {
		m_levelDuration = LEVEL_TIME_EASY;
	}
	else if (m_difficulty == HARD) {
		m_levelDuration = LEVEL_TIME_HARD;			// Level start time
	}

	//CCLOG("Game: Level Duration =  %f", 
	//	m_levelDuration);

	m_endTime = getTimeTick() + m_levelDuration;	// Set the level finish time

	//CCLOG("Game Endtime: %f", m_endTime);

	m_time = m_levelDuration / 1000;				// Set the time for the countdown timer
	m_currentTime = 0;

	// Initial level values
	m_score = 0;									// Initialise the score value
	m_level = 1;									// Initialise the level value
	m_gameOver = false;

	// Reset totals
	m_asteroidKills = 0;							// Number of asteroids destroyed
	m_enemyShipKills = 0;							// Number of enemy ships destroyed
	m_asteroidCount = 0;							// Number of asteroids spawned
	m_enemyShipCount = 0;							// Number of enemy ships spawnedawned

	//CCLOG("Level %d: Countdown Timer Initialised", 
	//	Game::Instance()->getLevel());
	
	return true;
}

void Game::addLife() { 
	if (m_lives <= MAX_PLAYER_LIVES) 				// Increment the number of lives (Max 5)
		m_lives++; 
	CCLOG("Player Lives Incremented: %d", m_lives);
}

bool Game::checkHighScore() {
	// Save Score
	UserDefault* def = UserDefault::getInstance();
	unsigned int highScore = def->getIntegerForKey("Score1", 0);

	if (getScore() > highScore) {
		return true;
	}

	return false;
}

void Game::updateTimer(float curTimeMillis) {
	if (curTimeMillis > m_currentTime) {
		m_currentTime = curTimeMillis + 1000.0f;
		m_time--;

		CCLOG("Countdown Timer: %d", m_time);
	}
}

float Game::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
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