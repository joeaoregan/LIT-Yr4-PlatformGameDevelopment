/*
	EnemyShipoDerpStar.h
	
	Joe O'Regan
	K00203642
	03/03/2018

	Enemy Boss
*/

#ifndef __ENEMY_SHIP_DERPSTAR_H__
#define __ENEMY_SHIP_DERPSTAR_H__

#include "EnemyShip.h"

class EnemyShipDerpStar : public EnemyShip {
public:

	static EnemyShipDerpStar* create(cocos2d::Size res);// Create Enemy Ship of type DerpStar
		
	virtual void update(float dt);

	virtual void init(cocos2d::Size res);

	virtual void spawnLaser();							// Spawn lasers
		
private:
	void initHealthBar(cocos2d::Size res);				// Initialise the healthbar
	void addCanons();									// Add canons to the enemy ship
	virtual void moveCanon();							// Apply actions to the canons (rotate them)
	
	cocos2d::Size screenSize;							// Screen resolution

	float m_speedMin = 15.0f;							// Set the min duration to travel across screen (Speed = faster)
	float m_speedMax = 20.0f;							// Set the max duration to travel across screen (Speed = slower)
};

typedef EnemyShipDerpStar DerpStar;

#endif // __ENEMY_SHIP_DERPSTAR_H__