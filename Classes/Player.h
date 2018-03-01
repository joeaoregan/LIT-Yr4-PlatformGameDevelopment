#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite {
public:
	Player() {};								// Player constructor
	~Player() {}

	//Player(cocos2d::Layer *layer);			// Player constructor

	static Player* create(cocos2d::Size res);

	void update();								// Update player movement etc.

	//cocos2d::Sprite* getSprite() { return player; }

	// Movement
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

private:
	cocos2d::Size visibleSize;					// Visible screen size

	//cocos2d::Sprite *player;					// Player sprite
};

#endif // __PLAYER_H__