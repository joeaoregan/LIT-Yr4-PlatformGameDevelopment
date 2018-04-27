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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"													// For leaderboard and achievements
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"										// 20180307 Google Analytics
#endif

/*
	Create level 4 scene
*/
Scene* Level4::createScene() {
	cocos2d::Scene* scene = Scene::create();													// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level4::create();														// 'layer' is an autorelease object, JOR replaced auto specifier  
	//layer->setName("Level4");																	// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
	scene->addChild(s_pLayerInstance);															// Add layer as a child to scene	    
	return scene;																				// Return the scene
}

/*
	Initialisation specific to Level 4
*/
bool Level4::init() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginGoogleAnalytics::logEvent("Achievement",
		"Unlocked", "Level 4 Started", 5);														// Google Analytics
#endif

	Level::init();																				// 20180221 Added Level base class

	Game::Instance()->setLevel(4);																// Specific to level 2
	m_pHUD->setLevelLabel();																	// Update HUD Level text display

	if (!Layer::init()) { return false; }														// super init first
	
	// ParallaxNode
	this->addChild(m_backgroundNode, -1);														// Add the parallax background
	m_backgroundNode->init();																	// Initialise the parallax scrolling background
	
	this->scheduleUpdate();																		// Start updating the scene
	
	m_pDerpStar = DerpStar::create(m_visibleSize);												// Create boss enemy
	this->addChild(m_pDerpStar);																// Add to scene
	
	cocos2d::MoveTo* derpForward = MoveTo::create(4.0f,											// Move the Boss towards the player
	Point(m_visibleSize.width * 0.75f, player->getPositionY()));	

	cocos2d::MoveTo* derpBack = MoveTo::create(2.0f,											// Move the boss backwards and off screen
		Point(m_visibleSize.width + m_pDerpStar->getContentSize().width, 
			player->getPositionY()));
	
	//derpStar->runAction(actionDerp);
	//auto func = CallFuncN::create([](Ref* sender) {
	//	//spawnDerpLaser();
	//	CCLOG("test");
	//	//derpStar->init(0);
	//	cocos2d::Sprite* sprite = dyanamic_cast<cocos2d::Sprite*>(sender);
	//});

	auto func = CallFunc::create(CC_CALLBACK_0(Level4::spawnDerpLaser, this));					// Spawn lasers as part of the movement action sequence
	auto func1 = CallFunc::create(CC_CALLBACK_0(Level4::spawnShips, this));						// Spawn ships when boss moves off screen

	auto sequence = Sequence::create(derpForward,
		MoveBy::create(2.5f,					
			Point(m_winSize.width - m_pDerpStar->getContentSize().width,						// move off the screen its full width
				player->getPositionY() - (m_pDerpStar->getContentSize().height / 2))),
		//CallFuncN::create(CC_CALLBACK_1(Level4::spawnDerpLaser(), this)), NULL)				// Tried manipulating the setVisible callback in Level class
		//CallFuncN::create(spawnDerpLaser(), NULL)	
		func, derpBack, func, func1, NULL);

	auto repeat = RepeatForever::create(sequence);												// Repeat the sequence

	//derpStar->setPosition(Point(visibleSize.width*0.75f, visibleSize.height / 2));
	m_pDerpStar->runAction(repeat);																// Apply the action ot the boss

	auto action3 = RotateBy::create(2.0f, 60.0f);												// Starting at -30, rotate to 60
	auto action4 = RotateBy::create(2.0f, -60.0f);												// Then back again
	auto fire1 = CallFunc::create(CC_CALLBACK_0(Level4::spawnCanonLaser1, this));				// Spawn lasers in upwards direction
	auto fire2 = CallFunc::create(CC_CALLBACK_0(Level4::spawnCanonLaser2, this));				// Spawn lasers in downwards direction

	auto canonSequence = Sequence::create(action3, fire1, action4, fire2, NULL);				// Rotate 60 degrees clockwise, fire, rotate back
	auto canonRepeat = RepeatForever::create(canonSequence);									// Then loop the canon actions

	m_pDerpStar->m_pCanon1->runAction(canonRepeat);												// Run the action
	//derpStar->canon1->setVisible(false);

	// Initialise lasers
	m_enemyLaserList3 = new Vector<Sprite*>(NUM_LASERS);										// List of lasers
	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* enemyLaser = Sprite::create(LASER_GREEN_IMG);							// Laser sprite, JOR replaced auto specifier
		enemyLaser->setVisible(false);
		this->addChild(enemyLaser);
		m_enemyLaserList3->pushBack(enemyLaser);
	}

	return true;
}

/*
	Spawn Derp Star lasers in upwards direction
	Using trigonometry to work out the point to move the laser to
*/
void Level4::spawnCanonLaser1() {
	CCLOG("**************************************************Canon Fire function called");
	float laserY = m_visibleSize.width * 0.577;													// Use tan to calculate the Y value of destination coordinate
	//if (derpStar->canon1->getRotation() > 30)
	//	laserY *= -1; // invert y direction
	//float distance = sqrt((visibleSize.width * visibleSize.width) + (laserY * laserY));
	//CCLOG("spawnCanonLaser1 distance %f", distance);

	cocos2d::Sprite* enemyLaser;																// Laswer sprite
	enemyLaser = m_enemyLaserList3->at(m_nextEnemyLaser);										// next blue laser
	m_nextEnemyLaser++;																			// The next sprite
	if (m_nextEnemyLaser >= m_enemyLaserList3->size()) m_nextEnemyLaser = 0;					// Loop index back to start if at end
	if (enemyLaser->isVisible()) return;														// If the laser is already visible skip it
	Audio::Instance()->playFX(LASER_ENEMY4);															// Play audio

	//enemyLaser->setPosition(derpStar->getPosition().x + derpStar->getContentSize().width * 0.6f, 
	//	(derpStar->getPosition().y/2) + derpStar->getContentSize().height / 2);					// Set the postion relevant to the ships coordinates

	enemyLaser->setPosition(m_pDerpStar->getPosition().x, m_pDerpStar->getPosition().y * 1.05f);// Set laser spawn position (offset from canon position)
	//enemyLaser->setPosition(derpStar->getPosition().x - 
	//	(derpStar->getContentSize().width / 2) + 
	//	derpStar->canon1->getPosition().x, derpStar->getPosition().y * 1.05f);
	enemyLaser->setRotation(30.0f);																// Set the laser rotation to 30 degrees, to appear travelling in straight line

	enemyLaser->stopAllActions();																// Stop actions for the laser
	enemyLaser->setVisible(true);																// Set visible on screen

	float x = m_pDerpStar->m_pCanon1->getPosition().x - m_visibleSize.width;					// Where to spawn the laser from, x axis
	float y = m_pDerpStar->m_pCanon1->getPosition().y + laserY;									// Y axis

	CCLOG("Laser end coords X: %f, Y: %f", x, y);

	enemyLaser->runAction(
		Sequence::create(MoveBy::create(0.5f, Point(x, y)),										// change to plus 100 for up - 100 for down
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));				// Set invisible when it reaches its destination using call back function
}

/*
	Spawn Derp Star lasers in downwards direction
*/
void Level4::spawnCanonLaser2() {
	float laserY = m_visibleSize.width * 0.577 * -1;											// Use tan to get y point in opposite direction for bottom canon angle (canon1)

	cocos2d::Sprite* enemyLaser;																// Laser sprite
	enemyLaser = m_enemyLaserList3->at(m_nextEnemyLaser);										// Next laser
	m_nextEnemyLaser++;
	if (m_nextEnemyLaser >= m_enemyLaserList3->size()) m_nextEnemyLaser = 0;
	if (enemyLaser->isVisible()) return;														// If the laser is already visible skip it
	Audio::Instance()->playFX(LASER_ENEMY4);														// Play audio

	//enemyLaser->setPosition(derpStar->getPosition().x + getContentSize().width * 0.6f, 
	//	derpStar->getPosition().y + getContentSize().height / 2);
	enemyLaser->setPosition(m_pDerpStar->getPosition().x, m_pDerpStar->getPosition().y * 0.95f);// Position of the canon on the boss sprite
	enemyLaser->setRotation(-30.0f);															// Set the postion relevant to the ships coordinates
	enemyLaser->setVisible(true);																// Set visible on screen
	enemyLaser->stopAllActions();																// Stop actions for the laser

	float x = m_pDerpStar->m_pCanon1->getPosition().x - m_visibleSize.width;					// x position to move the laser to
	float y = m_pDerpStar->m_pCanon1->getPosition().y + laserY;									// y position
	
	enemyLaser->runAction(
		Sequence::create(MoveBy::create(0.5f, Point(x, y)),										// change to plus 100 for up - 100 for down
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));				// Set invisible once it reaches destination
}

/*
	Spawn lasers for enemy boss
*/
void Level4::spawnDerpLaser() {
	CCLOG("Spawn lasers");																		// Set the node invisible

	m_pDerpStar->spawnLaser();																	// Call boss spawn laser function
}

/*
	Separate spawn ships function for level 4
	Called each time the enemy boss moves off screen
	No spawn time required
*/
void Level4::spawnShips() {
	CCLOG("spawn ships");																		// Set the node invisible

	// Spawn 2 enemy ships
	for (int i = 0; i < 2; i++) {
		if (m_enemyShipList->size() > 0) {
			EnemyShip *enemyShip = m_enemyShipList->at(m_nextEnemyShip);						// Create an Enemy ship
			m_nextEnemyShip++;																	// Increment the enemy ship list index
			if (m_nextEnemyShip >= (unsigned int)m_enemyShipList->size()) m_nextEnemyShip = 0;	// Loop back around to start of list if at the end
			Game::Instance()->incrementEnemyShipCount();
			enemyShip->init(m_visibleSize);

			cocos2d::MoveTo* action = MoveTo::create(enemyShip->getDuration() * 0.67f,			// For 2/3s of the duration move towards player coordinates
				Point(player->getPositionX(), player->getPositionY()));							// Part of time spent moving to player, the rest moving off screen
			enemyShip->runAction(action);

			CCLOG("Spawn Enemy Ship - MoveTo");

			// Move the enemy ship 
			enemyShip->runAction( Sequence::create( MoveBy::create(enemyShip->getDuration(),	// move ship to specified location off screen
						Point(-m_winSize.width - enemyShip->getContentSize().width, 0)),		// move off the screen its full width
					CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)			// Then set invisible if it reaches the target - TERMINATE WITH NULL
			);
		}
	}
}

/*
	Spawn the enemy ships for level 4
	Previous funtion used, no spawn time required
*/
void Level4::spawnEnemyShips(float curTimeMillis) {
	/*
	float randMillisecs = randomValueBetween(0.20F, 1.0F) * 2500;
	nextEnemyShipSpawnTime = randMillisecs + curTimeMillis;

	Game::Instance()->incrementEnemyShipCount();

	derpStar->init(visibleSize);

	cocos2d::MoveTo* action = MoveTo::create(derpStar->getDuration() * 0.67f,
		Point(player->getPositionX(), player->getPositionY()));									// Part of time spent moving to player, the rest moving off screen
	//derpStar->runAction(action);

	CCLOG("Spawn Enemy Ship - MoveTo");
	derpStar->runAction(
		Sequence::create(action,
			MoveBy::create(derpStar->getDuration(),
				Point(-winSize.width - derpStar->getContentSize().width, 0)),					// move off the screen its full width
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)					// Then set invisible if it reaches the target - TERMINATE WITH NULL
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
		EnemyShip* enemyShip2 = EnemyShipKling::create(m_visibleSize);							// Add type 2 enemies
		this->addChild(enemyShip2);
		m_enemyShipList->pushBack(enemyShip2);
	}
	
	//derpStar = DerpStar::create(visibleSize);													// Create boss enemy
	//m_enemyShipList->pushBack(derpStar);														// Add to list of enemies
	//this->addChild(derpStar);																	// Add to scene
	/*
	// Add 1 x Enemy Ship Type 1 - This will appear second
	for (unsigned int i = 0; i < L3_NUM_ENEMY_1; ++i) {
		EnemyShip* enemyShip1 = EnemyShip::create(visibleSize);
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
	} */

	// Add 2 x Enemy Ship Type 3 - Third on screen
	for (unsigned int i = 0; i < 2; ++i) {
		EnemyShip* enemyShip3 = EnemyShipWilKnot::create(m_visibleSize);						// Add type 3 enemies
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

/*
	Update level 4
*/
void Level4::update(float dt) {
	m_pDerpStar->update(dt);
	Level::update(dt);																			// Call base class update function		
}

/*
	Check collisions between ships and lasers
*/
void Level4::checkCollisions() {
	Level::checkCollisions();																	// Call base class function
	// Check collisions with different objects in different levels

	// Check collisions between player laser and derpstar
	for (cocos2d::Sprite* shipLaser : *m_playerLaserList) {										// JOR replaced auto specifier
		if (!(shipLaser->isVisible())) continue;

		if (shipLaser->getBoundingBox().intersectsRect(m_pDerpStar->getBoundingBox())) {
			Audio::Instance()->playFX(EXPLOSION_LARGE);											// Play explosion effect
			shipLaser->setVisible(false);														// Hide the player laser
			Game::Instance()->updateScore(20);													// Award 20 points for destroying an enemy ship
			m_pDerpStar->damage();
		}
	}

	// Check collisions laser type 2 (Orange)
	for (cocos2d::Sprite* enemyLaser : *m_enemyLaserList2) {
		if (!(enemyLaser->isVisible())) continue;

		if (enemyLaser->getPosition().x <= 0)													// If the laser moves off screen it's own width
			enemyLaser->setVisible(false);														// Hide the laser

																								// Check collisions between the player ship and Laser type 2 (Orange)
		if (player->getBoundingBox().intersectsRect(enemyLaser->getBoundingBox())) {			// If the ship collides with an asteroid
			enemyLaser->setVisible(false);														// Destroy the asteroid
			player->damage();
		}
	}

	// Check collisions for laser type 3 (Green)
	for (cocos2d::Sprite* enemyLaser : *m_enemyLaserList3) {
		if (!(enemyLaser->isVisible())) continue;

		if (enemyLaser->getPosition().x <= 0)													// If the laser moves off screen it's own width
			enemyLaser->setVisible(false);														// Hide the laser

																								// Check collisions between the player ship and Laser type 3 (Green)
		if (player->getBoundingBox().intersectsRect(enemyLaser->getBoundingBox())) {			// If the ship collides with an asteroid
			enemyLaser->setVisible(false);														// Destroy the asteroid
			player->damage();
		}
	}
}

/*
	Add extra label to give game over message indicating if Boss killed or not	
*/
void Level4::endScene(EndReason endReason) {
	cocos2d::Label* derpMessage = cocos2d::Label::createWithTTF(m_messageEOL,
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05);							// JOR replaced auto specifier
	derpMessage->setPosition(Point(m_visibleSize.width * 0.5f + m_Origin.x,
		m_visibleSize.height - (m_winSize.height / 9 * 5.25f)));
	derpMessage->setColor(Color3B::WHITE);

	this->addChild(derpMessage);

	Level::endScene(endReason);																	// End the scene

	restartLbl->setVisible(false);
}

/*
	Check game over for level 4, taking the enemy boss being alive into account
*/
void Level4::checkGameOver(float currenTime) {													// If the player has run out of lives
	// DerpStar destroyed
	if (!m_pDerpStar->isVisible()) {
		m_pDerpStar->stopAllActions();
		m_pDerpStar->m_pCanon1->stopAllActions();
		m_messageEOL = "DerpStar Has Been Destroyed";
		Game::Instance()->setWon(true);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Game Finished", 5);	// Google Analytics
		//sdkbox::PluginSdkboxPlay::unlockAchievement("Level 4 Complete");						// Achievement
		sdkbox::PluginSdkboxPlay::unlockAchievement("Game Complete");							// Achievement
#endif

		endLevelAchievement(KENDREASONWIN);														// Generic complete level achievements for console and analytics

		endScene(KENDREASONWIN);
	}
	
	// Player Lost
	if (Game::Instance()->getLives() <= 0) {													// If the player has run out of lives
		player->stopAllActions();																// CCNode.SpaceQuest
		player->setVisible(false);																// Destroy the ship
		endScene(KENDREASONLOSE);																// Player has died
	}

	// Time ran out
	else if (currenTime >= Game::Instance()->getEndTime()) {
		if (m_pDerpStar->isVisible()) {
			m_messageEOL = "Failed To Destroy The DerpStar";
			endScene(KENDREASONLOSE);															// Player has lost if derpstar is still alive when time runs out
		} else {
			m_messageEOL = "You Win";
			Game::Instance()->setWon(true);
			endScene(KENDREASONWIN);															// Player stays playing for the length of time
		}
	}
}
