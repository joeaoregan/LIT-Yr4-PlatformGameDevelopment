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
#include "Defines.h"


class Game {
public:	
	// Game singleton
	static Game* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Game();
			return s_pInstance;
		}
		return s_pInstance;
	}
		    
    virtual bool init();												// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

	//void checkHighScore();											// When the game has ended, check if the current score is a high score and save it if it is
	bool checkHighScore();												// When the game has ended, check if the current score is a high score and save it if it is

	void updateTimer(float curTimeMillis);								// 20180204 Update the countdown timer, 21/02/2018 Passing curTimeMillis solves Android timer issue

	// Get
	float getTimeTick();												// Get current time in milliseconds

	unsigned int getLevel() const { return m_levelNum; }				// 20180218 Return the current level
	unsigned int getScore() const { return m_score; }					// 20180214 Return the current score
	unsigned int getLives() const { return m_lives; }					// 20180218 Return the players number of lives
	unsigned int getHealth() const { return m_health; }					// Return the players health
	std::string getPlayerName() const { return m_playerName; }			// 20180221 Return the name of the player
	unsigned int getTimer() const { return m_time; }					// Get the time on the countdown timer
	bool isGameOver() const { return m_gameOver; }						// Check is the game over or not
	unsigned int getAsteroidKills() const { return m_asteroidKills; }	// The total number of asteroids destroyed in the game
	unsigned int getEnemyShipKills() const { return m_enemyShipKills; }	// The total number of enemy ships destroyed
	unsigned int getAsteroidCount() const { return m_asteroidCount; }	// The total number of asteroids spawned in the game
	unsigned int getEnemyShipCount() const { return m_enemyShipCount; }	// The total number of enemy ships spawned
	bool musicPlayerVisible() const { return m_PlayerVisible; }			// Is the music player to be hidden or not
	unsigned int getDifficulty() const{ return m_difficulty; }			// Get the difficulty setting
	float getLevelDuration() const { return m_levelDuration; }			// Get the level end time
	unsigned int getEndTime() const { return m_endTime; }				// Get the level end time
	bool getLivesCarried() const { return m_carryLives; }				// Are the lives carried over to the next level or not
	float getCurrentWeapon() const { return m_currentWeapon; }			// Get the stored weapon level
	float getButtonTimer() const { return m_nextTime; }					// Time between button presses
	bool getWon() const { return m_won;	}								// Has the game been won

	// Set
	void setLevel(unsigned int set) { m_levelNum = set; }				// Set the level number
	void setNextLevel() { 
		if (m_levelNum < MAX_LEVELS) m_levelNum++; 						// Set the next level number
		else m_levelNum = 1;											// Loop back around to level 1
	}	
	void updateScore(unsigned int set) { m_score += set; }				// Add the value passed as a parameter to the current score
	void setLives(unsigned int set) { m_lives = set; }					// Set the number of lives
	void setHealth(unsigned int set) { m_health = set; }				// Set the Players Health
	void takeLife() { m_lives--; };										// Decrement the number of lives
	void takeHealth() { m_health--; }
	void addLife();														// Increment the number of lives (Max 5)
	void setPlayerName(std::string set) { m_playerName = set; }			// Set the player name
	void setTimer(unsigned int set) { m_time = set; }					// Set the countdown timer
	void setGameOver(bool set) { m_gameOver = set;  }					// Set the game as over or not
	// Kills
	void incrementAsteroidKills() { m_asteroidKills++; }				// Increase the number of asteroids destroyed by 1
	void resetAsteroidKills() { m_asteroidKills = 0; }					// Reset the number of asteroids destroyed
	void incrementEnemyShipKills() { m_enemyShipKills++; }				// Increase the number of enemy ships destroyed by 1
	void resetEnemyShipKIlls() { m_enemyShipKills = 0; }				// Reset the number of enemy ships destroyed
	// Count
	void incrementAsteroidCount() { m_asteroidCount++; }				// Increase the number of asteroids spawned by 1
	void resetAsteroidCount() { m_asteroidCount = 0; }					// Reset the number of asteroids spawned
	void incrementEnemyShipCount() { m_enemyShipCount++; }				// Increase the number of enemy ships spawned by 1
	void resetEnemyShipCount() { m_enemyShipCount = 0; }				// Reset the number of enemy ships spawned
	void hideMusicPlayer(bool set) { m_PlayerVisible = set;  }			// Show or hide the music player
	void setDifficulty(unsigned int set) { m_difficulty = set; }		// Set the difficulty level
	void setLivesCarried(bool set) { m_carryLives = set; }				// Carry over lives to next level
	void setCurrentWeapon(float set) { m_currentWeapon = set; }			// Save the players current weapon
	void setButtonTimer(float set) { m_nextTime = set; }				// Set the time between button presses
	void setWon(bool set) { m_won = set; }								// The game has been won/lost

private:
	float m_nextTime;													// Used for menu button rate

	// Player
	std::string m_playerName;											// Current player name
	unsigned int m_currentWeapon;										// Store weapons grade between levels

	// HUD
	unsigned int m_lives = 0;											// Player lives
	unsigned int m_health = 10;											// Player health
	unsigned int m_score = 0;											// Current score
	unsigned int m_levelNum = 1;										// Current level

	static Game* s_pInstance;											// Single instance of Game used as singleton, so only one instance exists thoughout the game

	// Time
	unsigned int m_time;												// Countdown timer value
	float m_currentTime;												// 20180221 Change to float to fix Android timer issue
	float m_levelDuration;												// Level start time
	float m_endTime;													// Level end time

	// Music
	bool m_PlayerVisible = true;										// Show/Hide the music player in main menu and level scenes

	// Total kills
	unsigned int m_asteroidKills = 0;									// Number of asteroids destroyed
	unsigned int m_enemyShipKills = 0;									// Number of enemy ships destroyed
	unsigned int m_asteroidCount = 0;									// Number of asteroids spawned
	unsigned int m_enemyShipCount = 0;									// Number of enemy ships spawned
	
	// Settings
	bool m_gameOver = false;											// Has the game been won or lost
	unsigned int m_difficulty = MEDIUM;									// The game difficulty level, default value of MEDIUM

	bool m_carryLives = false;											// Carry over lives to next level (For testing)

	bool m_won = false;													// Destroyed enemy boss winning the game.
};

#endif // __GAME_SCENE_H__
