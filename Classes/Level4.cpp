/*
	Level4.cpp

	Joe O'Regan
	K00203642
	18/02/2018

	Added additional level
*/
#include "Level.h"
#include "Level4.h"
#include "Input.h"
#include "EnemyShipKling.h"
#include "EnemyShipWilKnot.h"
//#include "EnemyShipDerpStar.h"

Scene* Level4::createScene() {
	cocos2d::Scene* scene = Scene::create();							// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level4::create();								// 'layer' is an autorelease object, JOR replaced auto specifier  
	//layer->setName("Level4");											// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
	scene->addChild(s_pLayerInstance);									// Add layer as a child to scene	    
	return scene;														// Return the scene
}

/*
Initialisation specific to Level 4
*/
bool Level4::init() {
	Level::init();														// 20180221 Added Level base class

	Game::Instance()->setLevel(4);										// Specific to level 2
	newHUD->setLevelLabel();											// Update HUD Level text display

	if (!Layer::init()) { return false; }								// super init first
	
	// ParallaxNode
	this->addChild(m_backgroundNode, -1);								// Add the parallax background
	m_backgroundNode->init();											// Initialise the parallax scrolling background
	
	this->scheduleUpdate();												// Start updating the scene



	derpStar = DerpStar::create(visibleSize);							// Create boss enemy
	this->addChild(derpStar);											// Add to scene
	

	cocos2d::MoveTo* derpForward = MoveTo::create(4.0f,
		Point(visibleSize.width * 0.75f, player->getPositionY()));		// Part of time spent moving to player, the rest moving off scre

	cocos2d::MoveTo* derpBack = MoveTo::create(2.0f, 
		Point(visibleSize.width + derpStar->getContentSize().width, player->getPositionY()));


	//derpStar->runAction(actionDerp);

	//auto func = CallFuncN::create([](Ref* sender) {
	//	//spawnDerpLaser();
	//	CCLOG("test");
	//	//derpStar->init(0);
	//	cocos2d::Sprite* sprite = dyanamic_cast<cocos2d::Sprite*>(sender);
	//});

	auto func = CallFunc::create(CC_CALLBACK_0(Level4::spawnDerpLaser, this));
	auto func1 = CallFunc::create(CC_CALLBACK_0(Level4::spawnShips, this));

	auto sequence = Sequence::create(derpForward,
		MoveBy::create(2.5f,
			//Point(winSize.width - derpStar->getContentSize().width, 0)),						// move off the screen its full width
			Point(winSize.width - derpStar->getContentSize().width, player->getPositionY() - (derpStar->getContentSize().height / 2))),
		//CallFuncN::create(CC_CALLBACK_1(Level4::spawnDerpLaser(), this)), NULL)						// Then set invisible if it reaches the target - TERMINATE WITH NULL
		//CallFuncN::create(spawnDerpLaser(), NULL)						// Then set invisible if it reaches the target - TERMINATE WITH NULL
		func, derpBack, func, func1, NULL);

	auto repeat = RepeatForever::create(sequence);

	//derpStar->setPosition(Point(visibleSize.width*0.75f, visibleSize.height / 2));
	derpStar->runAction(repeat);

	auto action3 = RotateBy::create(2.0f, 60.0f);									// Starting at -30, rotate to 60
	auto action4 = RotateBy::create(2.0f, -60.0f);									// Then back again
	auto fire1 = CallFunc::create(CC_CALLBACK_0(Level4::spawnCanonLaser1, this));
	auto fire2 = CallFunc::create(CC_CALLBACK_0(Level4::spawnCanonLaser2, this));

	auto canonSequence = Sequence::create(action3, fire1, action4, fire2, NULL);
	auto canonRepeat = RepeatForever::create(canonSequence);

	derpStar->canon1->runAction(canonRepeat);
	//derpStar->canon1->setVisible(false);


	// Initialise lasers
	m_enemyLaserList3 = new Vector<Sprite*>(NUM_LASERS);				// List of lasers
	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* enemyLaser = Sprite::create(LASER_GREEN_IMG);	// Laser sprite, JOR replaced auto specifier
		enemyLaser->setVisible(false);
		this->addChild(enemyLaser);
		m_enemyLaserList3->pushBack(enemyLaser);
	}

	return true;
}

// Up
void Level4::spawnCanonLaser1() {

	CCLOG("***************************************************************Canon Fire function called");
	float laserY = visibleSize.width * 0.577; // tan
	//if (derpStar->canon1->getRotation() > 30)
	//	laserY *= -1; // invert y direction
	//float distance = sqrt((visibleSize.width * visibleSize.width) + (laserY * laserY));
	//CCLOG("spawnCanonLaser1 distance %f", distance);

	cocos2d::Sprite* enemyLaser;
	enemyLaser = m_enemyLaserList3->at(m_nextEnemyLaser);	// next blue laser
	m_nextEnemyLaser++;
	if (m_nextEnemyLaser >= m_enemyLaserList3->size()) m_nextEnemyLaser = 0;
	if (enemyLaser->isVisible()) return;													// If the laser is already visible skip it
	Audio::Instance()->laserFXEnemy();														// Play audio

	//enemyLaser->setPosition(derpStar->getPosition().x + derpStar->getContentSize().width * 0.6f, (derpStar->getPosition().y/2) + derpStar->getContentSize().height / 2);													// Set the postion relevant to the ships coordinates

	enemyLaser->setPosition(derpStar->getPosition().x, derpStar->getPosition().y * 1.05f);
	//enemyLaser->setPosition(derpStar->getPosition().x - (derpStar->getContentSize().width / 2) + derpStar->canon1->getPosition().x, derpStar->getPosition().y * 1.05f);
	enemyLaser->setRotation(30.0f);

	enemyLaser->setVisible(true);															// Set visible on screen
	enemyLaser->stopAllActions();															// Stop actions for the laser

	float x = derpStar->canon1->getPosition().x - visibleSize.width;
	float y = derpStar->canon1->getPosition().y + laserY;

	CCLOG("Laser end coords X: %f, Y: %f", x, y);

	enemyLaser->runAction(
		Sequence::create(MoveBy::create(0.5f, Point(x, y)),								// change to plus 100 for up - 100 for down
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
}

// Down
void Level4::spawnCanonLaser2() {
	float laserY = visibleSize.width * 0.577 * -1; // tan

	cocos2d::Sprite* enemyLaser;
	enemyLaser = m_enemyLaserList3->at(m_nextEnemyLaser);	// next blue laser
	m_nextEnemyLaser++;
	if (m_nextEnemyLaser >= m_enemyLaserList3->size()) m_nextEnemyLaser = 0;
	if (enemyLaser->isVisible()) return;													// If the laser is already visible skip it
	Audio::Instance()->laserFXEnemy();														// Play audio

	//enemyLaser->setPosition(derpStar->getPosition().x + getContentSize().width * 0.6f, derpStar->getPosition().y + getContentSize().height / 2);
	enemyLaser->setPosition(derpStar->getPosition().x, derpStar->getPosition().y * 0.95f);
	enemyLaser->setRotation(-30.0f);														// Set the postion relevant to the ships coordinates
	enemyLaser->setVisible(true);															// Set visible on screen
	enemyLaser->stopAllActions();															// Stop actions for the laser

	float x = derpStar->canon1->getPosition().x - visibleSize.width;
	float y = derpStar->canon1->getPosition().y + laserY;
	
	enemyLaser->runAction(
		Sequence::create(MoveBy::create(0.5f, Point(x, y)),									// change to plus 100 for up - 100 for down
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
}


void Level4::spawnDerpLaser() {
	CCLOG("Spawn lasers");		// Set the node invisible

	derpStar->spawnLaser();
}

void Level4::spawnShips() {
	CCLOG("spawn ships");		// Set the node invisible

	// Spawn 2 enemy ships
	for (int i = 0; i < 2; i++) {
		if (m_enemyLaserList1->size() > 0) {
			EnemyShip *enemyShip = m_enemyShipList->at(nextEnemyShip);
			nextEnemyShip++;
			if (nextEnemyShip >= (unsigned int)m_enemyShipList->size()) nextEnemyShip = 0;
			Game::Instance()->incrementEnemyShipCount();
			enemyShip->init(visibleSize);

			cocos2d::MoveTo* action = MoveTo::create(enemyShip->getDuration() * 0.67f,
				Point(player->getPositionX(), player->getPositionY()));										// Part of time spent moving to player, the rest moving off screen
			enemyShip->runAction(action);

			CCLOG("Spawn Enemy Ship - MoveTo");
			enemyShip->runAction(
				Sequence::create(
					MoveBy::create(enemyShip->getDuration(),
						Point(-winSize.width - enemyShip->getContentSize().width, 0)),						// move off the screen its full width
					CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)						// Then set invisible if it reaches the target - TERMINATE WITH NULL
			);
		}
	}
}



void Level4::spawnEnemyShips(float curTimeMillis) {
	/*
	float randMillisecs = randomValueBetween(0.20F, 1.0F) * 2500;
	nextEnemyShipSpawnTime = randMillisecs + curTimeMillis;

	Game::Instance()->incrementEnemyShipCount();

	derpStar->init(visibleSize);

	cocos2d::MoveTo* action = MoveTo::create(derpStar->getDuration() * 0.67f,
		Point(player->getPositionX(), player->getPositionY()));										// Part of time spent moving to player, the rest moving off screen
	//derpStar->runAction(action);

	CCLOG("Spawn Enemy Ship - MoveTo");
	derpStar->runAction(
		Sequence::create(action,
			MoveBy::create(derpStar->getDuration(),
				Point(-winSize.width - derpStar->getContentSize().width, 0)),						// move off the screen its full width
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)						// Then set invisible if it reaches the target - TERMINATE WITH NULL
	);
	*/

}
/*
	Add the enemies to appear in this level in the order required
	Made function virtual not need to clear list now, as function in level not loaded
*/
void Level4::initEnemyShips() {
	
	// Add 2 x Enemy Ship Type 2 - These will appear first on screen
	for (unsigned int i = 0; i < L3_NUM_ENEMY_2; ++i) {
		EnemyShip* enemyShip2 = EnemyShipKling::create(visibleSize);
		this->addChild(enemyShip2);
		m_enemyShipList->pushBack(enemyShip2);
	}
	
	//derpStar = DerpStar::create(visibleSize);	// Create boss enemy
	//m_enemyShipList->pushBack(derpStar);							// Add to list of enemies
	//this->addChild(derpStar);										// Add to scene

	/*
	// Add 1 x Enemy Ship Type 1 - This will appear second
	for (unsigned int i = 0; i < L3_NUM_ENEMY_1; ++i) {
		EnemyShip* enemyShip1 = EnemyShip::create(visibleSize);
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
	}
	*/

	// Add 2 x Enemy Ship Type 3 - Third on screen
	for (unsigned int i = 0; i < 2; ++i) {
		EnemyShip* enemyShip3 = EnemyShipWilKnot::create(visibleSize);
		this->addChild(enemyShip3);
		m_enemyShipList->pushBack(enemyShip3);
	}	
	/*
	// Add 2 x Enemy Ship Type 1 - 4th
	for (unsigned int i = 0; i < L3_NUM_ENEMY_1 + 1; ++i) {
		EnemyShip* enemyShip1 = EnemyShip::create(visibleSize);
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
	}
	*/
	CCLOG("Add enemy ships");
}

void Level4::update(float dt) {
	derpStar->update(dt);
	Level::update(dt);									// Call base class update function		
}

void Level4::checkCollisions() {
	Level::checkCollisions();							// Call base class function
	// Check collisions with different objects in different levels

	// Check collisions between player laser and derpstar
	for (cocos2d::Sprite* shipLaser : *m_playerLaserList) {											// JOR replaced auto specifier
		if (!(shipLaser->isVisible())) continue;

		if (shipLaser->getBoundingBox().intersectsRect(derpStar->getBoundingBox())) {
			Audio::Instance()->explodeFX();															// Play explosion effect
			shipLaser->setVisible(false);															// Hide the player laser
			Game::Instance()->updateScore(20);														// Award 20 points for destroying an enemy ship
			derpStar->takeLife();
		}
	}
}

void Level4::endScene(EndReason endReason) {
	cocos2d::Label* derpMessage = cocos2d::Label::createWithTTF(messageEOL,
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);										// JOR replaced auto specifier
	derpMessage->setPosition(Point(visibleSize.width * 0.5f + origin.x,
		visibleSize.height - (winSize.height / 9 * 5.25f)));
	derpMessage->setColor(Color3B::WHITE);

	this->addChild(derpMessage);

	Level::endScene(endReason);							// End the scene

														// 3. Total Enemy Ships Destroyed
	

}


void Level4::checkGameOver(float currenTime) {															// If the player has run out of lives
	// DerpStar destroyed
	if (!derpStar->isVisible()) {
		derpStar->stopAllActions();
		derpStar->canon1->stopAllActions();
		messageEOL = "DerpStar Has Been Destroyed";
		endScene(KENDREASONWIN);
	}
	
	// Player Lost
	if (Game::Instance()->getLives() <= 0) {															// If the player has run out of lives
		player->stopAllActions();																		// CCNode.cpp
		player->setVisible(false);																		// Destroy the ship
		endScene(KENDREASONLOSE);																		// Player has died
	}
	// Time ran out
	else if (currenTime >= Game::Instance()->getEndTime()) {
		if (derpStar->isVisible()) {
			messageEOL = "Failed To Destroy The DerpStar";
			endScene(KENDREASONLOSE);																	// Player has lost if derpstar is still alive when time runs out
		}
		else {
			messageEOL = "You Win";
			endScene(KENDREASONWIN);																		// Player stays playing for the length of time
		}
	}
}
