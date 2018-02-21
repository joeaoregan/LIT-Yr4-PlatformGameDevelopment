/*
	Game.cpp

	Joe O'Regan
	K00203642

	18/02/2018

	Stuff common to all levels of the game
*/

#include "Game.h"

Game* Game::s_pInstance;	// Game Singleton

bool Game::init() {
	score = 0;				// Initialise the score value
	level = 1;				// Initialise the level value
	
	return true;
}

// When the game has ended, check if the current score is a high score and save it if it is
void Game::checkHighScore() {
	// Save Score
	UserDefault* def = UserDefault::getInstance();
	int highScore = def->getIntegerForKey("HIGHSCORE", 0);

	if (getScore() > highScore) {
		highScore = getScore();

		def->setIntegerForKey("HIGHSCORE", highScore);
	}

	def->flush();
}

void Game::updateTimer(float curTimeMillis) {
	if (curTimeMillis > currentTime) {
		currentTime = curTimeMillis + 1000.0f;
		time--;
	}
	//timeLabel->setString("Time: " + to_string(time));
}