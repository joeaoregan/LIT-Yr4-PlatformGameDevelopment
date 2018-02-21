/*
	Level2.h

	Joe O'Regan
	K00203642
	02/02/2018

	Level2 Level sub class
*/
#ifndef __LEVEL_2_H__
#define __LEVEL_2_H__

#include "Level.h"

USING_NS_CC;

class Level2 : public Level {
public:
    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
                
    CREATE_FUNC(Level2);													// implement the "static create()" method manually

	//void setInvisible(Node * node);
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	void spawnLasers(int amount);											// 20180221

	void checkGameOver(float currenTime);									// 20180202 Check have game objects collided with each other
	void checkCollisions();													// 20180202 Check is the game over or not
	void spawnAsteroids(float curTimeMillis);								// 20180202 Spawn asteroids
	void spawnEnemyShips(float curTimeMillis);								// 20180214 Spawn enemy ships

private:

	Sprite *playerLife;														// Indicate lives left
	Sprite *EnemyShip;
	int _nextAsteroid = 0;
	int nextEnemyShip = 0;
	float _nextAsteroidSpawn = 0;											// time to spawn next asteroid
	float nextEnemyShipSpawnTime = 0;										// Time to spawn next enemy ship
			
	void update(float dt);
	void endScene(EndReason endReason);
	void restartTapped(Ref* pSender);										// Restart the current level	
};

#endif // __LEVEL_2_H__