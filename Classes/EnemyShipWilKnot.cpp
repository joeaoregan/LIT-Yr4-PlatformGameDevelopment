/*
	EnemyShipWilKnot.cpp
	
	Joe O'Regan
	K00203642
	28/02/2018

	Second type of enemy ship, looks similar to a Klingon Bird of Prey

	This class inherits from EnemyShip, which inherits from Sprite

	The ship fires 2 lasers, but with a slightly slower fire rate
	It should be more difficult as there is more for the player to avoid
*/

#include "EnemyShipWilKnot.h"
#include "Game.h"
#include "Level.h"

EnemyShipWilKnot* EnemyShipWilKnot::create(cocos2d::Size res) {
	EnemyShipWilKnot* eship = new EnemyShipWilKnot();
	float scale = (res.height == 720) ? 0.67f : 1.0f;

	if (eship && eship->initWithFile("ShipGrey.png")) {
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
			float(eship->getLives() / MAX_ENEMY_SHIP_LIVES),							// percentage
			redBR, transBR);
		eship->addChild(eship->bar);

		eship->canon = Sprite::create("DoubleCanon.png");
		//eship->canon->setPosition(cocos2d::Point(eship->getPosition().x, eship->getPosition().y));
		eship->canon->setPosition(cocos2d::Point(eship->getContentSize().width * 0.6f, eship->getContentSize().height / 2));
		//eship->addChild(eship->canon);

		
		/*

		// Put in update

		float angle = 45.0f;
		eship->targetX = res.width;
		eship->targetY = res.height;

		auto func = CallFunc::create(
			float x = eship->canon->getPosition().x;
			float y = eship->canon->getPosition().y;
			[&]() {
			Level::Instance()->spawnEnemyLaserAngled(Point(x, y), 
				cocos2d::Point(eship->canon->getPosition().x + eship->targetX, eship->canon->getPosition().y + eship->targetY), 45.0f);
		});


		auto sequence = Sequence::create(action, func, nullptr);
		*/
		float angle = 360.0f;
		auto action = RotateBy::create(5.0f, angle);
		eship->canon->runAction(action);		

		eship->addChild(eship->canon);
	}
	else {
		delete eship;
	}

	return eship;
}

void EnemyShipWilKnot::update(float curTimeMillis) {
	if (isVisible()) {
		if (curTimeMillis > m_nextFire) {
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx), 
				getPosition().y - (getContentSize().width * m_dy)));
		}
	}
	EnemyShip::update(curTimeMillis);											// Next fire time set here, not need to set twice, will cancel second laser
}
