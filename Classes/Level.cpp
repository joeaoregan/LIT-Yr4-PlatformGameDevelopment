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
#include "StoryScene.h"
#include "GameOverScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"																// For leaderboard
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"													// 20180422 Google Analytics
#endif

// Because cocos2d-x requres createScene to be static, we need to make other non-pointer members static
std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> Input::keys;

Level* Level::s_pLayerInstance;																				// Singleton for Level

cocos2d::Scene* Level::createScene() {
	cocos2d::Scene* scene = cocos2d::Scene::create();														// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level::create();																		// 'layer' is an autorelease object, JOR replaced auto specifier   
    scene->addChild(s_pLayerInstance);																		// Add layer as a child to scene	    
    return scene;																							// Return the scene
}

// on "init" you need to initialize your instance
bool Level::init() {
    if ( !Layer::init() ) { return false; }																	// super init first

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginGoogleAnalytics::logTiming("StartGame", 0, "Test Time", "Level init");					// Google Analytics: Register game exit time for menu button
#endif
	/*
	// Not registering with leaderboard

	// Moved to main menu, as shows up every level
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginSdkboxPlay::signin();																		// Sign in for leaderboard
	sdkbox::PluginSdkboxPlay::submitScore("leaderboard_my_leaderboard", 999);													// Add the score to the leaderboard
	sdkbox::PluginSdkboxPlay::submitScore("spacequest_leaderboard", 999);														// Add the score to the leaderboard
#endif
*/

	Game::Instance()->setGameOver(false);																	// Needed for starting new level, or restarting game
	Game::Instance()->setWon(false);																		// Has the boss been defeated

	// Screen size and position
    m_visibleSize = cocos2d::Director::getInstance()->getVisibleSize();										// Store the screen resolution
    m_Origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	(m_visibleSize.height == 720) ? m_scaleUp = 1.0f : m_scaleUp = 1.5f;
	(m_visibleSize.height == 1080) ? m_scaleDown = 1.0f : m_scaleDown = 0.67f;

	//  GALAXY
	m_batchNode = cocos2d::SpriteBatchNode::create("Sprites.pvr.ccz");										// Player sprite is added here
	this->addChild(m_batchNode);

	m_batchNodeJOE = cocos2d::SpriteBatchNode::create("JoeSprites.png");
	this->addChild(m_batchNodeJOE);

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");						// Add collection of SpaceGame sprites to be accessed
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("JoeSprites.plist");
	
	player = Player::create(m_visibleSize);
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
	m_enemyShipList = new cocos2d::Vector<EnemyShip*>(3);													// initEnemyShips now virtual function so Vector must be outside function
	initEnemyShips();
	initLasers();
	initDifficulty();																						// Initialise fire rate, weapons state, and power up duration on screen

	// Touch screen / Mouse press
	m_touchListener = cocos2d::EventListenerTouchAllAtOnce::create();										// JOR replaced auto specifier
	m_touchListener->onTouchesBegan = CC_CALLBACK_2(Level::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
		
	//float dpadPos = 375.0f;
	//(m_visibleSize.height == 1080) ? dpadPos = 375.0f : (m_visibleSize.height == 720) ? dpadPos = 250.0f : dpadPos * (m_visibleSize.height / 1080);

	// D-pad (Display on mobile device)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {				// If the target platform is a mobile device
		m_pController = DPad::create(DPAD_BASE, DPAD_ARROW, DPAD_ARROW_ACTIVE,
			(m_visibleSize.height == 1080) ? cocos2d::Point(375, 375) : (m_visibleSize.height == 720) ? Point(250, 250) : Point(375 * (m_visibleSize.height / 1080), 375 * (m_visibleSize.height / 1080)));

		//m_pController->setScale((m_visibleSize.height == 1080) ? 1.0f : (m_visibleSize.height == 720) ? 0.67f : m_visibleSize.height / 1080); // puts buttons out of position

		this->addChild(m_pController);
	}

	m_pHUD = HUD::create(m_Origin, m_visibleSize);															// Create the HUD at the origin point (0,0), and passing in the screen resolution
	this->addChild(m_pHUD);

	// Music Player
	if (Game::Instance()->musicPlayerVisible()) {
		m_pMPlayer = MusicPlayer::create(cocos2d::Point((m_visibleSize.width / 2),
			m_visibleSize.height * 0.065f));																// Position: scale in MusicPlayer class throws off measurement (undo first)
		this->addChild(m_pMPlayer);
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
			Game::Instance()->setLives(5);																	// Set the number of lives for the player
		else if (Game::Instance()->getDifficulty() == MEDIUM)
			Game::Instance()->setLives(3);																	// Set the number of lives for the player
		else
			Game::Instance()->setLives(2);																	// Set the number of lives for the player

		CCLOG("Level %d - Init Lives - Lives NOT Carried Over - Start With: %d",
			Game::Instance()->getLevel(), Game::Instance()->getLives());

		Game::Instance()->setHealth(10);																	// Reset health to 10 if not carrying over
	} else {
		CCLOG("Level %d - Init Lives - Lives ARE Carried Over: %d", 
			Game::Instance()->getLevel(), Game::Instance()->getLives());
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
	/*
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		EnemyShip* enemyShip1 = EnemyShip::create(m_visibleSize);
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
		//CCLOG("LEVEL - Add Enemy ship at array index %d", i);
	}
	*/
}

/*
	Same number of asteroid obstacles in every level
*/
void Level::initAsteroids() {
	// Asteroids
	m_asteroidsList = new cocos2d::Vector<Asteroid*>(MAX_NUM_ASTEROIDS_L1);									// List of asteroids

	for (int i = 0; i < MAX_NUM_ASTEROIDS_L1; ++i) {
		Asteroid* asteroid = AsteroidOriginal::create(m_visibleSize);										// Add one of 1st type of asteroid
		m_batchNode->addChild(asteroid);
		m_asteroidsList->pushBack(asteroid);

		// Add base class asteroid
		Asteroid* asteroid2 = Asteroid::create(m_visibleSize);												// Add one of 2nd type of asteroid
		this->addChild(asteroid2);
		m_asteroidsList->pushBack(asteroid2);
	}
	CCLOG("Level %d: Asteroids Initialised", Game::Instance()->getLevel());
}

void Level::initLasers() {
	// Player Ship Lasers:
	m_playerLaserList = new cocos2d::Vector<cocos2d::Sprite*>(NUM_LASERS);									// List of lasers

	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* shipLaser = cocos2d::Sprite::createWithSpriteFrameName(LASER_BLUE_IMG);			// Laser sprite, JOR replaced auto specifier
		shipLaser->setVisible(false);
		m_batchNode->addChild(shipLaser);
		m_playerLaserList->pushBack(shipLaser);
	}
	//CCLOG("TEST Level::initLasers() Ship Lasers: %d", m_playerLaserList->size());

	// Enemy Lasers
	m_enemyLaserList1 = new cocos2d::Vector<cocos2d::Sprite*>(NUM_LASERS);									// List of lasers for enemy 1

	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* enemyLaser = cocos2d::Sprite::createWithSpriteFrameName(LASER_BLUE_IMG);			// Laser sprite, JOR replaced auto specifier
		enemyLaser->setVisible(false);
		m_batchNode->addChild(enemyLaser);
		m_enemyLaserList1->pushBack(enemyLaser);
	}
	//CCLOG("Level %d: Enemy Ships Initialised", Game::Instance()->getLevel());

	// Enemy 2 Lasers
	m_enemyLaserList2 = new cocos2d::Vector<cocos2d::Sprite*>(NUM_LASERS);									// List of lasers for enemy 2

	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* enemyLaser = cocos2d::Sprite::createWithSpriteFrameName(LASER_ORANGE_IMG);			// Laser sprite, now in sprite sheet, JOR replaced auto specifier 
		enemyLaser->setVisible(false);
		this->addChild(enemyLaser);
		m_enemyLaserList2->pushBack(enemyLaser);
	}
}

/*
	Initialise the new life and weapon power ups
*/
void Level::initPowerUps() {
	m_pPowerUpLife = PowerUp::create(m_visibleSize, NEW_LIFE);
	this->addChild(m_pPowerUpLife);

	m_pPowerUpWeapon = PowerUp::create(m_visibleSize, WEAPON_UPGRADE);
	this->addChild(m_pPowerUpWeapon);
	//CCLOG("Level %d: New Life Power Up Initialised", Game::Instance()->getLevel());
}

void Level::update(float dt) {
	m_curTimeMillis = Game::Instance()->getTimeTick();														// Current game time

	m_winSize = cocos2d::Director::getInstance()->getWinSize();												// Dimensions of game screen (Needs to update here so lasers fire etc.)

	getInput();																								// Get keyboard input for Windows, Get DPad input for Android

	Game::Instance()->updateTimer(m_curTimeMillis);															// Update the countdown timer in Game class, pass in curTimeMillies solves Android Timer issue

	m_backgroundNode->update(dt);																			// Scroll the background objects

	spawnObjects(m_curTimeMillis);																			// Spawn asteroids
	spawnEnemyShips(m_curTimeMillis);																		// Spawn asteroids
	
	checkCollisions();																						// Check have game objects collided with each other

	checkGameOver(m_curTimeMillis);																			// Check is the game over or not
	
	player->update();																						// Update player sprite position, taking input from the keyboard
	
	for (EnemyShip* enemyShip : *m_enemyShipList) {
		enemyShip->update(m_curTimeMillis);																	// Fire Enemy Weapons etc.
	}

	if (Game::Instance()->musicPlayerVisible()) m_pMPlayer->update();										// Update the music player

	m_pHUD->update(m_curTimeMillis);																		// Update the HUD
}

/*
	Spawn power ups and asteroids, no real difference only what happens on collision
*/
void Level::spawnObjects(float curTimeMillis) {
	// Rotate
	cocos2d::RotateBy* rotate = cocos2d::RotateBy::create(2.5f, -360.0f);
	cocos2d::RepeatForever* repeat = cocos2d::RepeatForever::create(rotate);

	//powerUpTime = 2000;	// test
	if (!m_pPowerUpLife->isSpawned() && curTimeMillis > m_pPowerUpLife->getSpawnTime()) {
		m_pPowerUpLife->setVisible(true);
		cocos2d::MoveTo* actionpowerUp = cocos2d::MoveTo::create(m_powerUpDuration,
			cocos2d::Point(0 - m_pPowerUpLife->getContentSize().width,
				m_visibleSize.height * m_pPowerUpLife->getRandY()));
		m_pPowerUpLife->runAction(actionpowerUp);

		m_pPowerUpLife->runAction(repeat);
		m_pPowerUpLife->setSpawned();
	}

	if (!m_pPowerUpWeapon->isSpawned() && curTimeMillis > m_pPowerUpWeapon->getSpawnTime()) {
		m_pPowerUpWeapon->setVisible(true);
		cocos2d::MoveTo* actionpowerUp2 = cocos2d::MoveTo::create(m_powerUpDuration,
			cocos2d::Point(0 - m_pPowerUpWeapon->getContentSize().width,
				m_visibleSize.height * m_pPowerUpWeapon->getRandY()));
		m_pPowerUpWeapon->runAction(actionpowerUp2);
		m_pPowerUpWeapon->runAction(repeat);																	// Reuse the same sequence for weapon power ups
	
		CCLOG("Spawn Weapon Power Up");
		m_pPowerUpWeapon->setSpawned();
	}
	
	if (curTimeMillis > m_nextAsteroidSpawn) {
		float randMillisecs = randomValueBetween(0.20F, 1.0F) * 1000;
		m_nextAsteroidSpawn = randMillisecs + curTimeMillis;

		Asteroid *asteroid = m_asteroidsList->at(m_nextAsteroid);
		m_nextAsteroid++;																					// Increment the asteroid list index
		if (m_nextAsteroid >= m_asteroidsList->size()) m_nextAsteroid = 0;									// Loop back around to start of asteroids list

		Game::Instance()->incrementAsteroidCount();
		
		asteroid->init(m_visibleSize);																		// Initialise the Asteroid, stopping all active actions, set visibility, and random position, scale, and rotate
		asteroid->runAction(cocos2d::Sequence::create(														// Sequence of actions where the asteroid moves off screen to the left
			cocos2d::MoveBy::create(asteroid->getDuration(),
				cocos2d::Point(-m_winSize.width - asteroid->getContentSize().width, 0)),					// To a position fully off screen
			cocos2d::CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)						// And is then destroyed, DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
		);	

		//CCLOG("Spawn Asteroid");
	}
}

/*
	Spawn the Enemy Ships
*/
void Level::spawnEnemyShips(float curTimeMillis) {
	if (m_enemyShipList->size() > 0) {
		//CCLOG("Spawn Enemy Ship - size > 0");
		if (curTimeMillis > m_nextEnemyShipSpawnTime) {
			float randMillisecs = randomValueBetween(0.20F, 1.0F) * 2500;
			m_nextEnemyShipSpawnTime = randMillisecs + curTimeMillis;										// Set the time to spawn the next ship			

			EnemyShip *enemyShip = m_enemyShipList->at(m_nextEnemyShip); 
			m_nextEnemyShip++;																				// Moved to use min and max speed
			if (m_nextEnemyShip >= (unsigned int)m_enemyShipList->size()) m_nextEnemyShip = 0;				// Loop back around to start of enemy ship list
										
			Game::Instance()->incrementEnemyShipCount();													// Increment the enemy ship on the list
			
			enemyShip->init(m_visibleSize);																	// Stop actions, set position, set visible, and set lives

			//CCLOG("Spawn Enemy Ship init enemy ship");
			// Apply Actions: Move the ship to the players coordinate
			cocos2d::MoveTo* action = cocos2d::MoveTo::create(enemyShip->getDuration() * 0.67f,
				cocos2d::Point(player->getPositionX(), player->getPositionY()));							// Part of time spent moving to player, the rest moving off screen
			enemyShip->runAction(action);

			//CCLOG("Spawn Enemy Ship - MoveTo");
			enemyShip->runAction(																						
				cocos2d::Sequence::create(
					cocos2d::MoveBy::create(enemyShip->getDuration(),
						cocos2d::Point(-m_winSize.width - enemyShip->getContentSize().width, 0)),			// move off the screen its full width
					cocos2d::CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)				// Then set invisible if it reaches the target - TERMINATE WITH NULL
			);
			//CCLOG("Spawn Enemy Ship - END");
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
	cocos2d::Vector<cocos2d::Sprite*> *tempList;

	if (type == BLUE) {
		tempList = m_enemyLaserList1;																		// Specific list for each colour laser, as different sprites used
		Audio::Instance()->playFX(LASER_ENEMY1);															// Play enemy 1 laser audio
	}
	else if (type == ORANGE) {
		tempList = m_enemyLaserList2;
		Audio::Instance()->playFX(LASER_ENEMY2);															// Play enemy 2 laser audio
	}
	else if (type == GREEN1 || type == GREEN2 || type == GREEN3) {
		tempList = m_enemyLaserList3;
		Audio::Instance()->playFX(LASER_ENEMY3);															// Play enemy 2 laser audio
	}
	
	enemyLaser = tempList->at(m_nextEnemyLaser);															// Use the laser off the correct list
	
	m_nextEnemyLaser++;																						// Increment the lasers
	if (m_nextEnemyLaser >= tempList->size()) m_nextEnemyLaser = 0;											// Loop back to start of laser list

	if (enemyLaser->isVisible()) return;																	// If the laser is already visible skip it

	enemyLaser->setRotation(0);
	enemyLaser->setPosition(pos.x, pos.y);																	// Set the postion relevant to the ships coordinates
	enemyLaser->setVisible(true);																			// Set visible on screen
	enemyLaser->stopAllActions();																			// Stop actions for the laser

	cocos2d::MoveTo *action;																				// Declare the action
	
	if (type == GREEN2)
	action = cocos2d::MoveTo::create(0.525f,																// This is where Euclidean distance would be handy
		cocos2d::Point(pos.x - m_visibleSize.width - enemyLaser->getContentSize().width -					// Laser 2 travels up
				getContentSize().width, pos.y + m_visibleSize.height * 0.33f));
	else if (type == GREEN3)
		action = cocos2d::MoveTo::create(0.525f,
			cocos2d::Point(pos.x - m_visibleSize.width - enemyLaser->getContentSize().width -				// Laser 3 travels down
				getContentSize().width, pos.y - m_visibleSize.height * 0.33f));
	else	
		action = cocos2d::MoveTo::create(0.5f,																// Laser 1 travels centrally
			cocos2d::Point(pos.x - m_visibleSize.width - enemyLaser->getContentSize().width -
				getContentSize().width, pos.y));															// set to off screen the width of the laser + the screen width

	enemyLaser->runAction(action);																			// Start moving the laser sprite
}

/* 
	20180221 Up to 4 different types of laser, with different spawn points and rotations
*/
void Level::spawnLasers(int amount) {
	if (m_curTimeMillis > m_nextFire) {
		Audio::Instance()->playFX(LASER_SHIP);																	// Play the Player ship laser fire effect

		int yVal = 0, yPos = 0, rot = 0;																		// y value for point to aim for, y position of laser spawn point, rotation of laser	

		for (int i = 0; i < amount; i++) {
			cocos2d::Sprite* shipLaser = m_playerLaserList->at(m_nextShipLaser++);								// Next laser in the list, JOR replaced auto specifier
			if (m_nextShipLaser >= m_playerLaserList->size())
				m_nextShipLaser = 0;																			// Reset laser list index to 0 (go back to start of list)

			// Set laser spawn points
			if (amount < 4)	(i == 0) ? yPos = 0 : (i == 1) ? yPos = -12 : yPos = 12;							// 0 = midd
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
				cocos2d::Sequence::create(cocos2d::MoveBy::create(0.5, Point(m_winSize.width, yVal)),			// change to plus 100 for up - 100 for down
					cocos2d::CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
		}

		m_nextFire = m_curTimeMillis + m_fireRate;																// Set the next time to fire
	}
}

void Level::getInput() {
	// Android DPad (maybe change to returning a point (0,0)(1,0)(0,1),(-1,0),(0,-1)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {					// If the platform is mobile
		if (m_pController->getButton(8)->isSelected()) {														// Up arrow pressed
			player->moveUp();
			//CCLOG("Down button is pressed!");
		}
		else if (m_pController->getButton(2)->isSelected()) {													// Down arrow pressed
			player->moveDown();
			//CCLOG("Down button is pressed!");
		}
		if (m_pController->getButton(4)->isSelected()) {														// Left arrow pressed
			player->moveLeft();
			//CCLOG("Down button is pressed!");
		}
		else if (m_pController->getButton(6)->isSelected()) {													// Right arrow pressed
			player->moveRight();
			//CCLOG("Down button is pressed!");
		}

		if (m_pController->getButton(10)->isSelected()) {														// Up arrow pressed
			spawnLasers(player->getWeaponStrength());
			//CCLOG("Button A is pressed!");
		}
		else if (m_pController->getButton(11)->isSelected()) {													// Down arrow pressed
			spawnLasers(player->getWeaponStrength());
			//CCLOG("Button B is pressed!");
		}
	}

	if (Input::Instance()->isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_SPACE)) {
		spawnLasers(player->getWeaponStrength());																// Spawn lasers, depending on level of weapon	
	}

	/*
	// Highlight DPAD buttons with keyboard press (For trailer)
	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) ||
	Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
		m_pController->getButton(8)->selected();
	}
	else m_pController->getButton(8)->unselected();
	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) ||
	Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
		m_pController->getButton(2)->selected();
	}
	else m_pController->getButton(2)->unselected();
	// Shows the button as active in Windows / Linux / Mac
	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) ||
	Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
		m_pController->getButton(4)->selected();
	}
	else m_pController->getButton(4)->unselected();

	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ||
	Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
		m_pController->getButton(6)->selected();
	}
	else m_pController->getButton(6)->unselected();
	*/
}

/*
	Generate a random number from a range
*/
float Level::randomValueBetween(float low, float high) {
	// from http://stackoverflow.com/questions/686353/c-random-float-number-generation
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

void Level::setInvisible(cocos2d::Node * node) {
	node->setVisible(false);																					// Set the node invisible
}

/*
	Check for collisions with different power ups
*/
void Level::PowerUpCollision(PowerUp* powerUp) {
	if (player->getBoundingBox().intersectsRect(powerUp->getBoundingBox())) {									// If the ship collides with an asteroid
		player->runAction(cocos2d::Blink::create(1.0F, 9));														// Flash the Player ship
		powerUp->collected();																					// Set not visible, update the score (+50), and play pickup sound
	}
}

/*
	Check collisions between the player and different objects
*/
void Level::checkCollisions() {
	int currentWeaponGrade = player->getWeaponStrength();
	std::stringstream upgrade;

	// Blue Enemy laser collisions
	for (cocos2d::Sprite* enemyLaser : *m_enemyLaserList1) {
		if (!(enemyLaser->isVisible())) continue;
		if (enemyLaser->getPosition().x <= -enemyLaser->getContentSize().width)									// If the laser moves off screen it's own width
			enemyLaser->setVisible(false);																		// Hide the laser

		// Check collisions between the player ship and Laser type 1 (blue)
		if (player->getBoundingBox().intersectsRect(enemyLaser->getBoundingBox())) {							// If the ship collides with an asteroid
			enemyLaser->setVisible(false);																		// Destroy the asteroid
			player->damage();
		}
	}

	// Check player has picked up a power up
	if (m_pPowerUpLife->isVisible()) {
		if (player->getBoundingBox().intersectsRect(m_pPowerUpLife->getBoundingBox())) {						// If the ship collides with an asteroid
			PowerUpCollision(m_pPowerUpLife);
			Game::Instance()->addLife();																		// Add a life (up to 5 lives for a player)

			if (!Game::Instance()->getAchievedLife()) {
				CCLOG("***************************** NEW LIFE POWER UP ***************************");
				CCLOG("Player has picked up a new life power up");												// Indicate achievement unlocked

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Collect Power Up", 5);		// Google Analytics (18 characters max?)
#endif
				Game::Instance()->setAchievedLife(true);														// Mark the achievement as completed
			}
		}
	}

	/*
		Added achievement for collecting weapon powerup
	*/
	if (m_pPowerUpWeapon->isVisible()) {
		if (player->getBoundingBox().intersectsRect(m_pPowerUpWeapon->getBoundingBox())) {						// If the ship collides with an asteroid
			PowerUpCollision(m_pPowerUpWeapon);																	// Check has player collided with a powerup
			player->upgradeWeapon();																			// Updgrade the players weapon

			if (currentWeaponGrade != player->getWeaponStrength()) {											// If the current weapon level has changed
				CCLOG("***************************** WEAPON UPGRADED ***************************");
				CCLOG("Weapon Upgrade Level: %d", player->getWeaponStrength());									// Show the upgrade level in the Output pane

				upgrade << "Laser Upgrade " << player->getWeaponStrength();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				sdkbox::PluginGoogleAnalytics::logEvent("Achievement",
					//"Upgrade", "Weapon Upgrade Level: " + player->getWeaponStrength(), 5);					// Google Analytics
					//"Upgrade", "Laser Upgrade: " + player->getWeaponStrength(), 5);							// Google Analytics (16 characters max?)
					"Upgrade", upgrade.str(), 5);																// const char 17
#endif
			}
		}
	}

	// Asteroids Collisions
	for (Asteroid* asteroid : *m_asteroidsList) {																// JOR replaced auto specifier
		if (!(asteroid->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *m_playerLaserList) {													// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;															// If the ship is not visible

			if (shipLaser->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {
				shipLaser->setVisible(false);																	// Hide the laser
				asteroid->damage();																				// Destroy asteroid, update score, play sound effect, increment count of asteroids destroyed
			}
		}

		// Check collisions between the player ship and asteroids
		if (player->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {								// If the ship collides with an asteroid
			asteroid->setVisible(false);																		// Destroy the asteroid
			player->damage();																					// Flash hte player, decrease health/take life, play explosion/damage sound
		}
	}
	
	// Enemy ship collisions
	for (EnemyShip* enemyShip : *m_enemyShipList) {														
		if (!(enemyShip->isVisible())) continue;																// Skip if the enemy ship is not visible

		// Collisions with player laser
		for (cocos2d::Sprite* shipLaser : *m_playerLaserList) {													// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;															// If the laser is not visible skip the rest

			if (shipLaser->getBoundingBox().intersectsRect(enemyShip->getBoundingBox())) {
				shipLaser->setVisible(false);																	// Hide the player laser
				enemyShip->damage();																			// Decrease health, update score, play sound effect, update health bar
			}
		}

		// Collisions with Player Ship
		if (player->getBoundingBox().intersectsRect(enemyShip->getBoundingBox())) {								// If the ship collides with an asteroid
			enemyShip->setVisible(false);																		// Destroy the asteroid
			player->damage();
		}
	}
}

void Level::checkGameOver(float currenTime) {																	// Check is the game over
	if (Game::Instance()->getLives() <= 0) {																	// If the player has run out of lives
		player->stopAllActions();																				// CCNode.SpaceQuest
		player->setVisible(false);																				// Destroy the ship
		endScene(KENDREASONLOSE);																				// Player has died
	}
	else if (currenTime >= Game::Instance()->getEndTime()) {
		endScene(KENDREASONWIN);																				// Player stays playing for the length of time
	}
}

void Level::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event){
	//if (Game::Instance()->isGameOver()) return;																// If the ship is not visible
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX 
		|| CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
		if (!Game::Instance()->isGameOver())																	// If the ship is visible/alive
			spawnLasers(player->getWeaponStrength());															// Fire a laser, amount of lasers depends on weapon level		
	}
	CCLOG("Screen Touched");
}

void Level::updateLeaderboard() {
	CCLOG("Update Leaderboard");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginSdkboxPlay::submitScore("leaderboard_my_leaderboard", Game::Instance()->getScore());			// Add the score to the leaderboard
	sdkbox::PluginSdkboxPlay::submitScore("Score Leaderboard", 1000);											// Add the score to the leaderboard
	sdkbox::PluginSdkboxPlay::submitScore("joe_board", 1000);													// Add the score to the leaderboard
	//sdkbox::PluginSdkboxPlay::submitScore("leaderboard_spacequestleaderboard", Game::Instance()->getScore());	// Add the score to the leaderboard
#endif
}

void Level::endScene(EndReason endReason) {
	updateLeaderboard();																						// Update the score on the leaderboard

	if (endReason == KENDREASONLOSE) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		sdkbox::PluginGoogleAnalytics::logTiming("Exit Game", (int) Game::Instance()->getTimer(),
			"Exit Time", "Player Dead Time");																	// Google Analytics: Register game exit time for menu button
#endif
	}

	if (Game::Instance()->isGameOver()) return;																	// If already game over, skip this function
	Game::Instance()->setGameOver(true);																		// Set game over
	Game::Instance()->setLivesCarried(true);																	// Carry over lives to next level
	
	const int TOTAL_LIST_ELEMENTS = 9;																			// Used to vertically space menu items
	
	// Win / Lose Message
	std::string message = "Level " + cocos2d::StringUtils::toString(Game::Instance()->getLevel()) + " Complete";// Win 
	if (endReason == KENDREASONLOSE) message = "You Lose";

	// 1. Level Over Message
	m_pLevelCompleteLbl = cocos2d::Label::createWithTTF(message, 
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05);
	m_pLevelCompleteLbl->setPosition(m_winSize.width / 2, 
		m_winSize.height - (m_winSize.height / TOTAL_LIST_ELEMENTS * 2));
	this->addChild(m_pLevelCompleteLbl);
	
	// 2. Total Asteroids Destroyed
	cocos2d::Label* asteroidsLbl = cocos2d::Label::createWithTTF("Total Asteroids Destroyed: " + 
		cocos2d::StringUtils::toString(Game::Instance()->getAsteroidKills()),
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05);											// JOR replaced auto specifier
	asteroidsLbl->setPosition(cocos2d::Point(m_visibleSize.width * 0.5f + m_Origin.x,
		m_visibleSize.height - (m_visibleSize.height / TOTAL_LIST_ELEMENTS * 3)));
	asteroidsLbl->setColor(cocos2d::Color3B::RED);
	this->addChild(asteroidsLbl);
		
	// Display end of level stats
	statBarEOL((float)Game::Instance()->getAsteroidKills() / 
		(float)Game::Instance()->getAsteroidCount(), TOTAL_LIST_ELEMENTS, 3.5f);
	statBarEOL((float)Game::Instance()->getEnemyShipKills() / 
		(float)Game::Instance()->getEnemyShipCount(), TOTAL_LIST_ELEMENTS, 4.5f);

	killAchievement();
			
	// 3. Total Enemy Ships Destroyed
	cocos2d::Label* enemyShipsLbl = cocos2d::Label::createWithTTF("Total Enemy Ships Destroyed: " + 
		cocos2d::StringUtils::toString(Game::Instance()->getEnemyShipKills()),
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05);													// JOR replaced auto specifier
	enemyShipsLbl->setPosition(cocos2d::Point(m_visibleSize.width * 0.5f + m_Origin.x,
		m_visibleSize.height - (m_winSize.height / TOTAL_LIST_ELEMENTS * 4)));
	enemyShipsLbl->setColor(cocos2d::Color3B::RED);
	this->addChild(enemyShipsLbl);

	// 4. Restart Level
	message = "Restart Game";
	restartLbl = cocos2d::Label::createWithBMFont("Arial.fnt", message);												// JOR replaced auto specifier
	m_pRestartItem = cocos2d::MenuItemLabel::create(restartLbl,CC_CALLBACK_1(Level::restartTapped,this));				// JOR replaced auto specifier
	m_pRestartItem->setPosition(m_winSize.width / 2, 
		m_winSize.height - (m_winSize.height / TOTAL_LIST_ELEMENTS * 6));
	
	unsigned int level = Game::Instance()->getLevel();																	// Get the current level number

	// 5. Continue To Next Level
	if (endReason != KENDREASONLOSE && level < 4)
		message = "Start Level " + cocos2d::StringUtils::toString(Game::Instance()->getLevel()+1);						// Add the level number to the continue button text
	if (level == 4 || endReason == KENDREASONLOSE)
		message = "Continue";

	cocos2d::Label* continueLbl = cocos2d::Label::createWithBMFont("Arial.fnt", message);								// Set the continue label txt to Start Level x/Continue

	// Level Progression
	if (endReason != KENDREASONLOSE) {
		levelProgression(continueLbl);
	}
	else {
		m_pContinueItem = cocos2d::MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::returnToMenu, this));		// If the game is lost set the continue option to return to main menu
	}

	m_pContinueItem->setPosition(m_winSize.width / 2, 
		m_winSize.height - (m_winSize.height / TOTAL_LIST_ELEMENTS * 7));

	// Animate the men items
	m_pContinueItem->runAction(cocos2d::ScaleTo::create(0.5F, (m_visibleSize.height == RES_720P) ? 1.0f : 1.5f));		// Scale the continue to next level label	
	m_pRestartItem->runAction(cocos2d::ScaleTo::create(0.5F, (m_visibleSize.height == RES_720P) ? 1.0f : 1.5f));		// Scale the restart button label
	m_pLevelCompleteLbl->runAction(cocos2d::ScaleTo::create(0.5F, (m_visibleSize.height == RES_720P) ? 1.0f : 1.5f));	// Scale the level complete message label

	cocos2d::Menu* menu = cocos2d::Menu::create(m_pRestartItem, m_pContinueItem, NULL);									// JOR replaced auto specifier
	menu->setPosition(cocos2d::Point::ZERO);
	this->addChild(menu);

	this->unscheduleUpdate();																							// Terminate update callback
}

/*
	Display a percentage of objects destroyed
*/
void Level::statBarEOL(float percent, int elements, float y) {
	if (Game::Instance()->getEnemyShipCount() > 0) {
		HealthBar* healthBar2 = HealthBar::create(
			m_winSize.width * 0.5f, m_winSize.height - ((m_winSize.height / elements) * y),								// Position
			(m_visibleSize.height == 720) ? 200 : 300, (m_visibleSize.height == 720) ? 20 : 30,							// Dimensions
			percent,																									// percentage
			cocos2d::Color4F(1, 0, 0, 1), cocos2d::Color4F(1, 0, 0, 0.5f), true);										// Colours & show label
		this->addChild(healthBar2);
	}
}

void Level::levelProgression(cocos2d::Label* continueLbl) {
	unsigned int level = Game::Instance()->getLevel();

	if (level == 4)
		m_pContinueItem = cocos2d::MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::returnToMenu, this));
	else
		m_pContinueItem = cocos2d::MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::goToStoryScreen, this));
}

// Callbacks
void Level::restartTapped(cocos2d::Ref* pSender) {
	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionZoomFlipX::create(0.5, Level1::createScene()));		// Restart the current scene	
	this->scheduleUpdate();																									// reschedule
}
void Level::goToStoryScreen(cocos2d::Ref* pSender) {
	Game::Instance()->setNextLevel();																						// for parallax node init

	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionZoomFlipY::create(0.5, StoryScene::createScene()));	// Change scene, progressing to Level 2
}
void Level::returnToMenu(cocos2d::Ref* pSender) {
	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionRotoZoom::create(0.5, GameOverScene::createScene()));	// Go to game over scene
}
void Level::menuCloseCallback(cocos2d::Ref* pSender) {
	Audio::Instance()->playFX(BUTTON_FX);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
	cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

/*
	Unlock asteroid / enemy kills achievement
	If the kill rate is better than 50% for either Enemy Ships or Asteroids
	Or it the player completes the level / dies and destroys no objects with lasers
*/
void Level::killAchievement() {
	//CCLOG("Kill Rate Achievement");
	
	if (!Game::Instance()->getAchievedKills()) {															// If the achievement isn't complete
		if (Game::Instance()->getAsteroidKills() / 
			(float) Game::Instance()->getAsteroidCount() >= 0.5f ||											// Player gets a kill percentage of 50% or more for
			Game::Instance()->getEnemyShipKills() / 
			(float) Game::Instance()->getEnemyShipCount() >= 0.5f) {										// Asteroids or enemy ships
			CCLOG("Destroyed 50 Percent Or More Asteroids Or Enemy Ships");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Expert Marksman", 5);		// Google Analytics: Register the Expert Marksman achievement
#endif
			Game::Instance()->setAchievedKills(true);														// Mark the achievement as true
		}
	}
	
/*
// Test win
	if (!Game::Instance()->getAchievedKills()) {															// If the achievement isn't complete
		if (Game::Instance()->getAsteroidKills() / (float) Game::Instance()->getAsteroidCount() >= 0.5f) {	// Player gets a kill percentage of 50% or more
			CCLOG("Destroyed 50 Or More Asteroids %.2f", Game::Instance()->getAsteroidKills() / (float) Game::Instance()->getAsteroidCount());
			Game::Instance()->setAchievedKills(true);														// Mark the achievement as true
		}
	}

	if (!Game::Instance()->getAchievedKills()) {															// If the achievement isn't complete
		if (Game::Instance()->getEnemyShipKills() / (float) Game::Instance()->getEnemyShipCount() >= 0.5f) {
			CCLOG("Destroyed 50 Or More Enemy Ships %.2f", Game::Instance()->getEnemyShipKills() / (float) Game::Instance()->getEnemyShipCount());
			Game::Instance()->setAchievedKills(true);														// Mark the achievement as true
		}
	}
*/

	/*
		Kamikaze Achievement

		Player has completed the level / died
		without getting a laser on target
	*/
	if (!Game::Instance()->getAchievedKamikaze()) {															// If the achievement isn't complete
		if (Game::Instance()->getAsteroidKills() / (
			float) Game::Instance()->getAsteroidCount() == 0.0f &&											// Player didn't destroy any asteroids
			Game::Instance()->getEnemyShipKills() /															// and didn't destroy any enemy ships 
			(float) Game::Instance()->getEnemyShipCount() == 0.0f) {
			CCLOG("Destroyed Nothing %.2f %.2f", (float) (Game::Instance()->getAsteroidKills() /			// Display the percentages in output
				Game::Instance()->getAsteroidCount()),
				(float) (Game::Instance()->getEnemyShipKills() / 
				Game::Instance()->getEnemyShipCount() == 0.0f));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Kamikaze", 5);				// Google Analytics: Register the Kamikaze achievement
#endif
			Game::Instance()->setAchievedKamikaze(true);													// Mark the achievement as achieved
		}
	}
}
