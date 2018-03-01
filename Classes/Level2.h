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

class Level2 : public Level {
public:
    static cocos2d::Scene* createScene();		// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();						// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

	virtual void endScene(EndReason endReason);	// Do level complete stuff
	
	CREATE_FUNC(Level2);						// implement the "static create()" method manually

	void checkCollisions();						// 20180202 Check is the game over or not

private:

	//cocos2d::Sprite *playerLife;				// Indicate lives left
	//cocos2d::Sprite *EnemyShip;
	void initEnemyShips();
			
	virtual void update(float dt);
};

#endif // __LEVEL_2_H__