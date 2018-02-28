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

#define KNUMASTEROIDS 15						// Number of asteroids
#define KNUMASTEROIDSL2 20						// Number of asteroids
#define KNUMLASERS 8							// Number of lasers

class Level : public Layer {
public:
	enum EndReason {
		KENDREASONWIN,														// Player has won the game
		KENDREASONLOSE														// Player has lost the game
	};

	// Level singleton
	static Level* Instance() {
		if (layerInstance == 0) {
			layerInstance = new Level();
			return layerInstance;
		}
		return layerInstance;
	}

    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	void initLasers();														// Create lists of bullets for player and enemies
	void initPowerUps();													// Set up the power ups
	void initAsteroids();													// Add asteroids to the vector list of asteroids
	void initEnemyShips();													// Initialise the list of enemy ships
	void initDifficulty();													// Initialise the game difficulty
	
	virtual void update(float dt);											// Update function
	virtual void endScene(EndReason endReason);
    	
    void menuCloseCallback(cocos2d::Ref* pSender);							// Selector callback, exit the game when button pressed
	
	void getInput();														// Get input from DPad
	
	void checkCollisions();													// 20180202 Check is the game over or not

	void checkGameOver(float currenTime);									// 20180202 Check have game objects collided with each other

	void levelProgression(cocos2d::Label* continueLbl);						// Decide what scene comes next

	// Callbacks
	void restartTapped(Ref* pSender);
	void startLevel2(Ref* pSender);											// 20180218 Progress to the next level
	void startLevel3(Ref* pSender);
	void startLevel4(Ref* pSender);
	void returnToMenu(Ref* pSender);										// 20180218 Return to the main menu

	float randomValueBetween(float low, float high);						// Select a random value from a given range
	void setInvisible(Node * node);											// Hide the node/sprite
		     
    CREATE_FUNC(Level);														// Create the level layer
		
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	// Spawn game objects
	void spawnObjects(float curTimeMillis);								// 20180202 Spawn asteroids
	void spawnEnemyShips(float curTimeMillis);								// 20180214 Spawn enemy ships
	void spawnLasers(int amount);											// 20180221
	void spawnEnemyLaser(cocos2d::Point pos);								// 20180221

	// Fire enemy lasers
	//void enemyFireLaser(float curTimeMillis);

protected:
	// Get / Set methods
	Size getWinSize() { return winSize; }									// Get the window size
	int getNextShipLaser() { return _nextShipLaser; }						// Next laser in laser list
	void setNextShipLaser(int set) { _nextShipLaser = set; }				// Set the next laser in the list
	
	cocos2d::Size visibleSize;												// Screen resolution changes depending on the platform
	cocos2d::Size winSize;													// Current size of the game window (constantly updated)
	float scaleUp, scaleDown;												// Scale objects up in size for 1080p, or down for 720p
	cocos2d::Point origin;
	
	// Menu
	cocos2d::MenuItemImage* closeItem;										// Exit button in bottom right corner
	cocos2d::Menu* menuClose;

	SpriteBatchNode *_batchNode;											// Group nodes together for efficiency
	ParallaxNodeExtras *_backgroundNode;									// Scrolling background
	Player* player;															// Player sprite
	
	Vector<Sprite*>* getLaserList() { return shipLaserList; }					// return ship lasers
		
	MusicPlayer* mplayer;													// Controls for playing/pausing music and skipping tracks
	HUD* newHUD;															// Test hud
	DPad *controller;														// Add directional pad for mobile device

	static Level* layerInstance;											// Single instance of GameScene used as singleton, so only one instance exists thoughout the game
	cocos2d::EventListenerTouchAllAtOnce* touchListener;					// Touch listener

	double _gameOverTime;													// Game over time

	// Time
	float curTimeInit;														// Current game time
	float curTimeMillis;													// Current time in milliseconds

	// End of Level Labels
	cocos2d::Label* levelCompleteLbl;
	cocos2d::MenuItemLabel* restartItem;
	cocos2d::MenuItemLabel* continueItem;

	// Power ups
	cocos2d::Sprite* powerUpLife;											// New life power up
	float powerUpTime;
	float powerUpY;
	bool spawned = false;

	Sprite *playerLife;														// Indicate lives left

	// Asteroids
	int _nextAsteroid = 0;
	float _nextAsteroidSpawn = 0;											// time to spawn next asteroid

	// Enemies
	int nextEnemyShip = 0;
	float nextEnemyShipSpawnTime = 0;										// Time to spawn next enemy ship		

	// Weapons
	int _nextShipLaser = 0;													// Ship laser list index
	int nextEnemyLaser = 0;													// Enemy laser list index

	// Object lists
	Vector<Sprite*>* _asteroids;											// List of asteroids
	Vector<Sprite*>* EnemyShipList;											// List of enemy ships
	Vector <EnemyShip*> * EnemyShips;										// List of enemy ships
	Vector<Sprite*> *shipLaserList;											// List of player lasers
	Vector<Sprite*> *enemyLaserList;										// List of Enemylasers
	Sprite* livesList[MAX_PLAYER_LIVES];									// List of lives

	// Player fire rate
	float m_nextFire;
	unsigned int m_fireRate = 300;											// Set the fire rate millisecond interval depending on the difficult 200: easy, 300: normal, 400: hard
	float m_powerUpDuration = 3.5f;											// How long the power up will remain on screen in seconds (Easy: 5, Medium: 3.5, Hard: 2)

private:

};

#endif // __LEVEL_H__
