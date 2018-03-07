/*
	EnemyShipDerpStar.cpp
	
	Joe O'Regan
	K00203642
	03/03/2018

	Enemy Boss

	Rotating canon weapons, only one is initialised to fire, when it rotates to -30, and 30 degrees
*/

#include "EnemyShipDerpStar.h"
#include "Game.h"
#include "Level.h"

EnemyShipDerpStar* EnemyShipDerpStar::create(cocos2d::Size res) {
	EnemyShipDerpStar* derp = new EnemyShipDerpStar();
	float scale = (res.height == 720) ? 0.67f : 1.0f;													// Scale for PC / Mobile depending on resolution

	//if (derp && derp->initWithFile("DerpStar.png")) {
	if (derp && derp->initWithSpriteFrameName("DerpStar.png")) {										// Create sprite from sprite sheet
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
		derp->m_winSize = res;
		
		/* Different */
		// Set the lives based on the game difficulty setting
		derp->m_totalLives = MAX_ENEMY_SHIP4_LIVES;														// Total lives set to 80 lives Defines.h
		derp->m_fireRate = 500;
		if (Game::Instance()->getDifficulty() == EASY) {
			derp->m_totalLives -= 20;																	// 60 total lives
			derp->m_fireRate += 100;																	// Decrease the fire rate
		}
		else if (Game::Instance()->getDifficulty() == HARD) {
			derp->m_totalLives += 20;																	// 100 lives
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

/*
	Spawn a laser
*/
void EnemyShipDerpStar::spawnLaser() {
	if (Game::Instance()->getTimeTick() > m_nextFire) {
		Level::Instance()->spawnEnemyLaser(cocos2d::Point(getPosition().x + 
			(getContentSize().width * m_dx),
			getPosition().y + (getContentSize().width * m_dy)));
		m_nextFire = Game::Instance()->getTimeTick() + m_fireRate;										// Set the next time to fire
	}
}

/*
	Initialise the Enemy Ship
*/
void EnemyShipDerpStar::init(cocos2d::Size res) {
	stopAllActions();																					// Stop any previous actions for the Node (Asteroids are reused)
	setPosition(res.width + getContentSize().width / 2, m_randYPos);									// Set the postion off screen to the right, at random Y value
	setVisible(true);																					// Set visible

	m_pBar->updateBar(m_lives / m_totalLives);															// Update the health bar
}

/*
	Initialise the healthbar
*/
void EnemyShipDerpStar::initHealthBar(cocos2d::Size res) {
	float scale = (res.height == 720) ? 0.67f : 1.0f;													// Scale for PC / Mobile depending on resolution

	cocos2d::Color4F fgColour(0.52f, 0.52f, 0.52f, 1);													// Foreground colour
	cocos2d::Color4F bgColour(1, 0, 0, 0.5f);															// Background colour

	m_pBar = HealthBar::create(
		getContentSize().width / 2, getPosition().y - (getContentSize().height * scale * 0.95f),		// Position
		getContentSize().width*0.67f, (res.height == 720) ? 20 : 30,									// Dimensions
		float(m_lives / m_totalLives),																	// percentage (Current lives / Max lives - 5)
		fgColour, bgColour, true);																		// Bar colours, setting and updating percentage text causes nullptr error (was because it was updating bars with no child label)
	m_pBar->setLabelTag("Derp Health: ");
	addChild(m_pBar);
}

/*
	Add canons to the enemy ship
*/
void EnemyShipDerpStar::addCanons() {
	m_pCanon1 = Sprite::create("DoubleCanon.png");														// Create with sprite
	m_pCanon1->setPosition(cocos2d::Point(getContentSize().width * 0.6f, getContentSize().height / 2));	// Position
	m_pCanon1->setRotation(-30.0f);																		// Canon1 starts at -30, then rotates 60 to +30 then -60 to -30 and loops, firing lasers in between

	m_pCanon2 = Sprite::create("DoubleCanon.png");
	m_pCanon2->setPosition(cocos2d::Point(getContentSize().width * 0.3f, getContentSize().height * 0.75f));

	m_pCanon3 = Sprite::create("DoubleCanon.png");
	m_pCanon3->setPosition(cocos2d::Point(getContentSize().width * 0.3f, getContentSize().height * 0.25f));
}

/*
	Apply actions to the canons (rotate them)
*/
void EnemyShipDerpStar::moveCanon() {
	float angle = 360.0f;
	cocos2d::RotateBy* action = cocos2d::RotateBy::create(5.0f, angle);
	cocos2d::RotateBy* action2 = cocos2d::RotateBy::create(5.0f, -angle);

	// Canon 1
	//cocos2d::RotateBy* action3 = cocos2d::RotateBy::create(5.0f, 60.0f);				// Moved to Level class to use call backs
	//cocos2d::RotateBy* action4 = cocos2d::RotateBy::create(5.0f, -60.0f);

	//cocos2d::Sequence* sequence = cocos2d::Sequence::create(action3, action4, nullptr);
	//cocos2d::RepeatForever* repeat1 = cocos2d::RepeatForever::create(sequence);

	cocos2d::RepeatForever* repeat2 = cocos2d::RepeatForever::create(action);
	cocos2d::RepeatForever* repeat3 = cocos2d::RepeatForever::create(action2);

	//canon1->runAction(repeat1);	// middle gun // Set in Level now as callbacks was one of the last thing I attempted
	m_pCanon2->runAction(repeat2);	// top
	m_pCanon3->runAction(repeat3);	// bottom

	addChild(m_pCanon1);
	addChild(m_pCanon2);
	addChild(m_pCanon3);
}

/*
	Update the Enemy Ship
*/
void EnemyShipDerpStar::update(float curTimeMillis) {
	if (isVisible() && getPosition().x < m_winSize.width - getContentSize().width) {

		//CCLOG("Derpstar Update");

		// Spawn 3 green lasers
		if (curTimeMillis > m_nextFire) {
			Level::Instance()->spawnEnemyLaser(cocos2d::Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y + (getContentSize().height * m_dy)), GREEN2);
			Level::Instance()->spawnEnemyLaser(cocos2d::Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y), GREEN1);
			Level::Instance()->spawnEnemyLaser(cocos2d::Point(getPosition().x + (getContentSize().width * m_dx),
				getPosition().y - (getContentSize().height * m_dy)), GREEN3);

			m_nextFire = curTimeMillis + m_fireRate;
		}
	}
}
