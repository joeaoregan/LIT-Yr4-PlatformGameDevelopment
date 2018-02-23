/*
	Joe O'Regan
	Level2.cpp
	18/02/2018

	Added additional level
*/
#include "Level.h"
#include "Level2.h"
#include "MainMenu.h"
#include <string>
#include <sstream>
#include "Input.h"

Scene* Level2::createScene() {  
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier   
	Level2* layer = Level2::create();			// 'layer' is an autorelease object, JOR replaced auto specifier  
	layer->setName("Level2");					// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(layer);						// Add layer as a child to scene	    
    return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool Level2::init() {
	Level::init();																							// 20180221 Added Level base class

    if ( !Layer::init() ) { return false; }																	// super init first

	Game::Instance()->setLevel(2);																			// Specific to level 2
		
	this->addChild(menuClose, 1);																			// add a "close" icon to exit the progress. it's an autorelease object (initialied in Level class)
	
	//  GALAXY
	this->addChild(_batchNode);

	// 1) Create the ParallaxNode
	this->addChild(_backgroundNode, -1);
	_backgroundNode->init();																				// Initialise the parallax scrolling background
	
	Level2::addChild(ParticleSystemQuad::create("Stars1.plist"));
	Level2::addChild(ParticleSystemQuad::create("Stars2.plist"));
	Level2::addChild(ParticleSystemQuad::create("Stars3.plist"));
	
	// EnemyShip
	for (int i = 0; i < 3; ++i) {
		cocos2d::Sprite* enemyShip1 = Sprite::create("EnemyShip.png");										// Asteroid sprite, JOR replaced auto specifier
		enemyShip1->setVisible(false);
		this->addChild(enemyShip1);
		EnemyShipList->pushBack(enemyShip1);
	}
	
	touchListener->onTouchesBegan = CC_CALLBACK_2(Level2::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// Player Lives					
	for (unsigned int i = 0; i < Game::Instance()->getLives(); i++) {
		playerLife = Sprite::create("PlayerLife.png");
		playerLife->setPosition(visibleSize.width * 0.05 + (i * 52), visibleSize.height * 0.05);
		this->addChild(playerLife);
		livesList[i] = playerLife;																			// Add life sprite to list of lives
	}

	_gameOverTime = curTime + LEVEL_TIME;																	// Time to finish game
	
	HUD::Instance()->init(time, this);																		// Display score, level number, and time
	Input::Instance()->init(this, this->_eventDispatcher);													// Ship Movement
	
	this->addChild(scoreLabel, 10000);
	this->addChild(timeLabel);
	
	// D-pad (Display on mobile device)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		this->addChild(controller);
	}	

	this->addChild(mplayer);

	this->scheduleUpdate();																					// Start updating the scene

    return true;
}

void Level2::update(float dt) {
	Level::update(dt);																						// Call base class update function
		
	spawnAsteroids(curTimeMillis);																			// Spawn asteroids
	spawnEnemyShips(curTimeMillis);																			// Spawn asteroids
	checkCollisions();																						// Check have game objects collided with each other
	checkGameOver(curTimeMillis);																			// Check is the game over or not
}

void Level2::spawnAsteroids(float curTimeMillis) {
	if (curTimeMillis > _nextAsteroidSpawn) {
		float randMillisecs = randomValueBetween(0.20F, 1.0F) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;

		float randY = randomValueBetween(0.0F, winSize.height);												// Random Y position for asteroid
		float randDuration = randomValueBetween(2.0F, 10.0F);

		Sprite *asteroid = _asteroids->at(_nextAsteroid);
		_nextAsteroid++;																					// Increment the asteroid

		if (_nextAsteroid >= _asteroids->size()) _nextAsteroid = 0;											// Loop back around to start of asteroids list

		asteroid->stopAllActions();																			// CCNode.cpp
		asteroid->setPosition(winSize.width + asteroid->getContentSize().width / 2, randY);
		asteroid->setVisible(true);
		asteroid->runAction(
			Sequence::create(
				MoveBy::create(randDuration, Point(-winSize.width - asteroid->getContentSize().width, 0)),
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)							// DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
		);
	}
}

void Level2::spawnEnemyShips(float curTimeMillis) {
	if (curTimeMillis > nextEnemyShipSpawnTime) {
		float randMillisecs = randomValueBetween(0.20F, 1.0F) * 2500;
		nextEnemyShipSpawnTime = randMillisecs + curTimeMillis;												// Set the time to spawn the next ship

		float randY = randomValueBetween(0.0F, winSize.height);												// Random Y position for enemy ship
		float randDuration = randomValueBetween(2.0F, 10.0F);

		Sprite *enemyShip = EnemyShipList->at(nextEnemyShip);
		nextEnemyShip++;																					// Increment the enemy ship on the list

		if (nextEnemyShip >= EnemyShipList->size()) nextEnemyShip = 0;										// Loop back around to start of enemy ship list

		enemyShip->stopAllActions();																		// CCNode.cpp
		enemyShip->setPosition(winSize.width + enemyShip->getContentSize().width / 2, randY);
		enemyShip->setVisible(true);

		enemyShip->runAction(
			Sequence::create(
				MoveBy::create(randDuration, Point(-winSize.width - enemyShip->getContentSize().width, 0)),
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)							// TERMINATE WITH NULL
		);
	}
}

void Level2::spawnLasers(int amount) {																		// 20180221
	//int i;
	//for ((amount == 2) ? i = 1 : i = 1; i < amount; i++) {
	for (int i = 0; i < amount; i++) {
		cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);										// Next laser in the list, JOR replaced auto specifier
		if (_nextShipLaser >= _shipLasers->size())
			_nextShipLaser = 0;																				// Reset laser list index to 0 (go back to start of list)

		if (i == 0) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 0));	// middle
		if (i == 1) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, -12));	// bottom
		if (i == 2) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 12));	// top

		shipLaser->setVisible(true);
		shipLaser->stopAllActions();

		shipLaser->runAction(
			Sequence::create(MoveBy::create(0.5, Point(winSize.width, 0)), // change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
	}
}

void Level2::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event) {
	Audio::Instance()->laserFX();
	cocos2d::Size winSize = Director::getInstance()->getWinSize();											// JOR replaced auto specifier

	spawnLasers(2);
}

void Level2::checkCollisions() {
	Level::checkCollisions();																				// Call base class function
																											// Check collisions with different objects in different levels
}

void Level2::checkGameOver(float currenTime) {
	if (Game::Instance()->getLives() <= 0) {																// If the player has run out of lives
		player->getSprite()->stopAllActions();																// CCNode.cpp
		player->getSprite()->setVisible(false);																// Destroy the ship
		this->endScene(KENDREASONLOSE);																		// Player has died
	}
	else if (currenTime >= _gameOverTime) {
		this->endScene(KENDREASONWIN);																		// Player stays playing for the length of time
	}
}

void Level2::restartTapped(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, this->createScene()));							// Restart the level
	
	this->scheduleUpdate();																									// reschedule									
}

void Level2::endScene(EndReason endReason) {
	if (_gameOver) return;																									// If already game over, skip this function
	_gameOver = true;																										// Set game over

	cocos2d::Size winSize = Director::getInstance()->getWinSize();															// JOR replaced auto specifier

	Game::Instance()->checkHighScore();																						// The game has ended, check if the current score is the high score and save it if it is

	// Win / Lose Message
	char message[17] = "Level 2 Complete";
	if (endReason == KENDREASONLOSE) strcpy(message, "You Lose");

	cocos2d::Label* label = Label::createWithBMFont("Arial.fnt", message);													// JOR replaced auto specifier
	if (winSize.height == 720)
		label->setScale(0.2f);
	else
		label->setScale(0.3F);																								// JOR Use larger label for 1080p

	label->setPosition(winSize.width / 2, winSize.height*0.75f);
	this->addChild(label);
	
	// Restart Level
	strcpy(message, "Restart Level");	
	cocos2d::Label* restartLbl = Label::createWithBMFont("Arial.fnt", message);												// JOR replaced auto specifier
	cocos2d::MenuItemLabel* restartItem=MenuItemLabel::create(restartLbl,CC_CALLBACK_1(Level2::restartTapped,this));		// JOR replaced auto specifier
	restartItem->setScale(0.1F);
	restartItem->setPosition(winSize.width / 2, winSize.height*0.5f);

	cocos2d::Menu* menu = Menu::create(restartItem, NULL);																	// JOR replaced auto specifier
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Continue To Next Level
	if (endReason != KENDREASONLOSE) {
		strcpy(message, "Continue");
		cocos2d::Label* continueLbl = Label::createWithBMFont("Arial.fnt", message);										// JOR replaced auto specifier
		cocos2d::MenuItemLabel* continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::startLevel3,this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
																															//restartItem->setScale(0.1F);
		if (winSize.height == 720)
			continueItem->setScale(0.1F);
		else
			continueItem->setScale(0.15F);																					// JOR Use larger size for 1080p
		continueItem->setPosition(winSize.width / 2, winSize.height*0.25f);

		cocos2d::Menu* menu2 = Menu::create(continueItem, NULL);															// JOR replaced auto specifier
		menu2->setPosition(Point::ZERO);
		this->addChild(menu2);

		continueItem->runAction(ScaleTo::create(0.5F, 1.0F));
	} else {
		// Return To Main Menu
		strcpy(message, "Main Menu");
		cocos2d::Label* menuLbl = Label::createWithBMFont("Arial.fnt", message);											// JOR replaced auto specifier
		cocos2d::MenuItemLabel* mainMenuItem = MenuItemLabel::create(menuLbl, CC_CALLBACK_1(Level::returnToMenu, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		if (winSize.height == 720)
			mainMenuItem->setScale(0.1F);
		else
			mainMenuItem->setScale(0.15F);																					// JOR Use larger size for 1080p
		mainMenuItem->setPosition(winSize.width / 2, winSize.height*0.25f);

		cocos2d::Menu* menu2 = Menu::create(mainMenuItem, NULL);															// JOR replaced auto specifier
		menu2->setPosition(Point::ZERO);
		this->addChild(menu2);

		mainMenuItem->runAction(ScaleTo::create(0.5F, 1.0F));
	}

	// clear label and menu
	restartItem->runAction(ScaleTo::create(0.5F, 1.0F));
	label->runAction(ScaleTo::create(0.5F, 1.0F));
		
	this->unscheduleUpdate();																								// Terminate update callback
}