/*
	Game.h

	Joe O'Regan
	K00203642

	18/02/2018

	Stuff common to all levels of the game, that can be accessed from menus etc.
*/
#ifndef __GAME_H__
#define __GAME_H__

#include "ParallaxNodeExtras.h"
#include "Audio.h"
#include "DPad.h"
#include "HUD.h"

#define LEVEL_TIME 3000										// 30 Seconds until level is complete
#define MAX_LIVES 5												// The maximum number of lives a player can have

#define RES_720P 720
#define RES_1080P 1080

class Game : public Layer {
public:	
	// Game singleton
	static Game* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Game();
			return s_pInstance;
		}
		return s_pInstance;
	}
		    
    virtual bool init();										// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	
	void checkHighScore();										// When the game has ended, check if the current score is a high score and save it if it is

	void updateTimer(float curTimeMillis);						// 20180204 Update the countdown timer, 21/02/2018 Passing curTimeMillis solves Android timer issue
	
	// Get
	unsigned int getLevel() { return level; }					// 20180218 Return the current level
	unsigned int getScore() { return score; }					// 20180214 Return the current score
	unsigned int getLives() { return _lives; }					// 20180218 Return the players number of lives
	std::string getPlayerName() { return playerName; }			// 20180221 Return the name of the player
	unsigned int getTimer() { return time; }					// Get the time on the countdown timer
	bool isGameOver() { return _gameOver; }						// Check is the game over or not
	unsigned int getAsteroidKills() { return asteroidKills; }	// The total number of asteroids destroyed in the game
	unsigned int getEnemyShipKills() { return enemyShipKills; }	// The total number of enemy ships destroyed
	unsigned int getAsteroidCount() { return asteroidCount; }	// The total number of asteroids spawned in the game
	unsigned int getEnemyShipCount() { return enemyShipCount; }	// The total number of enemy ships spawned

	// Set
	void setLevel(unsigned int set) { level = set; }			// Set the level number
	void updateScore(unsigned int set) { score += set; }		// Add the value passed as a parameter to the current score
	void setLives(unsigned int set) { _lives = set; }			// Set the number of livess
	void takeLife() { _lives--; };								// Decrement the number of lives
	void addLife() { if (_lives <= MAX_LIVES) _lives++; };		// Increment the number of lives (Max 5)
	void setPlayerName(std::string set) { playerName = set; }	// Set the player name
	void setTimer(unsigned int set) { time = set; }				// Set the countdown timer
	void setGameOver(bool set) { _gameOver = set;  }			// Set the game as over or not
	// Kills
	void incrementAsteroidKills() { asteroidKills++; }			// Increase the number of asteroids destroyed by 1
	void resetAsteroidKills() { asteroidKills = 0; }			// Reset the number of asteroids destroyed
	void incrementEnemyShipKills() { enemyShipKills++; }		// Increase the number of enemy ships destroyed by 1
	void resetEnemyShipKIlls() { enemyShipKills = 0; }			// Reset the number of enemy ships destroyed
	// Count
	void incrementAsteroidCount() { asteroidCount++; }			// Increase the number of asteroids spawned by 1
	void resetAsteroidCount() { asteroidCount = 0; }			// Reset the number of asteroids spawned
	void incrementEnemyShipCount() { enemyShipCount++; }		// Increase the number of enemy ships spawned by 1
	void resetEnemyShipCount() { enemyShipCount = 0; }			// Reset the number of enemy ships spawned

private:
	unsigned int _lives = 0;									// Player lives

	std::string playerName;										// Current player name
	unsigned int score;											// Current score
	unsigned int level;											// Current level

	static Game* s_pInstance;									// Single instance of Game used as singleton, so only one instance exists thoughout the game

	unsigned int time;											// Countdown timer value
	float currentTime;											// 20180221 Change to float to fix Android timer issue

	// Total kills
	unsigned int asteroidKills = 0;								// Number of asteroids destroyed
	unsigned int enemyShipKills = 0;							// Number of enemy ships destroyed
	unsigned int asteroidCount = 0;								// Number of asteroids spawned
	unsigned int enemyShipCount = 0;							// Number of enemy ships spawned
	
	bool _gameOver = false;										// Has the game been won or lost
};

#endif // __GAME_SCENE_H__
