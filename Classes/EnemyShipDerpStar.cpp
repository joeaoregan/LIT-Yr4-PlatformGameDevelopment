/*
	EnemyShipDerpStar.cpp
	
	Joe O'Regan
	K00203642
	03/03/2018

	Enemy Boss
*/

#include "EnemyShipDerpStar.h"
#include "Game.h"
#include "Level.h"

EnemyShipDerpStar* EnemyShipDerpStar::create(cocos2d::Size res) {
	EnemyShipDerpStar* derp = new EnemyShipDerpStar();
	float scale = (res.height == 720) ? 0.67f : 1.0f;							// Scale for PC / Mobile depending on resolution

	if (derp && derp->initWithFile("DerpStar.png")) {
		derp->autorelease();

		derp->setVisible(false);
		derp->setScale(scale);													// Scale down the size for PC
		derp->screenSize = res;

		/* Different */
		// Set the lives
		derp->m_totalLives = MAX_ENEMY_SHIP4_LIVES;								// Set to 10 lives Defines.h
		derp->m_lives = derp->m_totalLives;

		derp->m_fireRate = 800;
		derp->m_dy = 0.375f * scale;
		derp->m_dx = -0.33f * scale;

		// Use to set spawn duration this gives slower time to travel across screen
		derp->m_speedMin = 20.0f;												// between 20 and 25 seconds = extremely slow
		derp->m_speedMax = 25.0f;

		cocos2d::Color4F redBR(0.52f, 0.52f, 0.52f, 1);
		cocos2d::Color4F transBR(1, 0, 0, 0.5f);

		derp->m_bar = HealthBar::create(
			derp->getPosition().x, derp->getPosition().y + derp->getContentSize().height,	// Position
			(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,					// Dimensions
			float(derp->getLives() / derp->m_totalLives),									// percentage (Max 5 lives)
			redBR, transBR);
		derp->addChild(derp->m_bar);

		// Add canon weapon
		derp->canon = Sprite::create("DoubleCanon.png");
		derp->canon->setPosition(cocos2d::Point(derp->getContentSize().width * 0.6f,
			derp->getContentSize().height / 2));

		derp->moveCanon();
	}
	else {
		delete derp;
	}

	return derp;
}


void EnemyShipDerpStar::moveCanon() {
	float angle = 360.0f;
	auto action = RotateBy::create(5.0f, angle);
	auto repeat = RepeatForever::create(action);
	canon->runAction(repeat);

	addChild(canon);
}

void EnemyShipDerpStar::update(float curTimeMillis) {
	if (isVisible() && getPosition().x < screenSize.width - getContentSize().width) {
		if (curTimeMillis > m_nextFire) {
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y + (getContentSize().height * m_dy)), GREEN2);
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y), GREEN1);
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y - (getContentSize().height * m_dy)), GREEN3);

			m_nextFire = curTimeMillis + m_fireRate;
		}
	}
}
