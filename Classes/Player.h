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
#include "HealthBar.h"

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

	void damage();
	void initHealthBar(cocos2d::Size res, cocos2d::Color4F fg);

	unsigned int getWeaponStrength() { return m_weaponStrength; }	// Current player weapon grade or level

	void weaponUpgrade() { 
		if (m_weaponStrength < MAX_WEAPON_LEVEL)					// If the weapons level isn't maxed out already
			m_weaponStrength++; 									// Increment the weapon
	}	

	void stopFlashing(float time) { m_flash = time; }				// Set the time to stop flashing the player --> make sure player is visible

private:
	HealthBar * m_pBar;												// HealthBar

	float m_flash = 0.0f;											// Time to flash player

	float m_maxHealth = 5.0f;										// The max health for healthbars etc
	int m_health = m_maxHealth;
	int m_lives = 3;

	cocos2d::Size m_visibleSize;									// Visible screen size
	float m_scale;													// Scale of the player based on the screen resolution
	unsigned int m_weaponStrength = 2;								// Current weapon

	cocos2d::Sprite* m_pCanon;										// Canon made visible 

	float m_speedMultiplier;										// Player too slow on android ...speed up
};

#endif // __PLAYER_H__