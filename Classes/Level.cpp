/*
	Level.cpp

	Joe O'Regan
	K00203642
	21/02/2018

	Level base class
*/
#include "Level.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "MainMenu.h"
#include "DPad.h"
#include <string>
#include <sstream>
#include "Input.h"

#include "HealthBar.h"

unsigned int NUM_LASERS_TO_FIRE = 2;

unsigned int LEVEL_START_TIME = 30000;										// 30 Seconds until level is complete

// Because cocos2d-x requres createScene to be static, we need to make other non-pointer members static
std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> Input::keys;

Level* Level::layerInstance;																				// Singleton for Level

Scene* Level::createScene() {
	cocos2d::Scene* scene = Scene::create();																// 'scene' is an autorelease object, JOR replaced auto specifier   
	layerInstance = Level::create();																		// 'layer' is an autorelease object, JOR replaced auto specifier   
    scene->addChild(layerInstance);																			// Add layer as a child to scene	    
    return scene;																							// Return the scene
}

// on "init" you need to initialize your instance
bool Level::init() {
    if ( !Layer::init() ) { return false; }																	// super init first

	Game::Instance()->setGameOver(false);																	// Needed for starting new level, or restarting game

	// Screen size and position
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	(visibleSize.height == 720) ? scaleUp = 1.0f : scaleUp = 1.5f;
	(visibleSize.height == 1080) ? scaleDown = 1.0f : scaleDown = 0.67f;

	//  GALAXY
	_batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");												// Player sprite is added here
	this->addChild(_batchNode);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");

	// Player sprite
	player = new Player(this);
	_batchNode->addChild(player->getSprite(), 1);

	// 1) Create the ParallaxNode
	_backgroundNode = ParallaxNodeExtras::create();
	//this->addChild(_backgroundNode, -1);
	//_backgroundNode->init();																				// ParallaxNodeExtras.cpp: Initialise the parallax scrolling background

	Level::addChild(ParticleSystemQuad::create("Stars1.plist"));
	Level::addChild(ParticleSystemQuad::create("Stars2.plist"));
	Level::addChild(ParticleSystemQuad::create("Stars3.plist"));

	// Time
	curTime = getTimeTick();																				// Current game time // Time to finish game

	//powerUpLife = Sprite::createWithSpriteFrameName("powerHeart.png");
	powerUpTime = curTime + randomValueBetween(10000, LEVEL_START_TIME);
	powerUpY = randomValueBetween(0.1f, 0.8f);																// Random Y position for asteroid
	powerUpLife = Sprite::Sprite::create("powerHeart.png");
	//powerUpLife->autorelease();
	powerUpLife->setVisible(false);
	powerUpLife->setPosition(visibleSize.width + powerUpLife->getContentSize().width, visibleSize.height * powerUpY);
	//_batchNode->addChild(powerUpLife);
	this->addChild(powerUpLife);
	

	// Asteroids
	_asteroids = new Vector<Sprite*>(KNUMASTEROIDS);														// List of asteroids
	for (int i = 0; i < KNUMASTEROIDS; ++i) {
		cocos2d::Sprite* asteroid = Sprite::createWithSpriteFrameName("asteroid.png");						// Asteroid sprite, JOR replaced auto specifier
		asteroid->setVisible(false);
		asteroid->setScale((visibleSize.height == 720) ? 1.0f : 1.5f);										// Increase scale of Asteroid for Android (My phone anyway)
		_batchNode->addChild(asteroid);
		_asteroids->pushBack(asteroid);
	}
	/*
	// Enemy Ship
	EnemyShipList = new Vector<Sprite*>(3);																	// List of enemy ships
	for (int i = 0; i < 3; ++i) {
		cocos2d::Sprite* enemyShip = Sprite::create("EnemyShip.png");										// Asteroid sprite, JOR replaced auto specifier
		enemyShip->setVisible(false);
		enemyShip->setScale((visibleSize.height == 720) ? 0.67f : 1.0f);									// Scale down the size for PC
		this->addChild(enemyShip);
		EnemyShipList->pushBack(enemyShip);
	}
	*/
	EnemyShips = new Vector<EnemyShip*>(3);
	for (int i = 0; i < 3; ++i) {
		//EnemyShip *enemyShip = new EnemyShip(this);
		//cocos2d::Sprite* enemyShip1 = Sprite::create("EnemyShip.png");											// Asteroid sprite, JOR replaced auto specifier
		//EnemyShip* enemyShip1 = new EnemyShip();
		EnemyShip* enemyShip1 = EnemyShip::create(visibleSize);
		enemyShip1->autorelease();

		//Sprite* blah = Sprite::create("EnemyShip.png");													// Asteroid sprite, JOR replaced auto specifier

		//enemyShip1->setSprite(&(*blah));
		enemyShip1->setVisible(false);
		//cocos2d::Sprite* enemySprite = Sprite::create("EnemyShip.png");									// Asteroid sprite, JOR replaced auto specifier
		//enemyShip1->addChild(enemySprite);
		enemyShip1->setScale((visibleSize.height == 720) ? 0.67f : 1.0f);									// Scale down the size for PC
		/*
		cocos2d::DrawNode* healthBar = createStatusBar(
			enemyShip1->getPosition().x + enemyShip1->getContentSize().width, enemyShip1->getPosition().y + enemyShip1->getContentSize().height,		// Position
			(visibleSize.height == 720) ? 80 : 120, (visibleSize.height == 720) ? 10 : 15,					// Dimensions
			float (enemyShip1->getLives() / MAX_SHIP_LIVES),			// percentage
			red, trans);		
		// Colours
		enemyShip1->addChild(healthBar);
		*/
		this->addChild(enemyShip1);
		EnemyShips->pushBack(enemyShip1);
	}
	/*
	EnemyShips = new Vector<EnemyShip*>(3);
	for (int i = 0; i < 3; ++i) {
		//EnemyShip *enemyShip = new EnemyShip(this);
		//cocos2d::Sprite* enemyShip1 = Sprite::create("EnemyShip.png");											// Asteroid sprite, JOR replaced auto specifier
		EnemyShip* enemyShip1 = new EnemyShip();
		Sprite* blah = Sprite::create("EnemyShip.png");											// Asteroid sprite, JOR replaced auto specifier
		enemyShip1->setSprite(&(*blah));
		enemyShip1->getSprite()->setVisible(false);
		enemyShip1->getSprite()->setScale((visibleSize.height == 720) ? 0.67f : 1.0f);										// Scale down the size for PC

		cocos2d::DrawNode* healthBar = createStatusBar(
			enemyShip1->getSprite()->getPosition().x + enemyShip1->getSprite()->getContentSize().height, enemyShip1->getSprite()->getPosition().y,		// Position
			(visibleSize.height == 720) ? 100 : 150, (visibleSize.height == 720) ? 10 : 15,						// Dimensions
			float(enemyShip1->getLives() / MAX_SHIP_LIVES),			// percentage
			red, trans);
		// Colours
		enemyShip1->getSprite()->addChild(healthBar);

		this->addChild(enemyShip1->getSprite());
		EnemyShips->pushBack(enemyShip1);
	}
	*/
	// Ship Lasers:
	_shipLasers = new Vector<Sprite*>(KNUMLASERS);															// List of lasers
	for (int i = 0; i < KNUMLASERS; ++i) {
		cocos2d::Sprite* shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");				// Laser sprite, JOR replaced auto specifier
		shipLaser->setVisible(false);
		_batchNode->addChild(shipLaser);
		_shipLasers->pushBack(shipLaser);
	}

	// Enemy Lasers
	enemyLasers = new Vector<Sprite*>(KNUMLASERS);															// List of lasers
	for (int i = 0; i < KNUMLASERS; ++i) {
		cocos2d::Sprite* enemyLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");				// Laser sprite, JOR replaced auto specifier
		enemyLaser->setVisible(false);
		_batchNode->addChild(enemyLaser);
		enemyLasers->pushBack(enemyLaser);
	}

	// Touch screen / Mouse press
	touchListener = EventListenerTouchAllAtOnce::create();													// JOR replaced auto specifier
	touchListener->onTouchesBegan = CC_CALLBACK_2(Level::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


																											// Set the starting lives based on the difficulty
	if (Game::Instance()->getDifficulty() == EASY) {
		LEVEL_START_TIME = 25000;																			// Set the level length: 25 easy, 30 medium, 40 hard
		NUM_LASERS_TO_FIRE = 3;																				// Start with 3 lasers easy, 2 medium, 1 hard
	}
	else if (Game::Instance()->getDifficulty() == HARD) {
		LEVEL_START_TIME = 40000;																			// Set the level length	
		NUM_LASERS_TO_FIRE = 1;
	}

	_gameOverTime = curTime + LEVEL_START_TIME;																// Time to finish game

	Game::Instance()->init();																				// Inite score and level	

	
	Game::Instance()->setTimer(LEVEL_START_TIME / 1000);													// Set the countdown timer time

	Input::Instance()->init(this, this->_eventDispatcher);													// Ship Movement

	// D-pad (Display on mobile device)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {				// If the target platform is a mobile device
	/*
		if (visibleSize.height == 1080)
			controller = DPad::create("DPad/Base300.png", "DPad/Arrow96.png", "DPad/Arrow96Pressed.png", Point(250, 250));
		else
			controller = DPad::create("DPad/Base150.png", "DPad/Arrow.png", "DPad/ArrowPressed.png", Point(150, 150));
		*/
		controller = DPad::create("Base300.png", "Arrow96.png", "Arrow96Pressed.png", 
			(visibleSize.height == 1080) ? Point(375, 375) : Point(250, 250));
		controller->setScale((visibleSize.height == 1080) ? 1.0f : 0.67f);

		this->addChild(controller);
	}

	newHUD = HUD::create(origin, visibleSize);																// Create the HUD at the origin point (0,0), and passing in the screen resolution
	this->addChild(newHUD);

	// Music Player

	if (Game::Instance()->musicPlayerVisible()) {
		mplayer = MusicPlayer::create(Point((visibleSize.width * 1.33) / 2, visibleSize.height * 0.1f));	// Position: scale in MusicPlayer class throws off measurement (undo first)
		//mplayer->setScale((visibleSize.height == 1080) ? 1.5f : 1.0f);
		this->addChild(mplayer);
	}

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

	winSize = Director::getInstance()->getWinSize();														// Dimensions of game screen (Needs to update here so lasers fire etc.)

	getInput();																								// Get keyboard input for Windows, Get DPad input for Android

	Game::Instance()->updateTimer(curTimeMillis);															// Update the countdown timer in Game class, pass in curTimeMillies solves Android Timer issue

	_backgroundNode->update(dt);																			// Scroll the background objects

	spawnAsteroids(curTimeMillis);																			// Spawn asteroids
	spawnEnemyShips(curTimeMillis);																			// Spawn asteroids
	checkCollisions();																						// Check have game objects collided with each other
	checkGameOver(curTimeMillis);																			// Check is the game over or not
	
	enemyFireLaser(curTimeMillis);

	player->update();																						// Update player sprite position

	for (EnemyShip* enemyShip : *EnemyShips) {													// 
		if (!(enemyShip->isVisible())) continue;
		enemyShip->update(dt);
	}



	if (Game::Instance()->musicPlayerVisible()) mplayer->update();											// Update the music player
	newHUD->update(curTimeMillis);																			// Update the HUD
}

void Level::enemyFireLaser(float curTimeMillis) {
	for (EnemyShip* enemyShip : *EnemyShips) {													// 
		if (!(enemyShip->isVisible())) continue;							// If the enemy ship is visible do this:
		
		//if (enemyShip->getPosition().x == visibleSize.width * 0.75f)
		if (curTimeMillis > enemyShip->getNextFire() && enemyShip->isVisible()) {
			spawnEnemyLaser(enemyShip->getPosition());
			enemyShip->setNextFire(curTimeMillis + ENEMY_FIRE_RATE);
		}

		/*
		for (cocos2d::Sprite* shipLaser : *_shipLasers) {												// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(enemyShip->getBoundingBox())) {
				Audio::Instance()->explodeFX();															// Play explosion effect
				shipLaser->setVisible(false);															// Hide the player laser
				Game::Instance()->updateScore(20);														// Award 20 points for destroying an enemy ship
				if (enemyShip->isVisible()) Game::Instance()->incrementEnemyShipKills();				// Increment the total number of enemy ships destroyed
																										//enemyShip->setVisible(false);															// Hide the asteroid
				enemyShip->setLives(enemyShip->getLives() - 1);		// DECREMENT LIVES
																	//enemyShip->updateBar( (enemyShip->getLives() * 100 )/ 3.0f);			
				if (enemyShip->getLives() < 1) enemyShip->setVisible(false);
			}
		}
		*/
	}
}

void Level::spawnAsteroids(float curTimeMillis) {	

	//powerUpTime = 0;	// test

	if (!spawned && curTimeMillis > powerUpTime) {
		powerUpLife->setVisible(true);
		//powerUpLife->setPosition(winSize.width + powerUpLife->getContentSize().width / 2, powerUpY);

		auto actionpowerUp = MoveTo::create(5, Point(0 - powerUpLife->getContentSize().width, visibleSize.height * powerUpY));
		powerUpLife->runAction(actionpowerUp);
		/*
		powerUpLife->runAction(
			Sequence::create(MoveBy::create(0.5f, Point(0 - powerUpLife->getContentSize().width, powerUpY)), // change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)
		);
		*/
		spawned = true;
	}
	
	if (curTimeMillis > _nextAsteroidSpawn) {
		float randMillisecs = randomValueBetween(0.20F, 1.0F) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;

		float randY = randomValueBetween(0.0F, winSize.height);												// Random Y position for asteroid
		float randDuration = randomValueBetween(2.0F, 10.0F);

		Sprite *asteroid = _asteroids->at(_nextAsteroid);
		_nextAsteroid++;																					// Increment the asteroid

		if (_nextAsteroid >= _asteroids->size()) _nextAsteroid = 0;											// Loop back around to start of asteroids list
		
		Game::Instance()->incrementAsteroidCount();

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

		//Sprite *enemyShip = EnemyShipList->at(nextEnemyShip);
		EnemyShip *enemyShip = EnemyShips->at(nextEnemyShip);
		nextEnemyShip++;																					// Increment the enemy ship on the list

		Game::Instance()->incrementEnemyShipCount();

		//if (nextEnemyShip >= EnemyShipList->size()) nextEnemyShip = 0;										// Loop back around to start of enemy ship list
		if (nextEnemyShip >= EnemyShips->size()) nextEnemyShip = 0;										// Loop back around to start of enemy ship list

		enemyShip->stopAllActions();																		// CCNode.cpp
		enemyShip->setPosition(winSize.width + enemyShip->getContentSize().width / 2, randY);
		enemyShip->setVisible(true);
		enemyShip->setLives(3);

		// Move the ship to the players coordinate
		auto action = MoveTo::create(3, Point(player->getSprite()->getPositionX(), player->getSprite()->getPositionY()));
		enemyShip->runAction(action);

		enemyShip->runAction(
			Sequence::create(
				MoveBy::create(randDuration, Point(-winSize.width - enemyShip->getContentSize().width, 0)),
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
				NULL)	// TERMINATE WITH NULL
		);
	}
}


void Level::spawnEnemyLaser(cocos2d::Point pos) {
	cocos2d::Sprite* enemyLaser = enemyLasers->at(nextEnemyLaser++);
	if (nextEnemyLaser >= enemyLasers->size())
		nextEnemyLaser = 0;

	if (enemyLaser->isVisible()) return;

	enemyLaser->setPosition(pos.x, pos.y);
	enemyLaser->setVisible(true);
	enemyLaser->stopAllActions();
	
	// Move the ship to the players coordinate
	//auto action = MoveTo::create(3, Point(player->getSprite()->getPositionX(), player->getSprite()->getPositionY()));
	auto action = MoveTo::create(0.5f, Point(0 - enemyLaser->getContentSize().width, pos.y));			// set to off screen the width of the laser
	enemyLaser->runAction(action);
}

// 20180221 Up to 4 different types of laser, with different spawn points and rotations
void Level::spawnLasers(int amount) {	
	int yVal = 0, yPos = 0, rot = 0;	// y value for point to aim for, y position of laser spawn point, rotation of laser	

	for (int i = 0; i < amount; i++) {
		cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);										// Next laser in the list, JOR replaced auto specifier
		if (_nextShipLaser >= _shipLasers->size())
			_nextShipLaser = 0;																				// Reset laser list index to 0 (go back to start of list)

		// Set laser spawn points
		if (amount < 4)
			(i == 0) ? yPos = 0 : (i == 1) ? yPos = -12 : yPos = 12;										// 0 = midd
		else
			(i == 0) ? yPos = 8 : (i == 1) ? yPos = -12 : (i == 2) ? yPos = 12 : yPos = -8;					// 0. = 5, 1. = 12, 2. = 12, 3. = -5

		shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, yPos));
		if (amount == 2) (i == 0) ? rot = -5 : rot = 5;														// Top, bottom lasers
		if (amount == 3) (i == 0) ? rot = 0 : (i == 1) ? rot = 5 : rot = -5;								// Middle, bottom, top lasers
		if (amount == 4) (i == 0) ? rot = -3 : (i == 1) ? rot = 5 : (i == 2) ? rot = -5 : rot = 3;			// laser 1: i = 2 (5), laser 2: i = 0 (3), laser 3: i = ,laser 4: i = 1 (-5)
		shipLaser->setRotation(rot);

		shipLaser->setVisible(true);
		shipLaser->stopAllActions();

		if (amount == 2) (i == 0) ? yVal = 60 : yVal = -60;													// if 2 lasers, first one goes up, second goes down
		if (amount == 3) (i == 1) ? yVal = -100 : (i == 2) ? yVal = 100 : yVal = 0;							// if 3 lasers, first goes straight, second goes down, third goes up
		if (amount == 4) (i == 1) ? yVal = -120 : (i == 2) ? yVal = 120 : (i == 3) ? yVal = -40 : yVal = 40;	// if 3 lasers, first goes straight, second goes down, third goes up

		shipLaser->runAction(
			Sequence::create(MoveBy::create(0.5, Point(winSize.width, yVal)), // change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
	}
}

void Level::getInput() {
	// Android DPad (maybe change to returning a point (0,0)(1,0)(0,1),(-1,0),(0,-1)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {			// If the platform is mobile
		if (controller->getButton(8)->isSelected()) {													// Up arrow pressed
			player->moveUp();
			//CCLOG("Down button is pressed!");
		}
		else if (controller->getButton(2)->isSelected()) {												// Down arrow pressed
			player->moveDown();
			//CCLOG("Down button is pressed!");
		}
		if (controller->getButton(4)->isSelected()) {													// Left arrow pressed
			player->moveLeft();
			//CCLOG("Down button is pressed!");
		}
		else if (controller->getButton(6)->isSelected()) {												// Right arrow pressed
			player->moveRight();
			//CCLOG("Down button is pressed!");
		}
	}
}

/*
	Generate a random number from a range
*/
float Level::randomValueBetween(float low, float high) {
	// from http://stackoverflow.com/questions/686353/c-random-float-number-generation
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

void Level::setInvisible(Node * node) {
	node->setVisible(false);																			// Set the node invisible
}

void Level::checkCollisions() {
	// Enemy laser move off screen
	for (Sprite* enemyLaser : *enemyLasers) {
		if (!(enemyLaser->isVisible())) continue;

		if (enemyLaser->getPosition().x == -enemyLaser->getContentSize().width)				// If the laser moves off screen it's own width
			enemyLaser->setVisible(false);													// Hide the laser
	}

	if (powerUpLife->isVisible()) {
		if (player->getSprite()->getBoundingBox().intersectsRect(powerUpLife->getBoundingBox())) {		// If the ship collides with an asteroid
			player->getSprite()->runAction(Blink::create(1.0F, 9));										// Flash the Player ship
			powerUpLife->setVisible(false);
			Game::Instance()->addLife();
			Audio::Instance()->powerUpFX();																// Play the power up sound effect
		}
	}

	// Asteroids Collisions
	for (cocos2d::Sprite* asteroid : *_asteroids) {														// JOR replaced auto specifier
		if (!(asteroid->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *_shipLasers) {												// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;													// If the ship is not visible

			if (shipLaser->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {
				Audio::Instance()->explodeFX();															// Play the explosion effect
				shipLaser->setVisible(false);															// Hide the laser
				Game::Instance()->updateScore(10);														// Award 10 points for destroying an asteroid
				if (asteroid->isVisible()) Game::Instance()->incrementAsteroidKills();					// Increment the number of asteroids destroyed
				asteroid->setVisible(false);															// Hide the asteroid
			}
		}

		// Check collisions between the player ship and asteroids
		if (player->getSprite()->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {			// If the ship collides with an asteroid
			asteroid->setVisible(false);																// Destroy the asteroid
			player->getSprite()->runAction(Blink::create(1.0F, 9));										// Flash the Player ship
			Game::Instance()->takeLife();																// Decrement the number of lives
			//livesList[Game::Instance()->getLives()]->setVisible(false);								// Set the player lives invisible (in the order 2,1,0)
			newHUD->updateLives();																		// Updates the displayed lives when the player collides with an object
		}
	}
	
	// Enemy ship collisions
	//for (cocos2d::Sprite* enemyShip : *EnemyShipList) {												// JOR replaced auto specifier
	//for (cocos2d::Sprite* enemyShip : *EnemyShips) {													//
	for (EnemyShip* enemyShip : *EnemyShips) {													// 
		if (!(enemyShip->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *_shipLasers) {												// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(enemyShip->getBoundingBox())) {
				Audio::Instance()->explodeFX();															// Play explosion effect
				shipLaser->setVisible(false);															// Hide the player laser
				Game::Instance()->updateScore(20);														// Award 20 points for destroying an enemy ship
				if (enemyShip->isVisible()) Game::Instance()->incrementEnemyShipKills();				// Increment the total number of enemy ships destroyed
				//enemyShip->setVisible(false);															// Hide the asteroid
				enemyShip->setLives(enemyShip->getLives() - 1);		// DECREMENT LIVES
				//enemyShip->updateBar( (enemyShip->getLives() * 100 )/ 3.0f);			
				if (enemyShip->getLives() < 1) enemyShip->setVisible(false);
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
*/

void Level::checkGameOver(float currenTime) {															// If the player has run out of lives
	if (Game::Instance()->getLives() <= 0) {															// If the player has run out of lives
		player->getSprite()->stopAllActions();															// CCNode.cpp
		player->getSprite()->setVisible(false);															// Destroy the ship
		endScene(KENDREASONLOSE);																		// Player has died
	}
	else if (currenTime >= _gameOverTime) {
		endScene(KENDREASONWIN);																		// Player stays playing for the length of time
	}
}

void Level::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event){
	if (Game::Instance()->isGameOver()) return;													// If the ship is not visible

	Audio::Instance()->laserFX();
	spawnLasers(NUM_LASERS_TO_FIRE);
}

void Level::endScene(EndReason endReason) {
	if (Game::Instance()->isGameOver()) return;															// If already game over, skip this function
	Game::Instance()->setGameOver(true);																// Set game over
	
	(visibleSize.height == 720) ? scaleUp = 1.0f : scaleUp = 0.67f;

	const int TOTAL_LIST_ELEMENTS = 9;

	Game::Instance()->checkHighScore();																	// The game has ended, check if the current score is the high score and save it if it is

	// Win / Lose Message
	//char message[17] = "Level 1 Complete";
	std::string message = "Level " + to_string(Game::Instance()->getLevel()) + " Complete";
	//if (endReason == KENDREASONLOSE) strcpy(message, "You Lose");
	if (endReason == KENDREASONLOSE) message = "You Lose";

	// 1. Level Over Message
	//levelCompleteLbl = Label::createWithBMFont("Arial.fnt", message);									// JOR replaced auto specifier
	levelCompleteLbl = cocos2d::Label::createWithTTF(message, "fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);
	levelCompleteLbl->setPosition(winSize.width / 2, winSize.height - (winSize.height / TOTAL_LIST_ELEMENTS * 2));
	this->addChild(levelCompleteLbl);
	
	// 2. Total Asteroids Destroyed
	cocos2d::Label* asteroidsLbl = cocos2d::Label::createWithTTF("Total Asteroids Destroyed: " + to_string(Game::Instance()->getAsteroidKills()), "fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);							// JOR replaced auto specifier
	asteroidsLbl->setPosition(Point(visibleSize.width * 0.5f + origin.x, visibleSize.height - (visibleSize.height / TOTAL_LIST_ELEMENTS * 3)));
	asteroidsLbl->setColor(Color3B::RED);
	//asteroidsLbl->setScale((visibleSize.height == 1080) ? 1.5f : 1.0f);
	this->addChild(asteroidsLbl);





	/*
	if (Game::Instance()->getAsteroidCount() > 0) {
		cocos2d::DrawNode* healthBar = createStatusBar(
			winSize.width * 0.5f, winSize.height - ((winSize.height / TOTAL_LIST_ELEMENTS) * 3.5f),					// Position
			(visibleSize.height == 720) ? 200 : 300, (visibleSize.height == 720) ? 20 : 30,							// Dimensions
			(float)Game::Instance()->getAsteroidKills() / (float)Game::Instance()->getAsteroidCount(),			// percentage
			red, trans);																							// Colours
		this->addChild(healthBar);
	}


	if (Game::Instance()->getEnemyShipCount() > 0) {
		cocos2d::DrawNode* healthBar = createStatusBar(
			winSize.width * 0.5f, winSize.height - ((winSize.height / TOTAL_LIST_ELEMENTS) * 4.5f),					// Position
			(visibleSize.height == 720) ? 200 : 300, (visibleSize.height == 720) ? 20 : 30,							// Dimensions
			(float)Game::Instance()->getEnemyShipKills() / (float)Game::Instance()->getEnemyShipCount(),			// percentage
			red, trans);																							// Colours
		this->addChild(healthBar);
	}
	*/


	// 3. Total Enemy Ships Destroyed
	cocos2d::Label* enemyShipsLbl = cocos2d::Label::createWithTTF("Total Enemy Ships Destroyed: " + to_string(Game::Instance()->getEnemyShipKills()), "fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);							// JOR replaced auto specifier
	enemyShipsLbl->setPosition(Point(visibleSize.width * 0.5f + origin.x, visibleSize.height - (winSize.height / TOTAL_LIST_ELEMENTS * 4)));
	enemyShipsLbl->setColor(Color3B::RED);
	this->addChild(enemyShipsLbl);

	// 4. Restart Level
	message = "Restart Game";
	cocos2d::Label* restartLbl = Label::createWithBMFont("Arial.fnt", message);							// JOR replaced auto specifier
	restartItem = MenuItemLabel::create(restartLbl,CC_CALLBACK_1(Level::restartTapped,this));			// JOR replaced auto specifier
	restartItem->setPosition(winSize.width / 2, winSize.height - (winSize.height / TOTAL_LIST_ELEMENTS * 6));
	
	unsigned int level = Game::Instance()->getLevel();

	// 5. Continue To Next Level
	if (endReason != KENDREASONLOSE && level < 4)
		message = "Start Level " + to_string(Game::Instance()->getLevel()+1);
	if (level == 4 || endReason == KENDREASONLOSE)
		message = "Main Menu";

	cocos2d::Label* continueLbl = Label::createWithBMFont("Arial.fnt", message);						// JOR replaced auto specifier
	//continueLbl->setBMFontSize(10);																	// Test set font size
	if (endReason != KENDREASONLOSE) {
		if (level == 1) 
			continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::startLevel2, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		else if (level == 2) 
			continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::startLevel3, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		else if (level == 3) 
			continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::startLevel4, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		else if (level == 4) 
			continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::returnToMenu, this));// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	}
	else 
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::returnToMenu, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	continueItem->setPosition(winSize.width / 2, winSize.height - (winSize.height / TOTAL_LIST_ELEMENTS * 7));
	continueItem->runAction(ScaleTo::create(0.5F, (visibleSize.height == RES_720P) ? 1.0f : 1.5f));		// Scale the continue to next level label
	
	// clear label and menu
	restartItem->runAction(ScaleTo::create(0.5F, (visibleSize.height == RES_720P) ? 1.0f : 1.5f));		// Scale the restart button label
	levelCompleteLbl->runAction(ScaleTo::create(0.5F, (visibleSize.height == RES_720P) ? 1.0f : 1.5f));	// Scale the level complete message label

	cocos2d::Menu* menu = Menu::create(restartItem, continueItem, NULL);								// JOR replaced auto specifier
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	this->unscheduleUpdate();																			// Terminate update callback
}

// Callbacks
void Level::restartTapped(Ref* pSender) {
	Audio::Instance()->selectMenuOption();
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, Level1::createScene()));		// Restart the current scene	
	this->scheduleUpdate();																				// reschedule
}
void Level::startLevel2(Ref* pSender) {
	Audio::Instance()->selectMenuOption();
	Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, Level2::createScene()));		// Change scene, progressing to Level 2
}
void Level::startLevel3(Ref* pSender) {
	Audio::Instance()->selectMenuOption();
	Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, Level3::createScene()));		// Load level 3
}
void Level::startLevel4(Ref* pSender) {
	Audio::Instance()->selectMenuOption();
	Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, Level4::createScene()));		// Load level 4
}
void Level::returnToMenu(Ref* pSender) {
	Audio::Instance()->selectMenuOption();
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5, MainMenu::createScene()));	// Return to the main menu
}
void Level::menuCloseCallback(Ref* pSender) {
	Audio::Instance()->selectMenuOption();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

