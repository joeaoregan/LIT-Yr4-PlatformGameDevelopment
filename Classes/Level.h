/*
	Level.h

	Joe O'Regan
	K00203642
	21/02/2018

	Base class for levels
*/
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"
#include "Player.h"
#include "EnemyShip.h"
#include "Game.h"
#include "MusicPlayer.h"

class Level : public Layer {
public:
	enum EndReason {
		KENDREASONWIN,															// Player has won the game
		KENDREASONLOSE															// Player has lost the game
	};

	// Level singleton
	static Level* Instance() {
		if (s_pLayerInstance == 0) {
			s_pLayerInstance = new Level();
			return s_pLayerInstance;
		}
		return s_pLayerInstance;
	}

    static cocos2d::Scene* createScene();										// there's no 'id' in cpp, so we recommend returning the class instance pointer
	
    virtual bool init();														// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	void initLasers();															// Create lists of bullets for player and enemies
	void initPowerUps();														// Set up the power ups
	void initAsteroids();														// Add asteroids to the vector list of asteroids
	void initEnemyShips();														// Initialise the list of enemy ships
	void initDifficulty();														// Initialise the game difficulty
	
	virtual void update(float dt);												// Update function
	virtual void endScene(EndReason endReason);
    	
    void menuCloseCallback(cocos2d::Ref* pSender);								// Selector callback, exit the game when button pressed
	
	void getInput();															// Get input from DPad
	
	void checkCollisions();														// 20180202 Check is the game over or not

	void checkGameOver(float currenTime);										// 20180202 Check have game objects collided with each other

	void levelProgression(cocos2d::Label* continueLbl);							// Decide what scene comes next

	// Callbacks
	void restartTapped(Ref* pSender);
	void startLevel2(Ref* pSender);												// 20180218 Progress to the next level
	void startLevel3(Ref* pSender);
	void startLevel4(Ref* pSender);
	void returnToMenu(Ref* pSender);											// 20180218 Return to the main menu

	float randomValueBetween(float low, float high);							// Select a random value from a given range
	void setInvisible(Node * node);												// Hide the node/sprite
		     
    CREATE_FUNC(Level);															// Create the level layer
		
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	// Spawn game objects
	void spawnObjects(float curTimeMillis);										// 20180202 Spawn asteroids
	void spawnEnemyShips(float curTimeMillis);									// 20180214 Spawn enemy ships
	void spawnLasers(int amount);												// 20180221
	void spawnEnemyLaser(cocos2d::Point pos);									// 20180221
	void spawnEnemyLaserAngled(cocos2d::Point a, cocos2d::Point b, float angle);// 20180302 fire from point A to point B, rotate the laser to face the direction its going

	// Fire enemy lasers
	//void enemyFireLaser(float curTimeMillis);

	// Get / Set methods
	Size getWinSize() { return winSize; }										// Get the window size
	int getNextShipLaser() { return _nextShipLaser; }							// Next laser in laser list
	void setNextShipLaser(int set) { _nextShipLaser = set; }					// Set the next laser in the list
	Vector<Sprite*>* getLaserList() { return m_playerLaserList; }				// return ship lasers
	
	void initLives();															// Moved to Level base class to test all levels

protected:
	cocos2d::Size visibleSize;													// Screen resolution changes depending on the platform
	cocos2d::Size winSize;														// Current size of the game window (constantly updated)
	float scaleUp, scaleDown;													// Scale objects up in size for 1080p, or down for 720p
	cocos2d::Point origin;														// Screen origin point
	
	// Background
	SpriteBatchNode *m_batchNode;												// Group nodes together for efficiency
	ParallaxNodeExtras *m_backgroundNode;										// Scrolling background

	// Objects
	Player* player;																// Player sprite		
	MusicPlayer* mplayer;														// Controls for playing/pausing music and skipping tracks
	HUD* newHUD;																// Test hud
	DPad *controller;															// Add directional pad for mobile device

	static Level* s_pLayerInstance;												// Single instance of GameScene used as singleton, so only one instance exists thoughout the game
	cocos2d::EventListenerTouchAllAtOnce* touchListener;						// Touch listener

	// Time
	float curTimeInit;															// Current game time
	float curTimeMillis;														// Current time in milliseconds

	// End of Level Labels
	cocos2d::Label* levelCompleteLbl;											// Level finished win/lose message
	cocos2d::MenuItemLabel* restartItem;										// Restart the game button
	cocos2d::MenuItemLabel* continueItem;										// Continue to next level button

	// Power ups
	cocos2d::Sprite* m_powerUpLife;												// New life power up
	float powerUpTime;
	float powerUpY;
	bool spawned = false;

	// Asteroids
	int _nextAsteroid = 0;
	float _nextAsteroidSpawn = 0;												// time to spawn next asteroid

	// Enemies
	unsigned int nextEnemyShip = 0;												// Next in the list of enemy ships
	float nextEnemyShipSpawnTime = 0;											// Time to spawn next enemy ship	
	unsigned int m_numEnemyShips = 3;											// Number of enemy ships

	// Weapons
	int _nextShipLaser = 0;														// Ship laser list index
	int m_nextEnemyLaser = 0;													// Enemy laser list index

	// Object lists
	Vector<Sprite*>* m_asteroidsList;											// List of asteroids
	Vector <EnemyShip*> * m_enemyShipList;										// List of enemy ships
	Vector<Sprite*> *m_playerLaserList;											// List of player lasers
	Vector<Sprite*> *m_enemyLaserList;											// List of Enemylasers

	// Player fire rate
	float m_nextFire;
	unsigned int m_fireRate = 300;												// Set the fire rate millisecond interval depending on the difficult 200: easy, 300: normal, 400: hard
	float m_powerUpDuration = 3.5f;												// How long the power up will remain on screen in seconds (Easy: 5, Medium: 3.5, Hard: 2)

private:

};

#endif // __LEVEL_H__
