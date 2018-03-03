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
#include "EnemyShipKling.h"
#include "EnemyShipWilKnot.h"
#include "AsteroidOriginal.h"
//#include "PowerUp.h"

// Because cocos2d-x requres createScene to be static, we need to make other non-pointer members static
std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> Input::keys;

Level* Level::s_pLayerInstance;																				// Singleton for Level

Scene* Level::createScene() {
	cocos2d::Scene* scene = Scene::create();																// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level::create();																		// 'layer' is an autorelease object, JOR replaced auto specifier   
    scene->addChild(s_pLayerInstance);																		// Add layer as a child to scene	    
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
	m_batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");												// Player sprite is added here
	this->addChild(m_batchNode);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");								// Add collection of SpaceGame sprites to be accessed
	
	player = Player::create(visibleSize);
	//_batchNode->addChild(player, 1);
	this->addChild(player);																					// Add the Player to the current layer

	// 1) Create the ParallaxNode
	m_backgroundNode = ParallaxNodeExtras::create();														// Create the parallax scrolling background

	Level::addChild(ParticleSystemQuad::create("Stars1.plist"));											// Add the star particles
	Level::addChild(ParticleSystemQuad::create("Stars2.plist"));
	Level::addChild(ParticleSystemQuad::create("Stars3.plist"));
		
	Game::Instance()->init();																				// Init score and level	
	Input::Instance()->init(this, this->_eventDispatcher);													// Ship Movement

	initLives();																							// Initialise the number of lives, set or carry over from previous level
	initPowerUps();																							// Initialise the power ups
	initAsteroids();
	m_enemyShipList = new Vector<EnemyShip*>(3);															// initEnemyShips now virtual function so Vector must be outside function
	initEnemyShips();
	initLasers();
	initDifficulty();																						// Initialise fire rate, weapons state, and power up duration on screen

	// Touch screen / Mouse press
	touchListener = EventListenerTouchAllAtOnce::create();													// JOR replaced auto specifier
	touchListener->onTouchesBegan = CC_CALLBACK_2(Level::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
		
	// D-pad (Display on mobile device)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {				// If the target platform is a mobile device
		controller = DPad::create(DPAD_BASE, DPAD_ARROW, DPAD_ARROW_ACTIVE,
			(visibleSize.height == 1080) ? Point(375, 375) : Point(250, 250));
		//controller->setScale((visibleSize.height == 1080) ? 1.0f : 0.67f);

		this->addChild(controller);
	}

	newHUD = HUD::create(origin, visibleSize);																// Create the HUD at the origin point (0,0), and passing in the screen resolution
	this->addChild(newHUD);

	// Music Player
	if (Game::Instance()->musicPlayerVisible()) {
		mplayer = MusicPlayer::create(Point((visibleSize.width / 2), visibleSize.height * 0.065f));			// Position: scale in MusicPlayer class throws off measurement (undo first)
		this->addChild(mplayer);
	}

	CCLOG("Level %d: Initialised", Game::Instance()->getLevel());

    return true;
}

/*
Choose the number of lives based on the game difficulty
*/
void Level::initLives() {
	if (!Game::Instance()->getLivesCarried()) {
		// Set the starting lives based on the difficulty
		if (Game::Instance()->getDifficulty() == EASY)
			Game::Instance()->setLives(5);						// Set the number of lives for the player
		else if (Game::Instance()->getDifficulty() == MEDIUM)
			Game::Instance()->setLives(3);						// Set the number of lives for the player
		else
			Game::Instance()->setLives(2);						// Set the number of lives for the player

		CCLOG("Level %d - Init Lives - Lives NOT Carried Over - Start With: %d", Game::Instance()->getLevel(), Game::Instance()->getLives());
	} else {
		CCLOG("Level %d - Init Lives - Lives ARE Carried Over: %d", Game::Instance()->getLevel(), Game::Instance()->getLives());
	}
}
/*
	Initialise fire rate, weapons state, and power up duration on screen
*/
void Level::initDifficulty() {
	// Set the starting lives based on the difficulty
	if (Game::Instance()->getDifficulty() == EASY) {
		m_fireRate = 200;																					// Set the players weapon fire rate to a shorter time interval
		m_powerUpDuration = 5.0f;																			// Easy Difficulty: Power up remains on screen longer time
	}
	else if (Game::Instance()->getDifficulty() == HARD) {
		m_fireRate = 400;																					// Set the players weapon fire rate to a longer time interval
		m_powerUpDuration = 2.0f;																			// Hard Difficulty: Power up remains on screen less time
	}
	CCLOG("Level %d: Player Laser Beams Initialised", Game::Instance()->getLevel());
}

void Level::initEnemyShips() {
	CCLOG("LEVEL initEnemyShips() - Level %d: Enemy Ships Initialised", Game::Instance()->getLevel());
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		EnemyShip* enemyShip1 = EnemyShip::create(visibleSize);
		//EnemyShipWilKnot* enemyShip1 = EnemyShipWilKnot::create(visibleSize);								// Test New Enemies
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
		//CCLOG("LEVEL - Add Enemy ship at array index %d", i);
	}
}

void Level::initAsteroids() {
	// Asteroids
	m_asteroidsList = new Vector<Asteroid*>(MAX_NUM_ASTEROIDS_L1);											// List of asteroids
	for (int i = 0; i < MAX_NUM_ASTEROIDS_L1; ++i) {
		//cocos2d::Sprite* asteroid = Sprite::createWithSpriteFrameName(ASTEROID_IMG);						// Asteroid sprite, JOR replaced auto specifier
		Asteroid* asteroid = AsteroidOriginal::create(visibleSize);						// Asteroid sprite, JOR replaced auto specifier
		//asteroid->setVisible(false);
		//asteroid->setScale((visibleSize.height == 720) ? 1.0f : 1.5f);										// Increase scale of Asteroid for Android (My phone anyway)
		m_batchNode->addChild(asteroid);
		m_asteroidsList->pushBack(asteroid);

		// Add base class asteroid
		Asteroid* asteroid2 = Asteroid::create(visibleSize);
		this->addChild(asteroid2);
		m_asteroidsList->pushBack(asteroid2);
	}
	CCLOG("Level %d: Asteroids Initialised", Game::Instance()->getLevel());
}

void Level::initLasers() {
	// Player Ship Lasers:
	m_playerLaserList = new Vector<Sprite*>(NUM_LASERS);													// List of lasers
	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* shipLaser = Sprite::createWithSpriteFrameName(LASER_BLUE_IMG);						// Laser sprite, JOR replaced auto specifier
		shipLaser->setVisible(false);
		m_batchNode->addChild(shipLaser);
		m_playerLaserList->pushBack(shipLaser);
	}
	//CCLOG("TEST Level::initLasers() Ship Lasers: %d", m_playerLaserList->size());

	// Enemy Lasers
	m_enemyLaserList1 = new Vector<Sprite*>(NUM_LASERS);														// List of lasers

	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* enemyLaser = Sprite::createWithSpriteFrameName(LASER_BLUE_IMG);					// Laser sprite, JOR replaced auto specifier
		enemyLaser->setVisible(false);
		m_batchNode->addChild(enemyLaser);
		m_enemyLaserList1->pushBack(enemyLaser);
	}
	//CCLOG("Level %d: Enemy Ships Initialised", Game::Instance()->getLevel());

	// eNEMY 2 LASERS
	m_enemyLaserList2 = new Vector<Sprite*>(NUM_LASERS);					// List of lasers
	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* enemyLaser = Sprite::create(LASER_ORANGE_IMG);		// Laser sprite, JOR replaced auto specifier
		enemyLaser->setVisible(false);
		this->addChild(enemyLaser);
		m_enemyLaserList2->pushBack(enemyLaser);
	}
}

/*
	Initialise the new life and weapon power ups
*/
void Level::initPowerUps() {
	m_powerUpLife = PowerUp::create(visibleSize, NEW_LIFE);
	this->addChild(m_powerUpLife);
	m_powerUpWeapon = PowerUp::create(visibleSize, WEAPON_UPGRADE);
	this->addChild(m_powerUpWeapon);
	CCLOG("Level %d: New Life Power Up Initialised", Game::Instance()->getLevel());
}

void Level::update(float dt) {
	curTimeMillis = Game::Instance()->getTimeTick();														// Current game time

	winSize = Director::getInstance()->getWinSize();														// Dimensions of game screen (Needs to update here so lasers fire etc.)

	getInput();																								// Get keyboard input for Windows, Get DPad input for Android

	Game::Instance()->updateTimer(curTimeMillis);															// Update the countdown timer in Game class, pass in curTimeMillies solves Android Timer issue

	m_backgroundNode->update(dt);																			// Scroll the background objects

	spawnObjects(curTimeMillis);																			// Spawn asteroids
	spawnEnemyShips(curTimeMillis);																			// Spawn asteroids
	
	checkCollisions();																						// Check have game objects collided with each other

	checkGameOver(curTimeMillis);																			// Check is the game over or not
	
	player->update();																						// Update player sprite position, taking input from the keyboard
	
	for (EnemyShip* enemyShip : *m_enemyShipList) {
		enemyShip->update(curTimeMillis);																	// Fire Enemy Weapons etc.
	}

	if (Game::Instance()->musicPlayerVisible()) mplayer->update();											// Update the music player

	newHUD->update(curTimeMillis);																			// Update the HUD
}

/*
	Spawn power ups and asteroids, no real difference only what happens on collision
*/
void Level::spawnObjects(float curTimeMillis) {
	// Rotate
	auto rotate = RotateBy::create(2.5f, -360.0f);
	auto repeat = RepeatForever::create(rotate);

	//powerUpTime = 2000;	// test
	if (!m_powerUpLife->isSpawned() && curTimeMillis > m_powerUpLife->getSpawnTime()) {
		m_powerUpLife->setVisible(true);
		auto actionpowerUp = MoveTo::create(m_powerUpDuration,
			Point(0 - m_powerUpLife->getContentSize().width, 
				visibleSize.height * m_powerUpLife->getRandY()));
		m_powerUpLife->runAction(actionpowerUp);

		m_powerUpLife->runAction(repeat);
		m_powerUpLife->setSpawned();
	}

	if (!m_powerUpWeapon->isSpawned() && curTimeMillis > m_powerUpWeapon->getSpawnTime()) {
		m_powerUpWeapon->setVisible(true);
		auto actionpowerUp2 = MoveTo::create(m_powerUpDuration,
			Point(0 - m_powerUpWeapon->getContentSize().width, 
				visibleSize.height * m_powerUpWeapon->getRandY()));
		m_powerUpWeapon->runAction(actionpowerUp2);
		m_powerUpWeapon->runAction(repeat);																	// Reuse the same sequence for weapon power ups
		
		/* powerUpLife->runAction(
			Sequence::create(MoveBy::create(0.5f, Point(0 - powerUpLife->getContentSize().width, powerUpY)),		// change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)
		); */
		CCLOG("Spawn Weapon Power Up");
		m_powerUpWeapon->setSpawned();
	}
	
	if (curTimeMillis > _nextAsteroidSpawn) {
		float randMillisecs = randomValueBetween(0.20F, 1.0F) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;

		Asteroid *asteroid = m_asteroidsList->at(m_nextAsteroid);
		m_nextAsteroid++;																					// Increment the asteroid list index
		if (m_nextAsteroid >= m_asteroidsList->size()) m_nextAsteroid = 0;									// Loop back around to start of asteroids list

		Game::Instance()->incrementAsteroidCount();
		
		asteroid->init(visibleSize);																		// Initialise the Asteroid, stopping all active actions, set visibility, and random positoin, scale, and rotate
		asteroid->runAction(Sequence::create(																// Sequence of actions where the asteroid moves off screen to the left
				MoveBy::create(asteroid->getDuration(), 
					Point(-winSize.width - asteroid->getContentSize().width, 0)),							// To a position fully off screen
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)							// And is then destroyed, DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
		);	

		//CCLOG("Spawn Asteroid");
	}
}

void Level::spawnEnemyShips(float curTimeMillis) {
	if (m_enemyLaserList1->size() > 0) {
		CCLOG("Spawn Enemy Ship - size > 0");
		if (curTimeMillis > nextEnemyShipSpawnTime) {
			float randMillisecs = randomValueBetween(0.20F, 1.0F) * 2500;
			nextEnemyShipSpawnTime = randMillisecs + curTimeMillis;											// Set the time to spawn the next ship			

			EnemyShip *enemyShip = m_enemyShipList->at(nextEnemyShip); 
			nextEnemyShip++;																				// Moved to use min and max speed
			if (nextEnemyShip >= (unsigned int)m_enemyShipList->size()) nextEnemyShip = 0;					// Loop back around to start of enemy ship list
										
			//if (enemyShip->isVisible()) return;

			Game::Instance()->incrementEnemyShipCount();													// Increment the enemy ship on the list
			
			enemyShip->init(visibleSize);																	// Stop actions, set position, set visible, and set lives

			CCLOG("Spawn Enemy Ship init enemy ship");
			// Apply Actions: Move the ship to the players coordinate
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
			CCLOG("Spawn Enemy Ship - END");
		}
	}
}

/*
	I replaced a number of enemy laser spawn functions by adding a type
	This will spawn blue, orange or green lasers depending on the enemy
	Using different lists as the lasers are recycled. Otherwise random
	lasers would be spawned during the game. This could be avoided by 
	creating a type for a laser object.
*/
void Level::spawnEnemyLaser(cocos2d::Point pos, int type) {
	cocos2d::Sprite* enemyLaser;
	Vector<Sprite*> *tempList;

	if (type == BLUE)
		tempList = m_enemyLaserList1;														// Specific list for each colour laser, as different sprites used
	else 
		if (type == ORANGE)
		tempList = m_enemyLaserList2;
	else if (type == GREEN1 || type == GREEN2 || type == GREEN3)
		tempList = m_enemyLaserList3;
	
	enemyLaser = tempList->at(m_nextEnemyLaser);											// Use the laser off the correct list
	
	m_nextEnemyLaser++;																		// Increment the lasers
	if (m_nextEnemyLaser >= tempList->size()) m_nextEnemyLaser = 0;							// Loop back to start of laser list

	if (enemyLaser->isVisible()) return;													// If the laser is already visible skip it
	Audio::Instance()->laserFXEnemy();														// Play audio

	enemyLaser->setRotation(0);
	enemyLaser->setPosition(pos.x, pos.y);													// Set the postion relevant to the ships coordinates
	enemyLaser->setVisible(true);															// Set visible on screen
	enemyLaser->stopAllActions();															// Stop actions for the laser

	cocos2d::MoveTo *action;																// Declare the action
	cocos2d::MoveBy *action2;
	
	if (type == GREEN2)
/*
		enemyLaser->runAction(
			Sequence::create(MoveBy::create(0.525f, Point(pos.x - visibleSize.width - enemyLaser->getContentSize().width -			// Laser 2 travels up
				getContentSize().width, pos.y + visibleSize.height * 0.33f)),								// change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
*/
		action = MoveTo::create(0.525f,														// This is where Euclidean distance would be handy
			Point(pos.x - visibleSize.width - enemyLaser->getContentSize().width -			// Laser 2 travels up
				getContentSize().width, pos.y + visibleSize.height * 0.33f));
	else if (type == GREEN3)
		/*
		enemyLaser->runAction(
		Sequence::create(MoveBy::create(0.525f, 
			Point(pos.x - visibleSize.width - enemyLaser->getContentSize().width -			// Laser 2 travels up
			getContentSize().width, pos.y - visibleSize.height * 0.33f)),								// change to plus 100 for up - 100 for down
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
		*/
		action = MoveTo::create(0.525f,
			Point(pos.x - visibleSize.width - enemyLaser->getContentSize().width -			// Laser 3 travels down
				getContentSize().width, pos.y - visibleSize.height * 0.33f));
	else
/*		enemyLaser->runAction(
			Sequence::create(MoveBy::create(0.5f, 
				Point(pos.x - visibleSize.width - enemyLaser->getContentSize().width -			// Laser 2 travels up
				getContentSize().width, pos.y)),								// change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
*/		action = MoveTo::create(0.5f,														// Laser 1 travels centrally
			Point(pos.x - visibleSize.width - enemyLaser->getContentSize().width - 
				getContentSize().width, pos.y));											// set to off screen the width of the laser + the screen width

	enemyLaser->runAction(action);															// Start moving the laser sprite
}

/* 
	20180221 Up to 4 different types of laser, with different spawn points and rotations
*/
void Level::spawnLasers(int amount) {
	if (curTimeMillis > m_nextFire) {
		Audio::Instance()->laserFX();																			// Play the laser fire effect

		int yVal = 0, yPos = 0, rot = 0;																		// y value for point to aim for, y position of laser spawn point, rotation of laser	

		for (int i = 0; i < amount; i++) {
			cocos2d::Sprite* shipLaser = m_playerLaserList->at(_nextShipLaser++);								// Next laser in the list, JOR replaced auto specifier
			if (_nextShipLaser >= m_playerLaserList->size())
				_nextShipLaser = 0;																				// Reset laser list index to 0 (go back to start of list)

			// Set laser spawn points
			if (amount < 4)	(i == 0) ? yPos = 0 : (i == 1) ? yPos = -12 : yPos = 12;								// 0 = midd
			else (i == 0) ? yPos = 8 : (i == 1) ? yPos = -12 : (i == 2) ? yPos = 12 : yPos = -8;				// 0. = 5, 1. = 12, 2. = 12, 3. = -5

			// Set the initial rotation of the lasers
			shipLaser->setPosition(player->getPosition() + Point(shipLaser->getContentSize().width/2,yPos));
			if (amount == 2) (i == 0) ? rot = -5 : rot = 5;														// Top, bottom lasers
			if (amount == 3) (i == 0) ? rot = 0 : (i == 1) ? rot = 5 : rot = -5;								// Middle, bottom, top lasers
			if (amount == 4) (i == 0) ? rot = -3 : (i == 1) ? rot = 5 : (i == 2) ? rot = -5 : rot = 3;			// laser 1: i = 2 (5), laser 2: i = 0 (3), laser 3: i = ,laser 4: i = 1 (-5)
			shipLaser->setRotation(rot);

			shipLaser->setVisible(true);
			shipLaser->stopAllActions();

			// Where to fire the lasers
			if (amount == 2) (i == 0) ? yVal = 60 : yVal = -60;													// if 2 lasers, first one goes up, second goes down
			if (amount == 3) (i == 1) ? yVal = -100 : (i == 2) ? yVal = 100 : yVal = 0;							// if 3 lasers, first goes straight, second goes down, third goes up
			if (amount == 4) (i == 1) ? yVal = -120 : (i == 2) ? yVal = 120 : (i == 3) ? yVal = -40 : yVal = 40;// if 3 lasers, first goes straight, second goes down, third goes up

			shipLaser->runAction(
				Sequence::create(MoveBy::create(0.5, Point(winSize.width, yVal)),								// change to plus 100 for up - 100 for down
					CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
		}

		m_nextFire = curTimeMillis + m_fireRate;																// Set the next time to fire
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

		if (controller->getButton(10)->isSelected()) {													// Up arrow pressed
			//spawnLasers(NUM_LASERS_TO_FIRE);
			spawnLasers(player->getWeaponStrength());
			//CCLOG("Button A is pressed!");
		}
		else if (controller->getButton(11)->isSelected()) {												// Down arrow pressed
			//spawnLasers(NUM_LASERS_TO_FIRE);
			spawnLasers(player->getWeaponStrength());
			//CCLOG("Button B is pressed!");
		}
	}

	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE)) {
		spawnLasers(player->getWeaponStrength());														// Spawn lasers, depending on level of weapon	
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

void Level::PowerUpCollision(PowerUp* powerUp) {
	if (player->getBoundingBox().intersectsRect(powerUp->getBoundingBox())) {							// If the ship collides with an asteroid
		player->runAction(Blink::create(1.0F, 9));														// Flash the Player ship
		powerUp->setVisible(false);																		// Hide the power up
		Audio::Instance()->powerUpFX();																	// Play the power up sound effect
		Game::Instance()->updateScore(50);																// Award 50 points for collecting a power up
	}
}

void Level::checkCollisions() {
	// Enemy laser move off screen
	for (cocos2d::Sprite* enemyLaser : *m_enemyLaserList1) {
		if (!(enemyLaser->isVisible())) continue;
		if (enemyLaser->getPosition().x <= -enemyLaser->getContentSize().width)							// If the laser moves off screen it's own width
			enemyLaser->setVisible(false);																// Hide the laser
	}

	// Check player has picked up a power up
	if (m_powerUpLife->isVisible()) {
		if (player->getBoundingBox().intersectsRect(m_powerUpLife->getBoundingBox())) {					// If the ship collides with an asteroid
			PowerUpCollision(m_powerUpLife);
			Game::Instance()->addLife();																// Add a life (up to 5 lives for a player)
		}
	}
	if (m_powerUpWeapon->isVisible()) {
		if (player->getBoundingBox().intersectsRect(m_powerUpWeapon->getBoundingBox())) {				// If the ship collides with an asteroid
			PowerUpCollision(m_powerUpWeapon);
			player->upgradeWeapon();																	// Updgrade the players weapon
		}
	}

	// Asteroids Collisions
	for (cocos2d::Sprite* asteroid : *m_asteroidsList) {												// JOR replaced auto specifier
		if (!(asteroid->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *m_playerLaserList) {											// JOR replaced auto specifier
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
		if (player->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {						// If the ship collides with an asteroid
			asteroid->setVisible(false);																// Destroy the asteroid
			player->runAction(Blink::create(1.0F, 9));													// Flash the Player ship
			Game::Instance()->takeLife();																// Decrement the number of lives
			Audio::Instance()->explodePlayerFX();														// Play the explosion effect
		}
	}
	
	// Enemy ship collisions
	for (EnemyShip* enemyShip : *m_enemyShipList) {														
		if (!(enemyShip->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *m_playerLaserList) {											// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(enemyShip->getBoundingBox())) {
				Audio::Instance()->explodeFX();															// Play explosion effect
				shipLaser->setVisible(false);															// Hide the player laser
				Game::Instance()->updateScore(20);														// Award 20 points for destroying an enemy ship
				//if (enemyShip->isVisible()) Game::Instance()->incrementEnemyShipKills();				// Increment the total number of enemy ships destroyed
				//enemyShip->setLives(enemyShip->getLives() - 1);		// DECREMENT LIVES
				enemyShip->takeLife();
				//enemyShip->updateBar( (enemyShip->getLives() * 100 )/ 3.0f);			
				//if (enemyShip->getLives() < 1) enemyShip->setVisible(false);
			}
		}
	}
}

void Level::checkGameOver(float currenTime) {															// If the player has run out of lives
	if (Game::Instance()->getLives() <= 0) {															// If the player has run out of lives
		player->stopAllActions();																		// CCNode.cpp
		player->setVisible(false);																		// Destroy the ship
		endScene(KENDREASONLOSE);																		// Player has died
	}
	else if (currenTime >= Game::Instance()->getEndTime()) {
		endScene(KENDREASONWIN);																		// Player stays playing for the length of time
	}
}

void Level::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event){
	//if (Game::Instance()->isGameOver()) return;														// If the ship is not visible
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX 
		|| CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
		if (!Game::Instance()->isGameOver())															// If the ship is visible/alive
			spawnLasers(player->getWeaponStrength());													// Fire a laser, amount of lasers depends on weapon level		
	}
	CCLOG("Screen Touched");
}

void Level::endScene(EndReason endReason) {
	if (Game::Instance()->isGameOver()) return;															// If already game over, skip this function
	Game::Instance()->setGameOver(true);																// Set game over
	Game::Instance()->setLivesCarried(true);															// Carry over lives to next level
	
	const int TOTAL_LIST_ELEMENTS = 9;																	// Used to vertically space menu items
	
	// Win / Lose Message
	std::string message = "Level " + StringUtils::toString(Game::Instance()->getLevel()) + " Complete";	// Win 
	if (endReason == KENDREASONLOSE) message = "You Lose";

	// 1. Level Over Message
	levelCompleteLbl = cocos2d::Label::createWithTTF(message, 
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);
	levelCompleteLbl->setPosition(winSize.width / 2, 
		winSize.height - (winSize.height / TOTAL_LIST_ELEMENTS * 2));
	this->addChild(levelCompleteLbl);
	
	// 2. Total Asteroids Destroyed
	cocos2d::Label* asteroidsLbl = cocos2d::Label::createWithTTF("Total Asteroids Destroyed: " + 
		StringUtils::toString(Game::Instance()->getAsteroidKills()), 
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);										// JOR replaced auto specifier
	asteroidsLbl->setPosition(Point(visibleSize.width * 0.5f + origin.x,
		visibleSize.height - (visibleSize.height / TOTAL_LIST_ELEMENTS * 3)));
	asteroidsLbl->setColor(Color3B::RED);
	//asteroidsLbl->setScale((visibleSize.height == 1080) ? 1.5f : 1.0f);
	this->addChild(asteroidsLbl);
		
	// Display end of level stats
	statBarEOL((float)Game::Instance()->getAsteroidKills() / (float)Game::Instance()->getAsteroidCount(), TOTAL_LIST_ELEMENTS, 3.5f);
	statBarEOL((float)Game::Instance()->getEnemyShipKills() / (float)Game::Instance()->getEnemyShipCount(), TOTAL_LIST_ELEMENTS, 4.5f);
			
	// 3. Total Enemy Ships Destroyed
	cocos2d::Label* enemyShipsLbl = cocos2d::Label::createWithTTF("Total Enemy Ships Destroyed: " + 
		StringUtils::toString(Game::Instance()->getEnemyShipKills()), 
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);										// JOR replaced auto specifier
	enemyShipsLbl->setPosition(Point(visibleSize.width * 0.5f + origin.x, 
		visibleSize.height - (winSize.height / TOTAL_LIST_ELEMENTS * 4)));
	enemyShipsLbl->setColor(Color3B::RED);
	this->addChild(enemyShipsLbl);

	// 4. Restart Level
	message = "Restart Game";
	cocos2d::Label* restartLbl = Label::createWithBMFont("Arial.fnt", message);							// JOR replaced auto specifier
	restartItem = MenuItemLabel::create(restartLbl,CC_CALLBACK_1(Level::restartTapped,this));			// JOR replaced auto specifier
	restartItem->setPosition(winSize.width / 2, 
		winSize.height - (winSize.height / TOTAL_LIST_ELEMENTS * 6));
	
	unsigned int level = Game::Instance()->getLevel();

	// 5. Continue To Next Level
	if (endReason != KENDREASONLOSE && level < 4)
		message = "Start Level " + StringUtils::toString(Game::Instance()->getLevel()+1);
	if (level == 4 || endReason == KENDREASONLOSE)
		message = "Main Menu";

	cocos2d::Label* continueLbl = Label::createWithBMFont("Arial.fnt", message);						// JOR replaced auto specifier
	// Level Progression
	if (endReason != KENDREASONLOSE) {
		levelProgression(continueLbl);
	}
	else {
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::returnToMenu, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		Game::Instance()->setLevel(1);																	// Start again
	}
	continueItem->setPosition(winSize.width / 2, 
		winSize.height - (winSize.height / TOTAL_LIST_ELEMENTS * 7));

	// Animate the men items
	continueItem->runAction(ScaleTo::create(0.5F, (visibleSize.height == RES_720P) ? 1.0f : 1.5f));		// Scale the continue to next level label	
	restartItem->runAction(ScaleTo::create(0.5F, (visibleSize.height == RES_720P) ? 1.0f : 1.5f));		// Scale the restart button label
	levelCompleteLbl->runAction(ScaleTo::create(0.5F, (visibleSize.height == RES_720P) ? 1.0f : 1.5f));	// Scale the level complete message label

	cocos2d::Menu* menu = Menu::create(restartItem, continueItem, NULL);								// JOR replaced auto specifier
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	this->unscheduleUpdate();																			// Terminate update callback
}

/*
	Display a percentage of objects destroyed
*/
void Level::statBarEOL(float percent, int elements, float y) {
	if (Game::Instance()->getEnemyShipCount() > 0) {
		HealthBar* healthBar2 = HealthBar::create(
			winSize.width * 0.5f, winSize.height - ((winSize.height / elements) * y),					// Position
			(visibleSize.height == 720) ? 200 : 300, (visibleSize.height == 720) ? 20 : 30,				// Dimensions
			percent,																					// percentage
			cocos2d::Color4F(1, 0, 0, 1), cocos2d::Color4F(1, 0, 0, 0.5f), true);						// Colours & show label
		this->addChild(healthBar2);
	}
}

void Level::levelProgression(cocos2d::Label* continueLbl) {
	unsigned int level = Game::Instance()->getLevel();

	if (level == 1)
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::startLevel2, this));
	else if (level == 2)
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::startLevel3, this));
	else if (level == 3)
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::startLevel4, this));
	else if (level == 4)
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::returnToMenu, this));

	Game::Instance()->setNextLevel();																	// for parallax node init
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