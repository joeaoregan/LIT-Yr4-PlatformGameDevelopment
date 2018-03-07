/*
	EnemyShipKling.cpp
	
	Joe O'Regan
	K00203642
	28/02/2018

	Second type of enemy ship, looks similar to a Klingon Bird of Prey

	This class inherits from EnemyShip, which inherits from Sprite

	The ship fires 2 lasers, but with a slightly slower fire rate
	It should be more difficult as there is more for the player to avoid
*/

#include "EnemyShipKling.h"
#include "Game.h"
#include "Level.h"

EnemyShipKling* EnemyShipKling::create(cocos2d::Size res) {
	EnemyShipKling* eship = new EnemyShipKling();
	float scale = (res.height == 720) ? 0.67f : 1.0f;
	eship->m_winSize = res;

	//if (eship && eship->initWithFile("ShipGreen.png")) {
	if (eship && eship->initWithSpriteFrameName("ShipGreen.png")) {				// Create sprite from sprite sheet
		eship->autorelease();

		eship->setVisible(false);
		eship->setScale(scale);													// Scale down the size for PC

		/* Different */
		// Set the lives
		eship->m_totalLives = MAX_ENEMY_SHIP2_LIVES;
		eship->m_lives = eship->m_totalLives;

		// Laser fire rate and spawn offsets
		eship->m_fireRate = 750;												// Time between firing lasers
		eship->m_dy = 0.375f * scale;											// Offset to spawn lasers
		eship->m_dx = -0.33f * scale;

		cocos2d::Color4F greenBar(0.39f, 0.65f, 0.7f, 1);						// Foregrond colour
		/*
		cocos2d::Color4F transBR(1, 0, 0, 0.5f);								// Background Colour

		eship->m_pBar = HealthBar::create(
			eship->getPosition().x + (eship->getContentSize().width / 2), 
			eship->getPosition().y + eship->getContentSize().height,			// Position
			(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,		// Dimensions
			float(eship->getLives() / eship->m_totalLives),						// percentage (Max 4 lives)
			greenBar, transBR);													// Set colours
		eship->addChild(eship->m_pBar);
		*/
		eship->initHealthBar(res, greenBar);									// Initialise the healthbar
	}
	else {
		delete eship;
	}

	return eship;
}

/*
	Update the ship spawning 2 x lasers based on the fire rate
*/
void EnemyShipKling::update(float curTimeMillis) {
	if (isVisible() && getPosition().x) {										// Don't shot until visibility set
		if (curTimeMillis > m_nextFire) {
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx), 
				getPosition().y - (getContentSize().width * m_dy)), ORANGE);

			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y + (getContentSize().width * m_dy)), ORANGE);

			m_nextFire = curTimeMillis + m_fireRate;
		}
	}
}
