/*
	Player.h

	Joe O'Regan
	K00203642

	Player class is a sub class of Sprite
*/
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Game.h"

class Player : public cocos2d::Sprite {
public:
	Player() {};													// Player constructor
	~Player() {}
	
	static Player* create(cocos2d::Size res);

	void update();													// Update player movement etc.

	void upgradeWeapon() {
		weaponUpgrade();
		canon->setVisible(true);
		Game::Instance()->setCurrentWeapon(m_weaponStrength);
	}

	// Movement
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	unsigned int getWeaponStrength() { return m_weaponStrength; }	// Current player weapon grade or level
	void weaponUpgrade() { m_weaponStrength++; }					// Increment the weapon

private:
	cocos2d::Size visibleSize;										// Visible screen size
	float scale;
	unsigned int m_weaponStrength = 2;

	cocos2d::Sprite* canon;
};

#endif // __PLAYER_H__