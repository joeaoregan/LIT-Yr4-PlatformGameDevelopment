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

	void upgradeWeapon();											// Upgrade the players weapon, set currently used weapon (up to level 4)

	// Movement
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	unsigned int getWeaponStrength() { return m_weaponStrength; }	// Current player weapon grade or level
	void weaponUpgrade() { 
		if (m_weaponStrength < MAX_WEAPON_LEVEL)					// If the weapons level isn't maxed out already
			m_weaponStrength++; 									// Increment the weapon
	}	

private:
	cocos2d::Size visibleSize;										// Visible screen size
	float scale;													// Scale of the player based on the screen resolution
	unsigned int m_weaponStrength = 2;								// Current weapon

	cocos2d::Sprite* m_pCanon;										// Canon made visible 
};

#endif // __PLAYER_H__