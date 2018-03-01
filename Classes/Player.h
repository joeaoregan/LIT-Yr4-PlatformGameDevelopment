/*
	Player.h

	Joe O'Regan
	K00203642

	Player class is a sub class of Sprite
*/
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite {
public:
	Player() {};								// Player constructor
	~Player() {}
	
	static Player* create(cocos2d::Size res);

	void update();								// Update player movement etc.

	// Movement
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

private:
	cocos2d::Size visibleSize;					// Visible screen size
	float scale;
};

#endif // __PLAYER_H__