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
#include "Game.h"

USING_NS_CC;


#define KNUMASTEROIDS 15						// Number of asteroids
#define KNUMASTEROIDSL2 20						// Number of asteroids
#define KNUMLASERS 5							// Number of lasers

class Level : public Layer {
public:
	/*
	// Level singleton
	static Level* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Level();
			return s_pInstance;
		}
		return s_pInstance;
	}
	*/

    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	
	virtual void update(float dt);
    	
    void menuCloseCallback(cocos2d::Ref* pSender);							// Selector callback, exit the game when button pressed

	float getTimeTick();													// Get current time in milliseconds

	void getInput();														// Get input from DPad

	void updateTimer(float curTimeMillis);									// 20180204 Update the countdown timer, 21/02/2018 Passing curTimeMillis solves Android timer issue

	void checkCollisions();													// 20180202 Check is the game over or not

	//void checkGameOver(float currenTime);									// 20180202 Check have game objects collided with each other

	void startLevel2(Ref* pSender);											// 20180218 Progress to the next level
	void startLevel3(Ref* pSender); 
	void returnToMenu(Ref* pSender);										// 20180218 Return to the main menu

	float randomValueBetween(float low, float high);						// Select a random value from a given range
	void setInvisible(Node * node);											// Hide the node/sprite

     
    CREATE_FUNC(Level);														// implement the "static create()" method manually
	/*
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	void spawnLaser();														// 20180205
	void spawn2Lasers();													// 20180205

	void spawnAsteroids(float curTimeMillis);								// 20180202 Spawn asteroids
	void spawnEnemyShips(float curTimeMillis);								// 20180214 Spawn enemy ships
	*/

	enum EndReason {
		KENDREASONWIN,
		KENDREASONLOSE
	};

	cocos2d::Size visibleSize;
	Size winSize;															// Current size of the game window (constantly updated)
	cocos2d::Point origin;
	
	// Menu
	cocos2d::MenuItemImage* closeItem;										// Exit button in bottom right corner
	cocos2d::Menu* menuClose;

	SpriteBatchNode *_batchNode;											// Group nodes together for efficiency
	Player* player;															// Player sprite

	ParallaxNodeExtras *_backgroundNode;									// Scrolling background

	Vector<Sprite*>* _asteroids;											// List of asteroids
	Vector<Sprite*>* EnemyShipList;											// List of enemy ships
	Vector<Sprite*> *_shipLasers;											// List of lasers
	Sprite* livesList[MAX_LIVES];											// List of lives
	int _nextShipLaser = 0;													// Ship laser list index

	cocos2d::EventListenerTouchAllAtOnce* touchListener;					// Touch listener

	// Time
	float currentTime;														// 20180221 Change to float to fix Android timer issue
	double curTime;															// Current game time
	double _gameOverTime;													// Game over time
	float curTimeMillis;													// Current time in milliseconds
	unsigned int time;

	// HUD
	cocos2d::Label* scoreLabel;												// Display the current score
	cocos2d::Label * timeLabel;												// Display the time remaining

	DPad *controller;														// Add directional pad for mobile device

	bool _gameOver = false;

	cocos2d::Scene* scene;// 'scene' is an autorelease object
	Level* layer;			// 'layer' is an autorelease object

private:
	/*
	void endScene(EndReason1 endReason);
	void restartTapped(Ref* pSender);

	Sprite *playerLife;														// Indicate lives left
	Sprite *EnemyShip;
	int _nextAsteroid = 0;
	int nextEnemyShip = 0;
	float _nextAsteroidSpawn = 0;											// time to spawn next asteroid
	float nextEnemyShipSpawnTime = 0;										// Time to spawn next enemy ship
	*/

	//static Level* s_pInstance;											// Single instance of GameScene used as singleton, so only one instance exists thoughout the game
};

#endif // __LEVEL_H__
