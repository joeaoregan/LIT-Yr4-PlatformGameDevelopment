/*
	Game.h

	Joe O'Regan
	K00203642

	18/02/2018

	Stuff common to all levels of the game
*/
#ifndef __GAME_H__
#define __GAME_H__

//#include "cocos2d.h"
#include "ParallaxNodeExtras.h"
//#include <sstream>														// 20180204 score
//#include "Player.h"
#include "Audio.h"
#include "DPad.h"
#include "HUD.h"
//#include "Input.h"

#define LEVEL_TIME 10000													// 30 Seconds until level is complete
#define MAX_LIVES 5															// The maximum number of lives a player can have

USING_NS_CC;

/*
typedef enum {
	KENDREASONWIN,
	KENDREASONLOSE
} EndReason;
*/

class Game : public Layer {
public:
	
	//DPad *controller;														// Add directional pad for mobile device
		
	// Game singleton
	static Game* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Game();
			return s_pInstance;
		}
		return s_pInstance;
	}
	
    //static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	
	void checkHighScore();													// When the game has ended, check if the current score is a high score and save it if it is

/*  
    void menuCloseCallback(cocos2d::Ref* pSender);							// a selector callback
        
    CREATE_FUNC(GameScene);													// implement the "static create()" method manually

	virtual void onAcceleration(Acceleration* acc, Event* event);
	float randomValueBetween(float low, float high);
	void setInvisible(Node * node);
	float getTimeTick();
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	void spawnLaser();														// 20180205
	void spawn2Lasers();													// 20180205

	void checkGameOver(float currenTime);									// 20180202 Check have game objects collided with each other
	void checkCollisions();													// 20180202 Check is the game over or not
	void moveShip(float dt);												// 20180202 Move the player ship
	void spawnAsteroids(float curTimeMillis);								// 20180202 Spawn asteroids
	void spawnEnemyShips(float curTimeMillis);								// 20180214 Spawn enemy ships
	void getInput();
	*/
	void updateTimer(float curTimeMillis);									// 20180204 Update the countdown timer, 21/02/2018 Passing curTimeMillis solves Android timer issue
	
	unsigned int getLevel() { return level; }								// 20180218 Return the current level
	unsigned int getScore() { return score; }								// 20180214 Return the current score
	unsigned int getLives() { return _lives; }								// 20180218 Return the players number of lives
	std::string getPlayerName() { return playerName; }						// 20180221 Return the name of the player
	unsigned int getTimer() { return time; }

	void setLevel(unsigned int set) { level = set; }
	void updateScore(unsigned int set) { score += set; }
	void setLives(unsigned int set) { _lives = set; }						// Set the number of livess
	void takeLife() { _lives--; };											// Decrement the number of lives
	void addLife() { if (_lives <= MAX_LIVES) _lives++; };					// Increment the number of lives (Max 5)
	void setPlayerName(std::string set) { playerName = set; }				// Set the player name
	void setTimer(unsigned int set) { time = set; }							// Set the countdown timer
	
private:
	unsigned int _lives = 0;												// Player lives

	std::string playerName;													// Current player name
	unsigned int score;														// Current score
	unsigned int level;														// Current level

	static Game* s_pInstance;												// Single instance of Game used as singleton, so only one instance exists thoughout the game

	unsigned int time;														// Countdown timer value
	float currentTime;														// 20180221 Change to float to fix Android timer issue
	/*
	Player* player;

	SpriteBatchNode *_batchNode;
	Sprite *playerLife;														// Indicate lives left
	Sprite *EnemyShip;
	ParallaxNodeExtras *_backgroundNode;
	Vector<Sprite*> *_asteroids;											// List of asteroids
	Vector<Sprite*> *EnemyShipList;											// List of enemy ships
	int _nextAsteroid = 0;
	int nextEnemyShip = 0;
	float _nextAsteroidSpawn = 0;											// time to spawn next asteroid
	float nextEnemyShipSpawnTime = 0;										// Time to spawn next enemy ship
	Vector<Sprite*> *_shipLasers;											// List of lasers
	int _nextShipLaser = 0;													// Ship laser list index
	Sprite* livesList[3];													// List of lives

	double _gameOverTime;
	bool _gameOver=false;
		
	Size winSize;															// Size of the game window

	void update(float dt);
	void endScene(EndReason endReason);
	void restartTapped(Ref* pSender);

	cocos2d::Label* scoreLabel;												// Display the current score
	cocos2d::Label * timeLabel;												// Display the time remaining

	*/
	
};

#endif // __GAME_SCENE_H__
