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
	//float scale = (res.height == 720) ? 0.67f : 1.0f;										// Set the scale
	float scale = (res.height == 1080) ? 1.0f :
		(res.height == 720) ? 0.67f : res.height / 1080;						// Kindle resolution is different

	//if (eship && eship->initWithFile("ShipGrey.png")) {
	if (eship && eship->initWithSpriteFrameName("ShipGrey.png")) {							// Create sprite from sprite sheet
		eship->autorelease();																// Clear when done

		eship->setVisible(false);															// Initially invisible until spawned
		eship->setScale(scale);																// Scale down the size for PC
		eship->m_winSize = res;																// Store the screen size

		/* Different */
		// Set the lives
		eship->m_totalLives = MAX_ENEMY_SHIP3_LIVES;										// Total lives
		eship->m_lives = eship->m_totalLives;												// Current lives

		eship->m_fireRate = 800;															// Fire rate

		if (Game::Instance()->getDifficulty() == EASY) eship->m_fireRate += 100;			// Slow down fire rate
		else if (Game::Instance()->getDifficulty() == EASY) eship->m_fireRate -= 100;		// Slow down fire rate

		eship->m_dy = 0.375f * scale;														// Offset the laser firing
		eship->m_dx = -0.33f * scale;

		// Use to set spawn duration this gives slower time to travel across screen
		eship->m_speedMin = 17.0f;															// Duration to travel across screen
		eship->m_speedMax = 20.0f;

		cocos2d::Color4F fgBar(0.52f, 0.52f, 0.52f, 1);										// Healthbar foreground colour
		/*
		cocos2d::Color4F transBR(1, 0, 0, 0.5f);											//  Background colour
		
		eship->m_pBar = HealthBar::create(
			eship->getPosition().x + (eship->getContentSize().width / 2), 
			eship->getPosition().y + eship->getContentSize().height,						// Position
			(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,					// Dimensions
			float(eship->getLives() / eship->m_totalLives),									// percentage (Max 5 lives)
			redBR, transBR);
		eship->addChild(eship->m_pBar);
		*/
		eship->initHealthBar(res, fgBar);

		// Create the ships canon
		eship->m_pCanon1 = Sprite::create("DoubleCanon.png");								// initWithSpriteFrameName() not working here (nullptr)
		eship->m_pCanon1->setPosition(cocos2d::Point(eship->getContentSize().width * 0.6f, 
			eship->getContentSize().height / 2));

		eship->moveCanon();																	// Move the canon
	}
	else {
		delete eship;
	}

	return eship;
}

/*
	Move the canon weapon (doesn't fire yet
*/
void EnemyShipWilKnot::moveCanon() {
	float angle = 360.0f;
	auto action = cocos2d::RotateBy::create(5.0f, angle);
	auto repeat = cocos2d::RepeatForever::create(action);
	m_pCanon1->runAction(repeat);

	addChild(m_pCanon1);
}

/*
	Update the enemy ship
*/
void EnemyShipWilKnot::update(float curTimeMillis) {
	if (isVisible() && getPosition().x < m_winSize.width - getContentSize().width) {
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
