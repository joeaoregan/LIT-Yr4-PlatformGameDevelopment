/*
	Level4.h

	Joe O'Regan
	K00203642
	18/02/2018
*/
#ifndef __LEVEL_4_H__
#define __LEVEL_4_H__

#include "Level.h"

class Level4 : public Level {
public:
	static cocos2d::Scene* createScene();		// there's no 'id' in cpp, so we recommend returning the class instance pointer

	virtual bool init();						// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

	void checkCollisions();						// 20180202 Check is the game over or not

	CREATE_FUNC(Level4);						// implement the "static create()" method manually

private:

	virtual void update(float dt);				// Update the level
	virtual void endScene(EndReason endReason);	// Do level complete stuff
};

#endif // __LEVEL_4_H__