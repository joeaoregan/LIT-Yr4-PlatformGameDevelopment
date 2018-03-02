#include "EnemyShip.h"
#include "Game.h"
#include "Level.h"

/*
void EnemyShip::myUpdate() {

	myDrawNode->clear();
	myDrawNode->setPosition(cocos2d::Vec2(myDrawNode->getPosition().x + 5, myDrawNode->getPosition().y));
	myDrawNode->drawCircle(myDrawNode->getPosition(), 100, 0.0f, 24, true, cocos2d::Color4F(1.0f, 1.0f, 1.0f, 1.0f));

}
*/
EnemyShip* EnemyShip::create(cocos2d::Size res) {
	EnemyShip* eship = new EnemyShip();
	float scale = (res.height == 720) ? 0.67f : 1.0f;

	if (eship && eship->initWithFile("EnemyShip.png")) {
	//if (eship && eship->initWithFile("ShipGrey.png")) {								// Test new ship sprite image
		eship->autorelease();
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
			res.width * 0.5f, res.height - ((res.height / TOTAL_LIST_ELEMENTS) * 4.5f),					// Position
			(res.height == 720) ? 200 : 300, (visibleSize.height == 720) ? 20 : 30,							// Dimensions
			(float)Game::Instance()->getEnemyShipKills() / (float)Game::Instance()->getEnemyShipCount(),			// percentage
			red, trans);																							// Colours
	*/

		eship->setVisible(false);
		eship->setScale(scale);									// Scale down the size for PC

		eship->m_dx = -0.65f * scale;

		cocos2d::Color4F redBR(1, 0, 0, 1);
		cocos2d::Color4F transBR(1, 0, 0, 0.5f);

		eship->bar = HealthBar::create(
			eship->getPosition().x + (eship->getContentSize().width / 2), eship->getPosition().y + eship->getContentSize().height,		// Position
			(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,						// Dimensions
			float(eship->getLives() / MAX_ENEMY_SHIP_LIVES),			// percentage
			//float(2 / MAX_SHIP_LIVES),			// percentage
			redBR, transBR);
		eship->addChild(eship->bar);
		
		//HealthBar* healthBar = HealthBar::create(
	}
	else {
		delete eship;
	}

	return eship;
}

/*
// I thought maybe returning a new healthbar when a ship is hit
HealthBar* createHealthBar(cocos2d::Size res) {
	cocos2d::Color4F redBR(1, 0, 0, 1);
	cocos2d::Color4F transBR(1, 0, 0, 0.5f);

	HealthBar * bar = HealthBar::create(
		getPosition().x + (getContentSize().width / 2), getPosition().y + getContentSize().height,		// Position
		(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,						// Dimensions
																							//float(eship->getLives() / MAX_SHIP_LIVES),			// percentage
		float(2 / MAX_SHIP_LIVES),			// percentage
		redBR, transBR);
	addChild(eship->bar);
	return 
}
*/
void EnemyShip::update(float curTimeMillis) {
	//CCLOG("Level %d: Update Enemy Ships", Game::Instance()->getLevel());

	// Fire laser	
	if (isVisible()) {
		//CCLOG("Enemy Ship Is Visible");

		if (curTimeMillis > m_nextFire) {
			//CCLOG("Enemy Call Fire Laser");

			Level::Instance()->spawnEnemyLaser(Point(getPosition().x + (getContentSize().width * m_dx), getPosition().y + (getContentSize().width * m_dy)));

			//CCLOG("Enemy Call Fire Laser");
			m_nextFire = curTimeMillis + m_fireRate;

			//setNextFire(curTimeMillis + ENEMY_FIRE_RATE);

			//CCLOG("Enemy Laser Fired");
		}
	}
	
	//bar->clear();
	//bar->redraw();
	//bar->draw();
	//bar->update(dt);


	//float test = MAX_SHIP_LIVES;
	//bar->updateBar(lives / test);
}

	/*
	Sprite *eShip = new EnemyShip();
	//eShip = cocos2d::Sprite::create("EnemyShip.png"); // Error Cannot be assigned to EnemyShip

	if (true) {
		eShip->autorelease();

		// Attempt to assign image to sprite
		//eShip = cocos2d::Sprite::create("EnemyShip.png");											// Asteroid sprite, JOR replaced auto specifier
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