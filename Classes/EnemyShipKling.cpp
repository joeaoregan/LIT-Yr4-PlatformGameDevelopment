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
	eship->winSize = res;

	if (eship && eship->initWithFile("ShipGreen.png")) {
		eship->autorelease();

		eship->setVisible(false);
		eship->setScale(scale);													// Scale down the size for PC

		/* Different */
		eship->m_fireRate = 750;
		eship->m_dy = 0.375f * scale;
		eship->m_dx = -0.33f * scale;

		cocos2d::Color4F redBR(1, 0, 0, 1);
		cocos2d::Color4F transBR(1, 0, 0, 0.5f);

		eship->bar = HealthBar::create(
			eship->getPosition().x + (eship->getContentSize().width / 2), 
			eship->getPosition().y + eship->getContentSize().height,			// Position
			(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,		// Dimensions
			float(eship->getLives() / MAX_ENEMY_SHIP_LIVES),					// percentage
			redBR, transBR);
		eship->addChild(eship->bar);
	}
	else {
		delete eship;
	}

	return eship;
}

void EnemyShipKling::update(float curTimeMillis) {
	if (isVisible() && getPosition().x) {						// Don't shot until visibility set
		if (curTimeMillis > m_nextFire) {
			Level::Instance()->spawnEnemyLaserOrange(Point(getPosition().x + (getContentSize().width * m_dx), 
				getPosition().y - (getContentSize().width * m_dy)));

			Level::Instance()->spawnEnemyLaserOrange(Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y + (getContentSize().width * m_dy)));

			m_nextFire = curTimeMillis + m_fireRate;
		}
	}
	//EnemyShip::update(curTimeMillis);											// Next fire time set here, not need to set twice, will cancel second laser
}
