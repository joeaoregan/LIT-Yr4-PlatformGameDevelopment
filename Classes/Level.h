/*
	Level.h

	Joe O'Regan
	K00203642
	21/02/2018

	Base class for levels
	Controls spawning, collisions, end of game/level conditions
*/

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"
#include "Player.h"
#include "EnemyShip.h"
#include "Game.h"
#include "MusicPlayer.h"
#include "Asteroid.h"
#include "PowerUp.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"											// For leaderboard and achievements
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"								// 20180307 Google Analytics
#endif

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

    static cocos2d::Scene* createScene();										// there's no 'id' in SpaceQuest, so we recommend returning the class instance pointer
	
    virtual bool init();														// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	void initLasers();															// Create lists of bullets for player and enemies
	void initPowerUps();														// Set up the power ups
	void initAsteroids();														// Add asteroids to the vector list of asteroids
	virtual void initEnemyShips();												// Initialise the list of enemy ships
	void initDifficulty();														// Initialise the game difficulty
	
	virtual void update(float dt);												// Update function
	virtual void endScene(EndReason endReason);									// End the scene
	virtual void checkCollisions();												// 20180202 Check is the game over or not
	    	
    void menuCloseCallback(cocos2d::Ref* pSender);								// Selector callback, exit the game when button pressed
	
	void getInput();															// Get input from DPad
	
	virtual void checkGameOver(float currenTime);								// 20180202 Check have game objects collided with each other

	void levelProgression(cocos2d::Label* continueLbl);							// Decide what scene comes next

	// Callbacks
	void restartTapped(cocos2d::Ref* pSender);
	void goToStoryScreen(cocos2d::Ref* pSender);
	
	void returnToMenu(cocos2d::Ref* pSender);									// 20180218 Return to the main menu

	float randomValueBetween(float low, float high);							// Select a random value from a given range
	void setInvisible(cocos2d::Node * node);									// Hide the node/sprite
		     
    CREATE_FUNC(Level);															// Create the level layer
		
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	// Spawn game objects
	void spawnObjects(float curTimeMillis);										// 20180202 Spawn asteroids
	virtual void spawnEnemyShips(float curTimeMillis);							// 20180214 Spawn enemy ships (Changes in Level 4)
	void spawnLasers(int amount);												// 20180221
	
	void spawnEnemyLaser(cocos2d::Point pos, int type = BLUE);					// Spawn enemy lasers, default is blue laser
	
	// Get / Set methods
	int getNextShipLaser() const { return m_nextShipLaser; }					// Next laser in laser list
	void setNextShipLaser(int set) { m_nextShipLaser = set; }					// Set the next laser in the list
	cocos2d::Vector<cocos2d::Sprite*>* const getLaserList() { 
		return m_playerLaserList; }												// return ship lasers list
	
	void initLives();															// Moved to Level base class to test all levels

	void PowerUpCollision(PowerUp* powerUp);									// Check collision with power ups
	
	void statBarEOL(float pc, int elements, float y);							// End of level stat bar, percentage, number of elements on screen, y position

	void updateLeaderboard();													// SDKBox leaderboard

	cocos2d::Vec2 getAnimation(const char* name)
	{
		
	}

	void killAchievement();														// Achievement for reaching a certain percentage of kills

	void endLevelAchievement(EndReason endReason);
	
protected:
	cocos2d::Size m_visibleSize;												// Screen resolution changes depending on the platform
	cocos2d::Size m_winSize;													// Current size of the game window (constantly updated)
	float m_scaleUp, m_scaleDown;												// Scale objects up in size for 1080p, or down for 720p
	cocos2d::Point m_Origin;													// Screen origin point
	
	// Background
	cocos2d::SpriteBatchNode *m_batchNode;										// Group nodes together for efficiency
	cocos2d::SpriteBatchNode *m_batchNodeJOE;									// Group nodes together for efficiency
	ParallaxNodeExtras *m_backgroundNode;										// Scrolling background

	// Objects
	Player* player;																// Player sprite		
	MusicPlayer* m_pMPlayer;													// Controls for playing/pausing music and skipping tracks
	HUD* m_pHUD;																// Test hud
	DPad *m_pController;														// Add directional pad for mobile device

	//auto listener = EventListenerTouchAllAtOnce::create();
	static Level* s_pLayerInstance;												// Single instance of GameScene used as singleton, so only one instance exists thoughout the game
	cocos2d::EventListenerTouchAllAtOnce* m_touchListener;						// Touch listener

	// Time
	//float curTimeInit;														// Current game time
	float m_curTimeMillis;														// Current time in milliseconds

	// End of Level Labels
	cocos2d::Label* m_pLevelCompleteLbl;										// Level finished win/lose message

	cocos2d::Label* restartLbl;

	cocos2d::MenuItemLabel* m_pRestartItem;										// Restart the game button
	cocos2d::MenuItemLabel* m_pContinueItem;									// Continue to next level button

	// Power ups
	PowerUp* m_pPowerUpLife;													// New life power up
	PowerUp* m_pPowerUpWeapon;													// Weapon upgrade power up

	bool m_spawned = false;														// Is the power up spawned already

	// Asteroids
	int m_nextAsteroid = 0;														// Asteroid list index
	float m_nextAsteroidSpawn = 0;												// time to spawn next asteroid

	// Enemies
	unsigned int m_nextEnemyShip = 0;											// Next in the list of enemy ships
	float m_nextEnemyShipSpawnTime = 0;											// Time to spawn next enemy ship	
	unsigned int m_numEnemyShips = 3;											// Number of enemy ships

	// Weapons
	int m_nextShipLaser = 0;													// Ship laser list index
	int m_nextEnemyLaser = 0;													// Enemy laser list index

	// Object lists
	cocos2d::Vector<Asteroid*> * m_asteroidsList;								// List of asteroids
	cocos2d::Vector<EnemyShip*> * m_enemyShipList;								// List of enemy ships
	cocos2d::Vector<cocos2d::Sprite*> *m_playerLaserList;						// List of player lasers
	cocos2d::Vector<cocos2d::Sprite*> *m_enemyLaserList1;						// List of Enemylasers
	cocos2d::Vector<cocos2d::Sprite*> *m_enemyLaserList2;						// List of lasers for 2nd enemy
	cocos2d::Vector<cocos2d::Sprite*> *m_enemyLaserList3;						// List of lasers for 3rd enemy
	
	// Player fire rate
	float m_nextFire;
	unsigned int m_fireRate = 300;												// Set the fire rate millisecond interval depending on the difficult 200: easy, 300: normal, 400: hard
	float m_powerUpDuration = 3.5f;												// How long the power up will remain on screen in seconds (Easy: 5, Medium: 3.5, Hard: 2)
};

#endif // __LEVEL_H__
