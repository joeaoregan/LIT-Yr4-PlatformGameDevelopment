/*
	EnemyShip.h

	Joe O'Regan
	K00203642

	Enemy Ship base class is a sub class of Sprite
	I tried implementing a health bar using a DrawNode object
*/
#include "EnemyShip.h"
#include "Game.h"
#include "Level.h"

/*
	Create an enemy ship with child health bar DrawNode object (not working)
*/
EnemyShip* EnemyShip::create(cocos2d::Size res) {
	EnemyShip* eship = new EnemyShip();
	float scale = (res.height == 720) ? 0.67f : 1.0f;

	//if (eship && eship->initWithFile("EnemyShip.png")) {
	if (eship && eship->initWithSpriteFrameName("EnemyShip.png")) {				// Create sprite from sprite sheet
		eship->m_winSize = res;													// Store the screen resolution
		eship->autorelease();													// Auto release it when done
		eship->setVisible(false);												// Initially invisible
		eship->setScale(scale);													// Scale down the size for PC

		eship->m_dx = -0.65f * scale;											// Offset for spawning the laser at the front of the ship
		eship->initHealthBar(res, cocos2d::Color4F(1, 0, 0, 1));				// Create the healthbar
	}
	else {
		delete eship;
	}

	return eship;
}

/*
	Initialise the healthbar (different for each enemy ship)
*/
void EnemyShip::initHealthBar(cocos2d::Size res, cocos2d::Color4F fg) {
	//cocos2d::Color4F fgColour(1, 0, 0, 1);									// Foreground colour (Red)
	cocos2d::Color4F bgColour(1, 0, 0, 0.5f);									// Background colour (Transparent red)

	m_pBar = HealthBar::create(
		getPosition().x + (getContentSize().width / 2),
		getPosition().y + getContentSize().height,								// Position
		(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,			// Dimensions
		float(getLives() / m_totalLives),										// percentage  (Max 4 lives)
		fg, bgColour);															// Set the bar colours
	addChild(m_pBar);
}

/*
	Take a life, and check if still alive, setting visibility for ship and bar
*/
void EnemyShip::takeLife() {
	m_lives--;

	if (m_lives < 1) {
		if (isVisible()) Game::Instance()->incrementEnemyShipKills();			// increment the enemies killed if visible (stop repeating)
		setVisible(false);														// The set visible
	}

	if (isVisible()) m_pBar->updateBar(m_lives / m_totalLives);					// The health bar percentage is lives / max lives
}

/*
	Initialise the asteroid
	Stop all actions, set the position, set visible, and scale accoring to speed
*/
void EnemyShip::init(cocos2d::Size res) {
	m_duration = Level::Instance()->randomValueBetween(m_speedMin, m_speedMax);	// Set a random speed
	m_randYPos = Level::Instance()->randomValueBetween(0.0f, res.height);		// Random Y position for asteroid

	stopAllActions();															// Stop any previous actions for the Node (Asteroids are reused)
	setPosition(res.width + getContentSize().width / 2, m_randYPos);			// Set the postion off screen to the right, at random Y value
	setVisible(true);															// Set visible
	setLives(m_totalLives);														// Reset the number of lives available
	m_pBar->updateBar(m_lives / m_totalLives);									// Update the health bar
}

/*
	Update: Fire laser, spacing the spawntime with a fire rate
*/
void EnemyShip::update(float curTimeMillis) {
	if (isVisible()) {
		if (curTimeMillis > m_nextFire) {
			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + 
				(getContentSize().width * m_dx), 
				getPosition().y + (getContentSize().width * m_dy)));

			m_nextFire = curTimeMillis + m_fireRate;							// Time to spawn next laser
		}
	}
}


/*
void EnemyShip::moveTo() {
// Move the ship to the players coordinate
auto action = MoveTo::create(3, Point(player->getPositionX(), 
player->getPositionY()));
enemyShip->runAction(action);
}
*/
//bar->clear();
//bar->redraw();
//bar->draw();
//bar->update(dt);


//float test = MAX_SHIP_LIVES;
//bar->updateBar(lives / test);
/*
// I thought maybe returning a new healthbar when a ship is hit
HealthBar* createHealthBar(cocos2d::Size res) {
	cocos2d::Color4F redBR(1, 0, 0, 1);
	cocos2d::Color4F transBR(1, 0, 0, 0.5f);

	HealthBar * bar = HealthBar::create(
		getPosition().x + (getContentSize().width / 2), getPosition().y + 
		getContentSize().height,		// Position
		(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,			// Dimensions
																				//float(eship->getLives() / MAX_SHIP_LIVES),			// percentage
		float(2 / MAX_SHIP_LIVES),			// percentage
		redBR, transBR);
	addChild(eship->bar);
	return 
}
*/

	//if (eship && eship->initWithFile("ShipGrey.png")) {						// Test new ship sprite image
		/*
		// Don't know how to update????????????
		ProgressTimer* pt = ProgressTimer::create(Sprite::create("btnMinus.png"));
		if (pt != NULL)
		{
			pt->setType(cocos2d::ProgressTimer::Type::BAR);
			pt->setMidpoint(ccp(0.5f, 0));
			pt->setBarChangeRate(ccp(1, 0));
			pt->setPercentage((eship->lives * 100) / 3);
			pt->setPosition(ccp(100, 100));
			eship->addChild(pt);
			pt->setName("pt");
		}
		*/
	/*
		cocos2d::DrawNode* healthBar = createStatusBar(
		
		HealthBar* healthBar = HealthBar::create(
			res.width * 0.5f, res.height - ((res.height / TOTAL_LIST_ELEMENTS) * 4.5f),	// Position
			(res.height == 720) ? 200 : 300, (visibleSize.height == 720) ? 20 : 30,		// Dimensions
			(float)Game::Instance()->getEnemyShipKills() / 
			(float)Game::Instance()->getEnemyShipCount(),								// percentage
			red, trans);																// Colours
	*/
	/*
	Sprite *eShip = new EnemyShip();
	//eShip = cocos2d::Sprite::create("EnemyShip.png"); // Error Cannot be assigned to EnemyShip

	if (true) {
		eShip->autorelease();

		// Attempt to assign image to sprite
		//eShip = cocos2d::Sprite::create("EnemyShip.png");								// Asteroid sprite, JOR replaced auto specifier
		//eShip->initWithFile("EnemyShip.png");
		j*/
			/*
		if (eShip && eShip->initWithFile("EnemyShip.png"))
		{
			eShip->autorelease();
			return (EnemyShip*)eShip;
		}
		*/
		//eShip->setPosition(cocos2d::Point(eShip->getPosition().x, eShip->getPosition().y));
		//eShip->addChild(eShip->enemyShip);
		/*
		cocos2d::Color4F red(1, 0, 0, 1);
		cocos2d::Color4F trans(1, 0, 0, 0.5f);
		
		cocos2d::DrawNode* bar = createStatusBar(
			eShip->getPosition().x + eShip->getContentSize().height, eShip->getPosition().y,	// Position
			(res.height == 720) ? 100 : 150, (res.height == 720) ? 10 : 15,						// Dimensions
			float(eShip->lives / MAX_SHIP_LIVES),												// percentage
			red, trans);

		eShip->addChild(bar);
		*/
	/*
		return (EnemyShip*) eShip;
	}

	CC_SAFE_DELETE(eShip);
	return NULL;
	*/

/*
Sprite* Sprite::create(const std::string& filename)
{
	Sprite *sprite = new (std::nothrow) Sprite();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
*/

//#define PLAYER_SPEED 3.0f
//#define PLAYER_COLLISION_BITMASK 0x000001
/*
EnemyShip::EnemyShip(cocos2d::Layer *layer) {
	visibleSize = Director::getInstance()->getVisibleSize();						// screen size
	origin = Director::getInstance()->getVisibleOrigin();							// origin coorinate

	enemyShip = Sprite::createWithSpriteFrameName("EnemyShip.png");
	enemyShip->setPosition(visibleSize.width * 0.1, visibleSize.height * 0.5);		// Place in middle left of screen
	enemyShip->setScale((visibleSize.height == 720) ? 1.0f : 1.5f);					// Increase scale of player for Android (My phone anyway)
}

void EnemyShip::update() {
	// Move the Enemy Ship
	}
*/