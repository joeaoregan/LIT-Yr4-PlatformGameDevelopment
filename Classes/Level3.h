/*
	Level3.h

	Joe O'Regan
	K00203642
	18/02/2018
*/
#ifndef __LEVEL_3_H__
#define __LEVEL_3_H__

#include "cocos2d.h"
#include "Player.h"
#include "Game.h"

USING_NS_CC;

enum EndReason3 {
	KENDREASON3WIN,
	KENDREASON3LOSE
};

class Level3 : public Layer {
public:

    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    void menuCloseCallback(cocos2d::Ref* pSender);							// a selector callback
        
    CREATE_FUNC(Level3);													// implement the "static create()" method manually

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
	Player* player;															// The Player ship sprite

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

	double _gameOverTime;
	bool _gameOver=false;
		
	Size winSize;															// Size of the game window

	void update(float dt);													// Update the scene
	void endScene(EndReason3 endReason);
	void restartTapped(Ref* pSender);										// Restart the current level
	void startLevel4(Ref* pSender);											// Transition to level 4
	void returnToMenu(Ref* pSender);										// Return to the main menu
	
	cocos2d::Label* scoreLabel;												// Display the current score
	cocos2d::Label * timeLabel;												// Display the time remaining

	unsigned int time;														// Current time

	int currentTime;
};

#endif // __LEVEL_3_H__