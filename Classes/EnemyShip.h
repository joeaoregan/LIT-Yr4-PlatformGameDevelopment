#ifndef __ENEMY_SHIP_H__
#define __ENEMY_SHIP_H__

#include "cocos2d.h"
#include "HealthBar.h"

#define MAX_SHIP_LIVES 3.0f

class EnemyShip : public cocos2d::Sprite {
public:

	static EnemyShip* create(cocos2d::Size res);
		
	void update();

	unsigned int getLives() { return lives; }
	void setLives(unsigned int set) { lives = set; }

	//void updateBar(float pc) {
		//bar->setPercent(1);
		//bar->update(0.0f);
	//	pt->setPercentage(pc);
	//}

	//HealthBar* createHealthBar(cocos2d::Size res);

private:

	cocos2d::Sprite* enemyShip;
	unsigned int lives = 3;

	HealthBar* bar;
};


/*
//class EnemyShip : public cocos2d::Node {
class EnemyShip {
public:
	//EnemyShip(cocos2d::Layer *layer);				// Player constructor

	//void update();									// Update player movement etc.

	cocos2d::Sprite* getSprite() { return enemyShip; }
	void setSprite(cocos2d::Sprite* set) { enemyShip = set; }

	unsigned int getLives() { return lives; }
	void setLives(unsigned int set) { lives = set; }

private:
	cocos2d::Size visibleSize;						// Visible screen size
	cocos2d::Vec2 origin;							// Origin of the screen

	cocos2d::Sprite *enemyShip;						// Player sprite

	unsigned int lives = 3;
	//unsigned int ammo = 10;
};
*/
#endif // __ENEMY_SHIP_H__