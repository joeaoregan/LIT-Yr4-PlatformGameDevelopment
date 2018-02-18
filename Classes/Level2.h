/*
	Joe O'Regan
	GameScene.h
	02/02/2018
*/
#ifndef __LEVEL_2_H__
#define __LEVEL_2_H__

#include "cocos2d.h"
//#include "ParallaxNodeExtras.h"
#include "Player.h"
#include "Game.h"

USING_NS_CC;

enum EndReason2 {
	KENDREASON2WIN,
	KENDREASON2LOSE
};

class Level2 : public Layer {
public:
	// Game singleton
	static Level2* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Level2();
			return s_pInstance;
		}
		return s_pInstance;
	}

    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    void menuCloseCallback(cocos2d::Ref* pSender);							// a selector callback
        
    CREATE_FUNC(Level2);													// implement the "static create()" method manually

	float randomValueBetween(float low, float high);
	void setInvisible(Node * node);
	float getTimeTick();
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	void spawnLaser();														// 20180205
	void spawn2Lasers();													// 20180205

	void checkGameOver(float currenTime);									// 20180202 Check have game objects collided with each other
	void checkCollisions();													// 20180202 Check is the game over or not
	void spawnAsteroids(float curTimeMillis);								// 20180202 Spawn asteroids
	void spawnEnemyShips(float curTimeMillis);								// 20180214 Spawn enemy ships
	void updateTimer();														// 20180204 Update the countdown timer
	void getInput();

private:
	Player* player;

	SpriteBatchNode *_batchNode;
	Sprite *playerLife;														// Indicate lives left
	Sprite *EnemyShip;
	ParallaxNodeExtras *_backgroundNode;
	Vector<Sprite*>* _asteroids;											// List of asteroids
	Vector<Sprite*>* EnemyShipList;											// List of enemy ships
	int _nextAsteroid = 0;
	int nextEnemyShip = 0;
	float _nextAsteroidSpawn = 0;											// time to spawn next asteroid
	float nextEnemyShipSpawnTime = 0;										// Time to spawn next enemy ship
	Vector<Sprite*> *_shipLasers;											// List of lasers
	int _nextShipLaser = 0;													// Ship laser list index
	Sprite* livesList[MAX_LIVES];											// List of lives
	//int _lives=0;															// Player lives, Moved to Game.h

	double _gameOverTime;
	bool _gameOver=false;
		
	Size winSize;															// Size of the game window

	void update(float dt);
	void endScene(EndReason2 endReason);
	void restartTapped(Ref* pSender);

	cocos2d::Label* scoreLabel;												// Display the current score
	cocos2d::Label * timeLabel;												// Display the time remaining

	unsigned int time;														// Current time

	int currentTime;

	static Level2* s_pInstance;												// Single instance of GameScene used as singleton, so only one instance exists thoughout the game
};

#endif // __LEVEL_2_H__