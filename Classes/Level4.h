/*
	Level4.h

	Joe O'Regan
	K00203642
	18/02/2018
*/
#ifndef __LEVEL_4_H__
#define __LEVEL_4_H__

#include "Level.h"
#include "EnemyShipDerpStar.h"

class Level4 : public Level {
public:
	static cocos2d::Scene* createScene();				// there's no 'id' in SpaceQuest, so we recommend returning the class instance pointer
		
	virtual bool init();								// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

	virtual void spawnEnemyShips(float curTimeMillis);	// Spawn enemy ships

	void checkCollisions();								// 20180202 Check is the game over or not
	
	void spawnDerpLaser();								// Spawn lasers for the Enemy Boss (Separate to canon weapon)

	void spawnCanonLaser1();							// Spawn laser upwards at 30 degree angle
	void spawnCanonLaser2();							// Spawn laser in downwards direction

	void spawnShips();									// Spawn ships, separated from level base class

	virtual void checkGameOver(float currenTime);		// 20180202 Check have game objects collided with each other

	CREATE_FUNC(Level4);								// implement the "static create()" method manually

private:

	virtual void update(float dt);						// Update the level
	virtual void endScene(EndReason endReason);			// Do level complete stuff
	virtual void initEnemyShips();						// Add the enemies to appear in this level in the order required

	EnemyShip* m_pDerpStar;								// Boss enemy

	std::string m_messageEOL;
};

#endif // __LEVEL_4_H__