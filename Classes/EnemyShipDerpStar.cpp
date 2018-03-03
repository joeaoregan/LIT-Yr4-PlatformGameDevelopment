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
//#include "Level4.h"

EnemyShipDerpStar* EnemyShipDerpStar::create(cocos2d::Size res) {
	EnemyShipDerpStar* derp = new EnemyShipDerpStar();
	float scale = (res.height == 720) ? 0.67f : 1.0f;													// Scale for PC / Mobile depending on resolution

	if (derp && derp->initWithFile("DerpStar.png")) {
		derp->autorelease();

		// Use to set spawn duration this gives slower time to travel across screen
		derp->m_speedMin = 20.0f;																		// between 20 and 25 seconds = extremely slow
		derp->m_speedMax = 25.0f;
		derp->m_duration = Level::Instance()->randomValueBetween(derp->m_speedMin, derp->m_speedMax);	// Set a random speed
		derp->m_randYPos = Level::Instance()->randomValueBetween(0.0f, res.height);						// Random Y position for asteroid

		derp->setVisible(true);
		///derp->setPosition(res.width + derp->getContentSize().width / 2, 0);							// Set the postion off screen to the right, at random Y value
		derp->setPosition(res.width + derp->getContentSize().width, res.height/2);						// Set the postion off screen to the right, at random Y value
		derp->setScale(scale);																			// Scale down the size for PC
		derp->screenSize = res;
		

		/* Different */
		// Set the lives based on the game difficulty setting
		derp->m_totalLives = MAX_ENEMY_SHIP4_LIVES;														// Total lives set to 40 lives Defines.h
		derp->m_fireRate = 500;
		if (Game::Instance()->getDifficulty() == EASY) {
			derp->m_totalLives -= 10;																	// 30 total lives
			derp->m_fireRate += 100;																	// Decrease the fire rate
		}
		else if (Game::Instance()->getDifficulty() == HARD) {
			derp->m_totalLives += 10;																	// 50 lives
			derp->m_fireRate -= 100;																	// Increase the fire rate
		}

		derp->m_lives = derp->m_totalLives;																// Set the number of lives

		derp->m_dy = 0.375f * scale;																	// Laser spawn offset
		derp->m_dx = -0.33f * scale;
						
		derp->initHealthBar(res);																		// Initialise the health bar

		// Add canon weapons
		derp->addCanons();
		derp->moveCanon();
	}
	else {
		delete derp;
	}

	return derp;
}

void EnemyShipDerpStar::spawnLaser() {
	if (Game::Instance()->getTimeTick() > m_nextFire) {
		Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
			getPosition().y + (getContentSize().width * m_dy)));
		m_nextFire = Game::Instance()->getTimeTick() + m_fireRate;
	}
}

void EnemyShipDerpStar::init(cocos2d::Size res) {
	stopAllActions();																	// Stop any previous actions for the Node (Asteroids are reused)
	setPosition(res.width + getContentSize().width / 2, m_randYPos);					// Set the postion off screen to the right, at random Y value
	setVisible(true);																	// Set visible

	m_bar->updateBar(m_lives / m_totalLives);											// Update the health bar
}

/*
	Initialise the healthbar
*/
void EnemyShipDerpStar::initHealthBar(cocos2d::Size res) {
	float scale = (res.height == 720) ? 0.67f : 1.0f;									// Scale for PC / Mobile depending on resolution

	cocos2d::Color4F fgColour(0.52f, 0.52f, 0.52f, 1);
	cocos2d::Color4F bgColour(1, 0, 0, 0.5f);

	m_bar = HealthBar::create(
		getContentSize().width / 2, getPosition().y - (getContentSize().height * scale * 0.95f),	// Position
		getContentSize().width*0.67f, (res.height == 720) ? 20 : 30,					// Dimensions
		float(m_lives / m_totalLives),													// percentage (Current lives / Max lives - 5)
		fgColour, bgColour, true);														// Bar colours, setting and updating percentage text causes nullptr error (was because it was updating bars with no child label)
	m_bar->setLabelTag("Derp Health: ");
	addChild(m_bar);
}

/*
	Add canons to the enemy ship
*/
void EnemyShipDerpStar::addCanons() {
	canon1 = Sprite::create("DoubleCanon.png");
	canon1->setPosition(cocos2d::Point(getContentSize().width * 0.6f,
		getContentSize().height / 2));
	canon1->setRotation(-30.0f);

	canon2 = Sprite::create("DoubleCanon.png");
	canon2->setPosition(cocos2d::Point(getContentSize().width * 0.3f,
		getContentSize().height * 0.75f));

	canon3 = Sprite::create("DoubleCanon.png");
	canon3->setPosition(cocos2d::Point(getContentSize().width * 0.3f,
		getContentSize().height * 0.25f));
}

/*
	Apply actions to the canons (rotate them)
*/
void EnemyShipDerpStar::moveCanon() {
	float angle = 360.0f;
	auto action = RotateBy::create(5.0f, angle);
	auto action2 = RotateBy::create(5.0f, -angle);

	// Canon 1
	auto action3 = RotateBy::create(5.0f, 60.0f);
	auto action4 = RotateBy::create(5.0f, -60.0f);

	//auto repeat3 = RepeatForever::create(action3);
	//auto repeat4 = RepeatForever::create(action4);
	auto sequence = Sequence::create(action3, action4, nullptr);
	auto repeat1 = RepeatForever::create(sequence);

	auto repeat2 = RepeatForever::create(action);
	auto repeat3 = RepeatForever::create(action2);

	//canon1->runAction(repeat1);	// middle gun
	canon2->runAction(repeat2);	// top
	canon3->runAction(repeat3);	// bottom

	addChild(canon1);
	addChild(canon2);
	addChild(canon3);


	//auto func = CallFunc::create(CC_CALLBACK_0(Level4::spawnDerpLaser, this));
}

void EnemyShipDerpStar::update(float curTimeMillis) {
	if (isVisible() && getPosition().x < screenSize.width - getContentSize().width) {

		CCLOG("Derpstar Update");

		if (curTimeMillis > m_nextFire) {
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y + (getContentSize().height * m_dy)), GREEN2);
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y), GREEN1);
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y - (getContentSize().height * m_dy)), GREEN3);

			m_nextFire = curTimeMillis + m_fireRate;
		}

		//if (canon1->getRotation() == 30)
		//	Level::Instance()->spawnEnemyLaser(Point(canon1->getPosition().x,
		//		canon1->getPosition().y), GREEN2);
	}
	//Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx),
	//	getPosition().y + (getContentSize().height * m_dy)), GREEN1);
}
