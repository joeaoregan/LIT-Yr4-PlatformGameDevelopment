#ifndef __ENEMY_SHIP_H__
#define __ENEMY_SHIP_H__

#include "cocos2d.h"

class EnemyShip : public cocos2d::Node {
public:
	//EnemyShip(cocos2d::Layer *layer);				// Player constructor

	//void update();									// Update player movement etc.

	//cocos2d::Sprite* getSprite() { return enemyShip; }

	unsigned int getLives() { return lives; }
	void setLives(unsigned int set) { lives = set; }

private:
	cocos2d::Size visibleSize;						// Visible screen size
	cocos2d::Vec2 origin;							// Origin of the screen

	cocos2d::Sprite *enemyShip;						// Player sprite

	unsigned int lives = 3;
};

#endif // __ENEMY_SHIP_H__