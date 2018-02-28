#ifndef __ENEMY_SHIP_H__
#define __ENEMY_SHIP_H__

#include "cocos2d.h"
#include "HealthBar.h"

#define MAX_SHIP_LIVES 3.0f
#define ENEMY_FIRE_RATE 500

class EnemyShip : public cocos2d::Sprite {
public:

	static EnemyShip* create(cocos2d::Size res);
		
	void update(float dt);

	unsigned int getLives() { return lives; }
	void setLives(unsigned int set) { lives = set; }

	//void updateBar(float pc) {
		//bar->setPercent(1);
		//bar->update(0.0f);
	//	pt->setPercentage(pc);
	//}

	//HealthBar* createHealthBar(cocos2d::Size res);


	//void myUpdate();

	float getNextFire() { return nextFire; }
	void setNextFire(float set) { nextFire = set; }

private:

	cocos2d::Sprite* enemyShip;
	unsigned int lives = 3;

	HealthBar* bar;

	unsigned int fireRate;
	float nextFire;

	unsigned int m_fireRate = 500;

	cocos2d::DrawNode* myDrawNode;
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