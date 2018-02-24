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
    static cocos2d::Scene* createScene();		// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();						// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

	void checkCollisions();						// 20180202 Check is the game over or not

    CREATE_FUNC(Level2);						// implement the "static create()" method manually

private:

	Sprite *playerLife;							// Indicate lives left
	Sprite *EnemyShip;
	int _nextAsteroid = 0;
	int nextEnemyShip = 0;
	float _nextAsteroidSpawn = 0;				// time to spawn next asteroid
	float nextEnemyShipSpawnTime = 0;			// Time to spawn next enemy ship
			
	virtual void update(float dt);
	virtual void endScene(EndReason endReason);		// CHANGE TO PROGRESS
};

#endif // __LEVEL_2_H__