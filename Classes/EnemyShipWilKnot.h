/*
	EnemyShipWilKnot.h
	
	Joe O'Regan
	K00203642
	28/02/2018

	Second type of enemy ship, looks similar to a Klingon Bird of Prey

	This class inherits from EnemyShip, which inherits from Sprite

	The ship fires 2 lasers, but with a slightly slower fire rate
	It should be more difficult as there is more for the player to avoid
*/

#ifndef __ENEMY_SHIP_WK_H__
#define __ENEMY_SHIP_WK_H__

#include "EnemyShip.h"

class EnemyShipWilKnot : public EnemyShip {
public:

	static EnemyShipWilKnot* create(cocos2d::Size res);
		
	void update(float dt);

	cocos2d::Sprite* canon;
	static EnemyShipWilKnot* eship;

	float targetX;
	float targetY;

	cocos2d::Size screenSize;
};

#endif // __ENEMY_SHIP_WK_H__