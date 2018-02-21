/*
	Joe O'Regan
	Level2.cpp
	18/02/2018

	Added additional level
*/
#include "Level2.h"
#include "Level3.h"
#include "MainMenu.h"
#include "DPad.h"
#include <string>
#include <sstream>
#include "Input.h"

#define KNUMASTEROIDS 20						// Number of asteroids
#define KNUMLASERS 5							// Number of lasers

DPad *controller2;								// Add directional pad for mobile device

Scene* Level2::createScene() {  
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier   
	Level2* layer = Level2::create();			// 'layer' is an autorelease object, JOR replaced auto specifier   
    scene->addChild(layer);						// Add layer as a child to scene	    
    return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool Level2::init() {    
    if ( !Layer::init() ) { return false; }																// super init first

	Game::Instance()->setLevel(2);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	cocos2d::MenuItemImage* closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(Level2::menuCloseCallback, this));												// JOR replaced auto specifier

	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	cocos2d::Menu* menu = Menu::create(closeItem, NULL);												// JOR replaced auto specifier
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
	
	//  GALAXY
	_batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");
	this->addChild(_batchNode);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");
		
	// Player sprite
	player = new Player(this);
	_batchNode->addChild(player->getSprite(), 1);

	// 1) Create the ParallaxNode
	_backgroundNode = ParallaxNodeExtras::create();
	this->addChild(_backgroundNode, -1);
	_backgroundNode->init();																								// Initialise the parallax scrolling background
	
	Level2::addChild(ParticleSystemQuad::create("Stars1.plist"));
	Level2::addChild(ParticleSystemQuad::create("Stars2.plist"));
	Level2::addChild(ParticleSystemQuad::create("Stars3.plist"));

	_asteroids = new Vector<Sprite*>(KNUMASTEROIDS);																		// List of asteroids
	for (int i = 0; i < KNUMASTEROIDS; ++i) {
		cocos2d::Sprite* asteroid = Sprite::createWithSpriteFrameName("asteroid.png");										// Asteroid sprite, JOR replaced auto specifier
		asteroid->setVisible(false);
		_batchNode->addChild(asteroid);
		_asteroids->pushBack(asteroid);
	}

	// EnemyShip
	EnemyShipList = new Vector<Sprite*>(3);																					// List of enemy ships
	for (int i = 0; i < 3; ++i) {
		cocos2d::Sprite* enemyShip1 = Sprite::create("EnemyShip.png");														// Asteroid sprite, JOR replaced auto specifier
		enemyShip1->setVisible(false);
		this->addChild(enemyShip1);
		EnemyShipList->pushBack(enemyShip1);
	}

	_shipLasers = new Vector<Sprite*>(KNUMLASERS);																			// List of lasers
	for (int i = 0; i < KNUMLASERS; ++i) {
		cocos2d::Sprite* shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");								// Laser sprite, JOR replaced auto specifier
		shipLaser->setVisible(false);
		_batchNode->addChild(shipLaser);
		_shipLasers->pushBack(shipLaser);
	}

	Device::setAccelerometerEnabled(true);																					// Enable accelerometer
	cocos2d::EventListenerAcceleration* accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(Level2::onAcceleration, this));	// JOR replaced auto specifier
	_eventDispatcher->addEventListenerWithSceneGraphPriority(accelerationListener, this);

	cocos2d::EventListenerTouchAllAtOnce* touchListener = EventListenerTouchAllAtOnce::create();							// JOR replaced auto specifier
	touchListener->onTouchesBegan = CC_CALLBACK_2(Level2::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// Player Lives					
	for (unsigned int i = 0; i < Game::Instance()->getLives(); i++) {
		playerLife = Sprite::create("PlayerLife.png");
		playerLife->setPosition(visibleSize.width * 0.05 + (i * 52), visibleSize.height * 0.05);
		this->addChild(playerLife);
		livesList[i] = playerLife;																							// Add life sprite to list of lives
	}

	double curTime = getTimeTick();																							// Current game time
	_gameOverTime = curTime + LEVEL_TIME;																					// Time to finish game

	currentTime = 0.0f;																										// Current game time, for timer, changed to float to solve Android timer issue

	//Audio::Instance()->init();																							// Initialise the game audio
	HUD::Instance()->init(time, this);																						// Display score, level number, and time
	Input::Instance()->init(this, this->_eventDispatcher);																	// Ship Movement

	__String *tempScore = __String::createWithFormat("Score: %i", Game::Instance()->getScore());
	__String *tempTime = __String::createWithFormat("Time: %i", time);
	
	// Score & Timer set size
	//timeLabel->setPosition(Point(visibleSize.width - timeLabel->getWidth() - 250, visibleSize.height * 0.95 + origin.y));
	if (visibleSize.height == 1080) {
		scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.075f);
		timeLabel = Label::createWithTTF(tempTime->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.075f);
		timeLabel->setPosition(Point(visibleSize.width - timeLabel->getWidth() - 150, visibleSize.height * 0.95 + origin.y));
	}
	else {
		scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.05f);
		timeLabel = Label::createWithTTF(tempTime->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.05f);
		timeLabel->setPosition(Point(visibleSize.width - timeLabel->getWidth() - 80, visibleSize.height * 0.95 + origin.y));
	}

	scoreLabel->setColor(Color3B::WHITE);
	scoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.95 + origin.y));

	this->addChild(scoreLabel, 10000);
	this->addChild(timeLabel);
	
	// D-pad (Display on mobile device)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (visibleSize.height == 1080)
			controller2 = DPad::create("DPad/Base300.png", "DPad/Arrow96.png", "DPad/Arrow96Pressed.png", Point(250, 250));
		else
			controller2 = DPad::create("DPad/Base150.png", "DPad/Arrow.png", "DPad/ArrowPressed.png", Point(150, 150));

		this->addChild(controller2);
	}
	

	this->scheduleUpdate();

    return true;
}

void Level2::update(float dt) {
	float curTimeMillis = getTimeTick();																	// Current game time
	winSize = Director::getInstance()->getWinSize();														// Dimensions of game screen
	
    scoreLabel->setString("Score: " + to_string(Game::Instance()->getScore()));								// Update the displayed score
	//HUD::Instance()->update();																			// Update the score (Not working)

	getInput();																								// Get keyboard input for Windows, Get DPad input for Android
	updateTimer(curTimeMillis);																				// Update the countdown timer, pass in curTimeMillies solves Android Timer issue
	_backgroundNode->update(dt);																			// Scroll the background objects
	spawnAsteroids(curTimeMillis);																			// Spawn asteroids
	spawnEnemyShips(curTimeMillis);																			// Spawn asteroids
	checkCollisions();																						// Check have game objects collided with each other
	checkGameOver(curTimeMillis);																			// Check is the game over or not

	player->update();																						// Update player sprite position

	// Update displayed lives
	//if (Game::Instance()->getLives() < MAX_LIVES && !_gameOver) {											// If the players lives are less than the max num lives
	//if (Game::Instance()->getLives() < MAX_LIVES) {															// If the players lives are less than 3
	//	livesList[Game::Instance()->getLives()]->setVisible(false);											// Set the lives invisible (2,1,0)
	//}

	// Update the enemy ship position
	//EnemyShip->setPosition(EnemyShip->getPosition().x - 2, EnemyShip->getPosition().y);
}

void Level2::getInput() {
	// Android DPad (maybe change to returning a point (0,0)(1,0)(0,1),(-1,0),(0,-1)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (controller2->getButton(8)->isSelected()) player->moveUp();			// up
		else if (controller2->getButton(2)->isSelected()) player->moveDown();	// down

		if (controller2->getButton(4)->isSelected()) player->moveLeft();		// left
		else if (controller2->getButton(6)->isSelected()) player->moveRight();	// right
	}
}

void Level2::updateTimer(float curTimeMillis) {
	if (curTimeMillis > currentTime) {
		currentTime = curTimeMillis + 1000.0f;
		time--;
	}
	timeLabel->setString("Time: " + to_string(time));
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
				CallFuncN::create(CC_CALLBACK_1(Level2::setInvisible, this)),
				NULL /* DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)*/)
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
				CallFuncN::create(CC_CALLBACK_1(Level2::setInvisible, this)),
				NULL /* TERMINATE WITH NULL */)
		);
	}
}

void Level2::checkCollisions() {
	// Asteroids Collisions
	for (cocos2d::Sprite* asteroid : *_asteroids) {															// JOR replaced auto specifier
		if (!(asteroid->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *_shipLasers) {													// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {
				Audio::Instance()->explodeFX();
				shipLaser->setVisible(false);
				asteroid->setVisible(false);
				Game::Instance()->updateScore(10);															// Award 10 points for destroying an asteroid
			}
		}

		// Check collisions between the player ship and asteroids
		if (player->getSprite()->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {				// If the ship collides with an asteroid
			asteroid->setVisible(false);																	// Destroy the asteroid
			player->getSprite()->runAction(Blink::create(1.0F, 9));											// Flash the Player ship
			Game::Instance()->takeLife();																	// Decrement the number of lives
			livesList[Game::Instance()->getLives()]->setVisible(false);										// Set the lives invisible (2,1,0)
		}
	}

	// Enemy ship collisions
	for (cocos2d::Sprite* enemyShip : *EnemyShipList) {														// JOR replaced auto specifier
		if (!(enemyShip->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *_shipLasers) {													// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(enemyShip->getBoundingBox())) {
				Audio::Instance()->explodeFX();
				shipLaser->setVisible(false);
				enemyShip->setVisible(false);
				Game::Instance()->updateScore(20);															// Award 20 points for destroying an enemy ship
			}
		}
	}
}

void Level2::checkGameOver(float currenTime) {
	if (Game::Instance()->getLives() <= 0) {																// If the player has run out of lives
		player->getSprite()->stopAllActions();																// CCNode.cpp
		player->getSprite()->setVisible(false);																// Destroy the ship
		this->endScene(KENDREASON2LOSE);																	// Player has died
	}
	else if (currenTime >= _gameOverTime) {
		this->endScene(KENDREASON2WIN);																		// Player stays playing for the length of time
	}
}

 float Level2::randomValueBetween(float low, float high) {
	// from http://stackoverflow.com/questions/686353/c-random-float-number-generation
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

float Level2::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
}

void Level2::setInvisible(Node * node) {
	node->setVisible(false);
}

void Level2::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event){
	Audio::Instance()->laserFX();
	cocos2d::Size winSize = Director::getInstance()->getWinSize();									// JOR replaced auto specifier

	//spawnLaser();
	spawn2Lasers();
}

void Level2::spawnLaser() {
	cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);									// Next laser in the list, JOR replaced auto specifier
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;																			// Reset laser list index to 0 (go back to start of list)
	
	shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width/2, 0));
	shipLaser->setVisible(true);
	shipLaser->stopAllActions();
	shipLaser->runAction(
		Sequence::create(
			MoveBy::create(0.5, Point(winSize.width, 0)),
			CallFuncN::create(CC_CALLBACK_1(Level2::setInvisible, this)),
			NULL));
}
void Level2::spawn2Lasers() {
	cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);														// Next laser in the list, JOR replaced auto specifier
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;																								// Reset laser list index to 0 (go back to start of list)
	cocos2d::Sprite* shipLaser2 = _shipLasers->at(_nextShipLaser++);													// Next laser in the list, JOR replaced auto specifier
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;																								// Reset laser list index to 0 (go back to start of list)

	shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 12));
	shipLaser->setVisible(true);
	shipLaser->stopAllActions();
	shipLaser->runAction(
		Sequence::create(
			MoveBy::create(0.5, Point(winSize.width, 0)),
			CallFuncN::create(CC_CALLBACK_1(Level2::setInvisible, this)),
			NULL)); 
	shipLaser2->setPosition(player->getSprite()->getPosition() + Point(shipLaser2->getContentSize().width / 2, -12));
	shipLaser2->setVisible(true);
	shipLaser2->stopAllActions();
	shipLaser2->runAction(
		Sequence::create(
			MoveBy::create(0.5, Point(winSize.width, 0)),
			CallFuncN::create(CC_CALLBACK_1(Level2::setInvisible, this)),
			NULL));
}

void Level2::restartTapped(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, this->createScene()));							// Restart the level
	
	this->scheduleUpdate();																									// reschedule									
}

void Level2::startLevel3(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, Level3::createScene()));							// Load level 3	
}

void Level2::returnToMenu(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5, MainMenu::createScene()));						// Return to the main menu
}

void Level2::endScene(EndReason2 endReason) {
	if (_gameOver) return;																									// If already game over, skip this function
	_gameOver = true;																										// Set game over

	cocos2d::Size winSize = Director::getInstance()->getWinSize();															// JOR replaced auto specifier

	Game::Instance()->checkHighScore();																						// The game has ended, check if the current score is the high score and save it if it is

	// Win / Lose Message
	char message[17] = "Level 2 Complete";
	if (endReason == KENDREASON2LOSE) strcpy(message, "You Lose");

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
	if (endReason != KENDREASON2LOSE) {
		strcpy(message, "Continue");
		cocos2d::Label* continueLbl = Label::createWithBMFont("Arial.fnt", message);										// JOR replaced auto specifier
		cocos2d::MenuItemLabel* continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level2::startLevel3,this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
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
		cocos2d::MenuItemLabel* mainMenuItem = MenuItemLabel::create(menuLbl, CC_CALLBACK_1(Level2::returnToMenu, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

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
	
	// Terminate update callback
	this->unscheduleUpdate();
}

void Level2::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
