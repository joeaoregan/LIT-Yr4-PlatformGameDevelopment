#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player {
public:
	Player(cocos2d::Layer *layer);			// Player constructor

	void update();							// Update player movement etc.

	cocos2d::Sprite* getSprite() { return player; }
/*
	// Movement
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
*/
private:
	cocos2d::Size visibleSize;				// Visible screen size
	cocos2d::Vec2 origin;					// Origin of the screen

	cocos2d::Sprite *player;				// Player sprite
};

#endif // __BIRD_H__