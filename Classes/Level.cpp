/*
	Level.cpp

	Joe O'Regan
	K00203642
	21/02/2018

	Level base class
*/
#include "Level.h"
#include "Level2.h"
#include "Level3.h"
#include "MainMenu.h"
#include "DPad.h"
#include <string>
#include <sstream>
#include "Input.h"

// Because cocos2d-x requres createScene to be static, we need to make other non-pointer members static
std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> Input::keys;

//Input* Input::s_pInstance;																			// Singleton for Input


Scene* Level::createScene() {
	cocos2d::Scene* scene = Scene::create();															// 'scene' is an autorelease object, JOR replaced auto specifier   
	Level* layer = Level::create();																		// 'layer' is an autorelease object, JOR replaced auto specifier   
    scene->addChild(layer);																				// Add layer as a child to scene	    
    return scene;																						// Return the scene
}

// on "init" you need to initialize your instance
bool Level::init() {
    if ( !Layer::init() ) { return false; }																// super init first

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
			
	// add a "close" icon to exit the progress. it's an autorelease object
	closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(Level::menuCloseCallback, this));													// JOR replaced auto specifier

	
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	
	// create menu, it's an autorelease object
	menuClose = Menu::create(closeItem, NULL);															// JOR replaced auto specifier
	menuClose->setPosition(Point::ZERO);
	
	_batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");

	// Player sprite
	player = new Player(this);
	_batchNode->addChild(player->getSprite(), 1);

	// 1) Create the ParallaxNode
	_backgroundNode = ParallaxNodeExtras::create();
	//this->addChild(_backgroundNode, -1);
	//_backgroundNode->init();																			// ParallaxNodeExtras.cpp: Initialise the parallax scrolling background

	_asteroids = new Vector<Sprite*>(KNUMASTEROIDS);													// List of asteroids
	for (int i = 0; i < KNUMASTEROIDS; ++i) {
		cocos2d::Sprite* asteroid = Sprite::createWithSpriteFrameName("asteroid.png");					// Asteroid sprite, JOR replaced auto specifier
		asteroid->setVisible(false);
		_batchNode->addChild(asteroid);
		_asteroids->pushBack(asteroid);
	}


	//delete EnemyShipList;
	EnemyShipList = new Vector<Sprite*>(3);																// List of enemy ships
	//_nextShipLaser = 0;
	_shipLasers = new Vector<Sprite*>(KNUMLASERS);														// List of lasers
	for (int i = 0; i < KNUMLASERS; ++i) {
		cocos2d::Sprite* shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");			// Laser sprite, JOR replaced auto specifier
		shipLaser->setVisible(false);
		_batchNode->addChild(shipLaser);
		_shipLasers->pushBack(shipLaser);
	}

	touchListener = EventListenerTouchAllAtOnce::create();												// JOR replaced auto specifier

	// Time
	currentTime = 0.0f;																					// Current game time, for timer, changed to float to solve Android timer issue
	curTime = getTimeTick();																			// Current game time																						// Time to finish game
	_gameOverTime = curTime + LEVEL_TIME;																// Time to finish game

	// HUD
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


	// D-pad (Display on mobile device)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (visibleSize.height == 1080)
			controller = DPad::create("DPad/Base300.png", "DPad/Arrow96.png", "DPad/Arrow96Pressed.png", Point(250, 250));
		else
			controller = DPad::create("DPad/Base150.png", "DPad/Arrow.png", "DPad/ArrowPressed.png", Point(150, 150));
	}

	/*
	this->addChild(menu, 1);
	
	//  GALAXY
	this->addChild(_batchNode);
	
	// 1) Create the ParallaxNode
	_backgroundNode = ParallaxNodeExtras::create();
	this->addChild(_backgroundNode, -1);
	_backgroundNode->init();																								// ParallaxNodeExtras.cpp: Initialise the parallax scrolling background
	
	Level::addChild(ParticleSystemQuad::create("Stars1.plist"));
	Level::addChild(ParticleSystemQuad::create("Stars2.plist"));
	Level::addChild(ParticleSystemQuad::create("Stars3.plist"));
	
	// EnemyShip
	for (int i = 0; i < 3; ++i) {
		//EnemyShip = Sprite::create("EnemyShip.png");
		//EnemyShip->setPosition(visibleSize.width, visibleSize.height / 2);
		//this->addChild(EnemyShip);
		cocos2d::Sprite* enemyShip1 = Sprite::create("EnemyShip.png");														// Asteroid sprite, JOR replaced auto specifier
		enemyShip1->setVisible(false);
		this->addChild(enemyShip1);
		EnemyShipList->pushBack(enemyShip1);
	}

	for (int i = 0; i < KNUMLASERS; ++i) {
		cocos2d::Sprite* shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");								// Laser sprite, JOR replaced auto specifier
		shipLaser->setVisible(false);
		_batchNode->addChild(shipLaser);
		_shipLasers->pushBack(shipLaser);
	}
	
	touchListener->onTouchesBegan = CC_CALLBACK_2(Level::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// Number of lives
	for (unsigned int i = 0; i < MAX_LIVES; i++) {
		playerLife = Sprite::create("PlayerLife.png");
		playerLife->setPosition(visibleSize.width * 0.05 + (i * 52), visibleSize.height * 0.05);
		this->addChild(playerLife);
		livesList[i] = playerLife;																							// Add life sprite to list of lives
	}
	livesList[3]->setVisible(false);
	livesList[4]->setVisible(false);

	
	Game::Instance()->init();																								// Inite score and level
	HUD::Instance()->init(time, this);																						// Display score, level number, and time

	//time = LEVEL_TIME / 1000;
	Game::Instance()->setTimer(LEVEL_TIME/1000);

	Input::Instance()->init(this, this->_eventDispatcher);																	// Ship Movement


	this->addChild(scoreLabel, 10000);
	this->addChild(timeLabel);
	
	
	// D-pad (Display on mobile device)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (visibleSize.height == 1080)
			controller = DPad::create("DPad/Base300.png", "DPad/Arrow96.png", "DPad/Arrow96Pressed.png", Point(250, 250));
		else
			controller = DPad::create("DPad/Base150.png", "DPad/Arrow.png", "DPad/ArrowPressed.png", Point(150, 150));

		this->addChild(controller);
	}
	
	this->scheduleUpdate();
	*/

    return true;
}

float Level::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
}

void Level::update(float dt) {
	curTimeMillis = getTimeTick();																			// Current game time

	winSize = Director::getInstance()->getWinSize();														// Dimensions of game screen

	getInput();																								// Get keyboard input for Windows, Get DPad input for Android

	scoreLabel->setString("Score: " + to_string(Game::Instance()->getScore()));								// Update the displayed score
	
	updateTimer(curTimeMillis);																				// Update the countdown timer, pass in curTimeMillies solves Android Timer issue
	
	_backgroundNode->update(dt);																			// Scroll the background objects
	
	player->update();																						// Update player sprite position
}

void Level::getInput() {
	// Android DPad (maybe change to returning a point (0,0)(1,0)(0,1),(-1,0),(0,-1)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (controller->getButton(8)->isSelected()) {
			player->moveUp();
			//CCLOG("Down button is pressed!");
		}
		else if (controller->getButton(2)->isSelected()) {
			player->moveDown();
			//CCLOG("Down button is pressed!");
		}
		if (controller->getButton(4)->isSelected()) {
			player->moveLeft();
			//CCLOG("Down button is pressed!");
		}
		else if (controller->getButton(6)->isSelected()) {
			player->moveRight();
			//CCLOG("Down button is pressed!");
		}
	}
}

void Level::updateTimer(float dt) {
	if (dt > currentTime) {
		currentTime = dt + 1000.0f;
		time--;
	}

	timeLabel->setString("Time: " + to_string(time));
}

float Level::randomValueBetween(float low, float high) {
	// from http://stackoverflow.com/questions/686353/c-random-float-number-generation
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

void Level::setInvisible(Node * node) {
	node->setVisible(false);
}


void Level::checkCollisions() {
	// Asteroids Collisions
	for (cocos2d::Sprite* asteroid : *_asteroids) {															// JOR replaced auto specifier
		if (!(asteroid->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *_shipLasers) {													// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {
				Audio::Instance()->explodeFX();
				shipLaser->setVisible(false);
				asteroid->setVisible(false);
				//score += 10;																				// Award 10 points for destroying an asteroid
				Game::Instance()->updateScore(10);
			}
		}

		// Check collisions between the player ship and asteroids
		if (player->getSprite()->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {				// If the ship collides with an asteroid
			asteroid->setVisible(false);																	// Destroy the asteroid
			player->getSprite()->runAction(Blink::create(1.0F, 9));											// Flash the Player ship
																											//_lives--;																						// Decrement the number of lives
			Game::Instance()->takeLife();																	// Decrement the number of lives
			livesList[Game::Instance()->getLives()]->setVisible(false);										// Set the player lives invisible (in the order 2,1,0)
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
				//score += 20;																				// Award 20 points for destroying an enemy ship
				Game::Instance()->updateScore(20);
			}
		}
	}
}

/*
void Level::update(float dt) {
	
	//HUD::Instance()->update();																			// Update the score (Not working)


	// Update timer this class

	// Update timer from Game class
	Game::Instance()->updateTimer(curTimeMillis);															// Update the countdown timer, pass in curTimeMillies solves Android Timer issue
	timeLabel->setString("Time: " + to_string(Game::Instance()->getTimer()));

	//time -= ((int) dt % 10);
	//time += dt;
	//this->schedule(schedule_selector(Level1::updateTimer), 1.0f);											// Call the function every 1 second
	//if (curTimeMillis > timerTime) {
	//	timerTime = curTimeMillis + 1000;
	//	time--;
	//}

	//timeLabel->setString("Time: " + to_string(time));

	spawnAsteroids(curTimeMillis);																			// Spawn asteroids
	spawnEnemyShips(curTimeMillis);																			// Spawn asteroids
	checkCollisions();																						// Check have game objects collided with each other
	checkGameOver(curTimeMillis);																			// Check is the game over or not


		// Update displayed lives
	//if (_lives < 3 && !_gameOver) {																		// If the players lives are less than 3
	if (Game::Instance()->getLives() < MAX_LIVES && !_gameOver) {											// If the players lives are less than the max num lives
		livesList[Game::Instance()->getLives()]->setVisible(false);											// Set the lives invisible (2,1,0)
	}

	// Update the enemy ship position
	//EnemyShip->setPosition(EnemyShip->getPosition().x - 2, EnemyShip->getPosition().y);
}



void Level::spawnAsteroids(float curTimeMillis) {
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
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
				NULL)	 // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
		);
	}
}

void Level::spawnEnemyShips(float curTimeMillis) {
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
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
				NULL)	// TERMINATE WITH NULL
		);
	}
}


void Level::checkGameOver(float currenTime) {
	//if (_lives <= 0) {																					// If the player has run out of lives
	if (Game::Instance()->getLives() <= 0) {																// If the player has run out of lives
		player->getSprite()->stopAllActions();																// CCNode.cpp
		player->getSprite()->setVisible(false);																// Destroy the ship
		this->endScene(KENDREASONLOSE);																		// Player has died
	}
	else if (currenTime >= _gameOverTime) {
		this->endScene(KENDREASONWIN);																		// Player stays playing for the length of time
	}
}


void Level::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event){
	Audio::Instance()->laserFX();
	cocos2d::Size winSize = Director::getInstance()->getWinSize();									// JOR replaced auto specifier

	//spawnLaser();
	spawn2Lasers();
}

void Level::spawnLaser() {
	cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);														// Next laser in the list, JOR replaced auto specifier
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;																								// Reset laser list index to 0 (go back to start of list)
	
	shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width/2, 0));
	shipLaser->setVisible(true);
	shipLaser->stopAllActions();
	shipLaser->runAction(
		Sequence::create(
			MoveBy::create(0.5, Point(winSize.width, 0)),
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
			NULL));
}
void Level::spawn2Lasers() {
	cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);														// Next laser in the list, JOR replaced auto specifier
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;																								// Reset laser list index to 0 (go back to start of list)
	
	shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 12));
	shipLaser->setVisible(true);
	shipLaser->stopAllActions();
	shipLaser->runAction(
		Sequence::create(
			MoveBy::create(0.5, Point(winSize.width, 0)),
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
			NULL)); 

	cocos2d::Sprite* shipLaser2 = _shipLasers->at(_nextShipLaser++);													// Next laser in the list, JOR replaced auto specifier
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;																								// Reset laser list index to 0 (go back to start of list)

	shipLaser2->setPosition(player->getSprite()->getPosition() + Point(shipLaser2->getContentSize().width / 2, -12));
	shipLaser2->setVisible(true);
	shipLaser2->stopAllActions();
	shipLaser2->runAction(
		Sequence::create(
			MoveBy::create(0.5, Point(winSize.width, 0)),
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
			NULL));
}

void Level::restartTapped(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, this->createScene()));							// Restart the current scene	
	this->scheduleUpdate();																									// reschedule
}


void Level::returnToMenu(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5, MainMenu::createScene()));						// Return to the main menu
}

void Level::endScene(EndReason1 endReason) {
	if (_gameOver) return;																									// If already game over, skip this function
	_gameOver = true;																										// Set game over

	cocos2d::Size winSize = Director::getInstance()->getWinSize();															// JOR replaced auto specifier
	
	Game::Instance()->checkHighScore();																						// The game has ended, check if the current score is the high score and save it if it is

	// Win / Lose Message
	char message[17] = "Level 1 Complete";
	if (endReason == KENDREASONLOSE) strcpy(message, "You Lose");

	cocos2d::Label* label = Label::createWithBMFont("Arial.fnt", message);													// JOR replaced auto specifier

	if (winSize.height == 720)
		label->setScale(0.2f);
	else
		label->setScale(0.3F);																								// JOR Use larger label for 1080p

	label->setPosition(winSize.width / 2, winSize.height*0.75f);
	this->addChild(label);
	
	// Restart Level
	strcpy(message, "Restart Game");
	cocos2d::Label* restartLbl = Label::createWithBMFont("Arial.fnt", message);												// JOR replaced auto specifier
	cocos2d::MenuItemLabel* restartItem=MenuItemLabel::create(restartLbl,CC_CALLBACK_1(Level::restartTapped,this));		// JOR replaced auto specifier
	//restartItem->setScale(0.1F);
	if (winSize.height == 720)
		restartItem->setScale(0.1F);
	else
		restartItem->setScale(0.15F);																						// JOR Use larger size for 1080p
	restartItem->setPosition(winSize.width / 2, winSize.height*0.5f);

	cocos2d::Menu* menu = Menu::create(restartItem, NULL);																	// JOR replaced auto specifier
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Continue To Next Level
	if (endReason != KENDREASONLOSE) {
		strcpy(message, "Continue");
		cocos2d::Label* continueLbl = Label::createWithBMFont("Arial.fnt", message);										// JOR replaced auto specifier
		cocos2d::MenuItemLabel* continueItem = MenuItemLabel::create(continueLbl,CC_CALLBACK_1(Level::startLevel2,this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
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
*/

void Level::startLevel2(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, Level2::createScene()));							// Change scene, progressing to Level 2
}

void Level::startLevel3(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, Level3::createScene()));							// Load level 3	
}

void Level::returnToMenu(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5, MainMenu::createScene()));						// Return to the main menu
}

void Level::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

