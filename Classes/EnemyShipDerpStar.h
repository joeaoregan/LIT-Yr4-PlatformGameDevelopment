/*
	EnemyShipoDerpStar.h
	
	Joe O'Regan
	K00203642
	03/03/2018

	Enemy Boss
*/

#ifndef __ENEMY_SHIP_WK_H__
#define __ENEMY_SHIP_WK_H__

#include "EnemyShip.h"

class EnemyShipDerpStar : public EnemyShip {
public:

	static EnemyShipDerpStar* create(cocos2d::Size res);
		
	virtual void update(float dt);

	virtual void moveCanon();

	cocos2d::Sprite* canon;
	static EnemyShipDerpStar* eship;

	float targetX;
	float targetY;

	cocos2d::Size screenSize;
};

#endif // __ENEMY_SHIP_WK_H__