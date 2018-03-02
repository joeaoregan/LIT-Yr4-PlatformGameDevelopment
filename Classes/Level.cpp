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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");

	// Player sprite
	//player = new Player(this);
	//_batchNode->addChild(player->getSprite(), 1);

	player = Player::create(visibleSize);
	//_batchNode->addChild(player, 1);
	player->setScale(scaleDown);
	this->addChild(player);

	// 1) Create the ParallaxNode
	m_backgroundNode = ParallaxNodeExtras::create();															// Create the parallax scrolling background

	Level::addChild(ParticleSystemQuad::create("Stars1.plist"));											// Add the star particles
	Level::addChild(ParticleSystemQuad::create("Stars2.plist"));
	Level::addChild(ParticleSystemQuad::create("Stars3.plist"));
		
	Game::Instance()->init();																				// Inite score and level	
	Input::Instance()->init(this, this->_eventDispatcher);													// Ship Movement

	initLives();																							// Initialise the number of lives, set or carry over from previous level
	initPowerUps();																							// Initialise the power ups
	initAsteroids();
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
		//NUM_LASERS_TO_FIRE = 3;																				// Start with 3 lasers easy, 2 medium, 1 hard
		m_fireRate = 200;																					// Set the players weapon fire rate to a shorter time interval
		m_powerUpDuration = 5.0f;																			// Easy Difficulty: Power up remains on screen longer time
	}
	else if (Game::Instance()->getDifficulty() == HARD) {
		//NUM_LASERS_TO_FIRE = 1;																				// Less laser beams initially
		m_fireRate = 400;																					// Set the players weapon fire rate to a longer time interval
		m_powerUpDuration = 2.0f;																			// Hard Difficulty: Power up remains on screen less time
	}
	CCLOG("Level %d: Player Laser Beams Initialised", Game::Instance()->getLevel());
}

void Level::initEnemyShips() {
	m_enemyShipList = new Vector<EnemyShip*>(3);
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		EnemyShip* enemyShip1 = EnemyShip::create(visibleSize);
		//EnemyShipWilKnot* enemyShip1 = EnemyShipWilKnot::create(visibleSize);								// Test New Enemies
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
		//CCLOG("Add Enemy ship at array index %d", i);
	}
	//CCLOG("Level %d: Enemy Ships Initialised", Game::Instance()->getLevel());
}

void Level::initAsteroids() {
	// Asteroids
	m_asteroidsList = new Vector<Sprite*>(MAX_NUM_ASTEROIDS_L1);											// List of asteroids
	for (int i = 0; i < MAX_NUM_ASTEROIDS_L1; ++i) {
		cocos2d::Sprite* asteroid = Sprite::createWithSpriteFrameName(ASTEROID_IMG);						// Asteroid sprite, JOR replaced auto specifier
		asteroid->setVisible(false);
		asteroid->setScale((visibleSize.height == 720) ? 1.0f : 1.5f);										// Increase scale of Asteroid for Android (My phone anyway)
		m_batchNode->addChild(asteroid);
		m_asteroidsList->pushBack(asteroid);
	}
	//CCLOG("Level %d: Asteroids Initialised", Game::Instance()->getLevel());
}

void Level::initLasers() {
	// Player Ship Lasers:
	m_playerLaserList = new Vector<Sprite*>(NUM_LASERS);													// List of lasers
	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* shipLaser = Sprite::createWithSpriteFrameName(LASER_BLUE_IMG);				// Laser sprite, JOR replaced auto specifier
		shipLaser->setVisible(false);
		m_batchNode->addChild(shipLaser);
		m_playerLaserList->pushBack(shipLaser);
	}
	//CCLOG("TEST Level::initLasers() Ship Lasers: %d", m_playerLaserList->size());

	// Enemy Lasers
	m_enemyLaserList = new Vector<Sprite*>(NUM_LASERS);														// List of lasers

	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* enemyLaser = Sprite::createWithSpriteFrameName(LASER_BLUE_IMG);				// Laser sprite, JOR replaced auto specifier
		enemyLaser->setVisible(false);
		this->addChild(enemyLaser);
		m_enemyLaserList->pushBack(enemyLaser);
	}
	//CCLOG("TEST Level::initLasers() Enemy Lasers: %d", m_enemyLaserList->size());
	//CCLOG("Level %d: Enemy Ships Initialised", Game::Instance()->getLevel());
}

void Level::initPowerUps() {
	// Power Up
	curTimeInit = Game::Instance()->getTimeTick();															// Current game time // Time to finish game	
	float lastSpawnTime = Game::Instance()->getLevelDuration() - 5000.0f;
	powerUpTime = curTimeInit + randomValueBetween(2000.0f, lastSpawnTime);									// -5000 (5 secs before end Don't spawn it when the player has no chance of getting it	
	CCLOG("Level: Power Up Spawn Time: %f", (powerUpTime - curTimeInit) / 1000.0f);

	powerUpY = randomValueBetween(0.1f, 0.8f);																// Random Y position for asteroid
	m_powerUpLife = Sprite::Sprite::create(POWER_UP_LIFE_IMG);
	m_powerUpLife->setVisible(false);
	m_powerUpLife->setPosition(visibleSize.width + m_powerUpLife->getContentSize().width, 
		visibleSize.height * powerUpY);
	this->addChild(m_powerUpLife);

	//CCLOG("Level %d: New Life Power Up Initialised", Game::Instance()->getLevel());
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
	
	player->update();																						// Update player sprite position
	
	for (EnemyShip* enemyShip : *m_enemyShipList) {
		enemyShip->update(curTimeMillis);																	// Fire Enemy Weapons etc.
	}

	if (Game::Instance()->musicPlayerVisible()) mplayer->update();											// Update the music player

	newHUD->update(curTimeMillis);																			// Update the HUD
}

void Level::spawnObjects(float curTimeMillis) {	
	//powerUpTime = 2000;	// test
	if (!spawned && curTimeMillis > powerUpTime) {		
		m_powerUpLife->setVisible(true);

		auto actionpowerUp = MoveTo::create(m_powerUpDuration, 
			Point(0 - m_powerUpLife->getContentSize().width, visibleSize.height * powerUpY));
		m_powerUpLife->runAction(actionpowerUp);

		auto rotate = RotateBy::create(2.5f, -360.0f); 
		auto sequence = cocos2d::Sequence::create(rotate, rotate, nullptr);
		m_powerUpLife->runAction(sequence);
		/*
		powerUpLife->runAction(
			Sequence::create(MoveBy::create(0.5f, Point(0 - powerUpLife->getContentSize().width, powerUpY)), // change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)
		);
		*/

		CCLOG("Spawn New Life Power Up");
		spawned = true;
	}
	
	if (curTimeMillis > _nextAsteroidSpawn) {
		float randMillisecs = randomValueBetween(0.20F, 1.0F) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;

		float randY = randomValueBetween(0.0F, winSize.height);												// Random Y position for asteroid
		float randDuration = randomValueBetween(2.0F, 10.0F);

		Sprite *asteroid = m_asteroidsList->at(_nextAsteroid);
		_nextAsteroid++;																					// Increment the asteroid

		if (_nextAsteroid >= m_asteroidsList->size()) _nextAsteroid = 0;									// Loop back around to start of asteroids list
		
		Game::Instance()->incrementAsteroidCount();

		asteroid->stopAllActions();																			// CCNode.cpp
		asteroid->setPosition(winSize.width + asteroid->getContentSize().width / 2, randY);
		asteroid->setVisible(true);
		asteroid->runAction(Sequence::create(
				MoveBy::create(randDuration, Point(-winSize.width - asteroid->getContentSize().width, 0)),
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)							// DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
		);
		//CCLOG("Spawn Asteroid");
	}
}

void Level::spawnEnemyShips(float curTimeMillis) {
	if (m_enemyLaserList->size() > 0) {
		if (curTimeMillis > nextEnemyShipSpawnTime) {
			float randMillisecs = randomValueBetween(0.20F, 1.0F) * 2500;
			nextEnemyShipSpawnTime = randMillisecs + curTimeMillis;												// Set the time to spawn the next ship

			float randY = randomValueBetween(0.0F, winSize.height);												// Random Y position for enemy ship
			float randDuration = randomValueBetween(2.0F, 10.0F);

			EnemyShip *enemyShip = m_enemyShipList->at(nextEnemyShip);
			nextEnemyShip++;																					// Increment the enemy ship on the list

			Game::Instance()->incrementEnemyShipCount();

			if (nextEnemyShip >= m_enemyShipList->size()) nextEnemyShip = 0;									// Loop back around to start of enemy ship list

			enemyShip->stopAllActions();																		// CCNode.cpp
			enemyShip->setPosition(winSize.width + enemyShip->getContentSize().width / 2, randY);
			enemyShip->setVisible(true);
			enemyShip->setLives(3);

			// Move the ship to the players coordinate
			auto action = MoveTo::create(3, Point(player->getPositionX(), player->getPositionY()));
			enemyShip->runAction(action);

			enemyShip->runAction(
				Sequence::create(
					MoveBy::create(randDuration, Point(-winSize.width - enemyShip->getContentSize().width, 0)),
					CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
					NULL)	// TERMINATE WITH NULL
			);

			CCLOG("Spawn Enemy Ship");
		}
	}
}

void Level::spawnEnemyLaserAngled(cocos2d::Point a, cocos2d::Point b, float angle) {
	cocos2d::Sprite* enemyLaser = m_enemyLaserList->at(m_nextEnemyLaser);
	m_nextEnemyLaser++;
	if (m_nextEnemyLaser >= m_enemyLaserList->size()) m_nextEnemyLaser = 0;	
	if (enemyLaser->isVisible()) return;

	Audio::Instance()->laserFXEnemy();

	enemyLaser->setPosition(a.x, a.y);
	enemyLaser->setRotation(angle);										// Rotate to face direction it is moving
	enemyLaser->setVisible(true);										// Make visible on screen
	enemyLaser->stopAllActions();

	// Move the ship to the players coordinate
	auto action = MoveTo::create(0.5f, Point(b.x, b.y));				// set to off screen the width of the laser
	enemyLaser->runAction(action);
}

void Level::spawnEnemyLaser(cocos2d::Point pos) {
	cocos2d::Sprite* enemyLaser = m_enemyLaserList->at(m_nextEnemyLaser);
	m_nextEnemyLaser++;
		
	if (m_nextEnemyLaser >= m_enemyLaserList->size())
		m_nextEnemyLaser = 0;

	if (enemyLaser->isVisible()) return;
	Audio::Instance()->laserFXEnemy();

	//CCLOG("Enemy Laser: Set Position");
	enemyLaser->setPosition(pos.x, pos.y);
	//CCLOG("Enemy Laser: Set Visible");
	enemyLaser->setVisible(true);
	//CCLOG("Enemy Laser: Stop Actions");
	enemyLaser->stopAllActions();

	//CCLOG("Enemy Laser: MoveTo");
	// Move the ship to the players coordinate
	//auto action = MoveTo::create(0.5f, Point(0 - enemyLaser->getContentSize().width, pos.y));									// set to off screen the width of the laser
	auto action = MoveTo::create(0.5f, Point(pos.x - visibleSize.width - enemyLaser->getContentSize().width, pos.y));				// set to off screen the width of the laser + the screen width

	float distance = pos.x - visibleSize.width;

	enemyLaser->runAction(action);
}

/* 
	20180221 Up to 4 different types of laser, with different spawn points and rotations
*/
void Level::spawnLasers(int amount) {
	if (curTimeMillis > m_nextFire) {
		Audio::Instance()->laserFX();																		// Play the laser fire effect

		int yVal = 0, yPos = 0, rot = 0;																	// y value for point to aim for, y position of laser spawn point, rotation of laser	

		for (int i = 0; i < amount; i++) {
			cocos2d::Sprite* shipLaser = m_playerLaserList->at(_nextShipLaser++);							// Next laser in the list, JOR replaced auto specifier
			if (_nextShipLaser >= m_playerLaserList->size())
				_nextShipLaser = 0;																			// Reset laser list index to 0 (go back to start of list)

			// Set laser spawn points
			if (amount < 4)																					// If the number of lasers to fire is 4
				(i == 0) ? yPos = 0 : (i == 1) ? yPos = -12 : yPos = 12;									// 0 = midd
			else
				(i == 0) ? yPos = 8 : (i == 1) ? yPos = -12 : (i == 2) ? yPos = 12 : yPos = -8;				// 0. = 5, 1. = 12, 2. = 12, 3. = -5

			// Set the initial rotation of the lasers
			shipLaser->setPosition(player->getPosition() + Point(shipLaser->getContentSize().width/2,yPos));
			if (amount == 2) (i == 0) ? rot = -5 : rot = 5;													// Top, bottom lasers
			if (amount == 3) (i == 0) ? rot = 0 : (i == 1) ? rot = 5 : rot = -5;							// Middle, bottom, top lasers
			if (amount == 4) (i == 0) ? rot = -3 : (i == 1) ? rot = 5 : (i == 2) ? rot = -5 : rot = 3;		// laser 1: i = 2 (5), laser 2: i = 0 (3), laser 3: i = ,laser 4: i = 1 (-5)
			shipLaser->setRotation(rot);

			shipLaser->setVisible(true);
			shipLaser->stopAllActions();

			// Where to fire the lasers
			if (amount == 2) (i == 0) ? yVal = 60 : yVal = -60;													// if 2 lasers, first one goes up, second goes down
			if (amount == 3) (i == 1) ? yVal = -100 : (i == 2) ? yVal = 100 : yVal = 0;							// if 3 lasers, first goes straight, second goes down, third goes up
			if (amount == 4) (i == 1) ? yVal = -120 : (i == 2) ? yVal = 120 : (i == 3) ? yVal = -40 : yVal = 40;// if 3 lasers, first goes straight, second goes down, third goes up

			shipLaser->runAction(
				Sequence::create(MoveBy::create(0.5, Point(winSize.width, yVal)),							// change to plus 100 for up - 100 for down
					CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
		}

		m_nextFire = curTimeMillis + m_fireRate;															// Set the next time to fire
	}
}

void Level::getInput() {
	// Android DPad (maybe change to returning a point (0,0)(1,0)(0,1),(-1,0),(0,-1)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {				// If the platform is mobile
		if (controller->getButton(8)->isSelected()) {														// Up arrow pressed
			player->moveUp();
			//CCLOG("Down button is pressed!");
		}
		else if (controller->getButton(2)->isSelected()) {													// Down arrow pressed
			player->moveDown();
			//CCLOG("Down button is pressed!");
		}
		if (controller->getButton(4)->isSelected()) {														// Left arrow pressed
			player->moveLeft();
			//CCLOG("Down button is pressed!");
		}
		else if (controller->getButton(6)->isSelected()) {													// Right arrow pressed
			player->moveRight();
			//CCLOG("Down button is pressed!");
		}

		if (controller->getButton(10)->isSelected()) {														// Up arrow pressed
			//spawnLasers(NUM_LASERS_TO_FIRE);
			spawnLasers(player->getWeaponStrength());
			//CCLOG("Button A is pressed!");
		}
		else if (controller->getButton(11)->isSelected()) {													// Down arrow pressed
			//spawnLasers(NUM_LASERS_TO_FIRE);
			spawnLasers(player->getWeaponStrength());
			//CCLOG("Button B is pressed!");
		}
	}

	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE)) {
		//spawnLasers(NUM_LASERS_TO_FIRE);
		spawnLasers(player->getWeaponStrength());															// Fire a laser			
	}

	/*
	// Highlight DPAD buttons with keyboard press
	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) ||
		Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
		controller->getButton(8)->selected();
	}
	else controller->getButton(8)->unselected();

	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) ||
		Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
		controller->getButton(2)->selected();
	}
	else controller->getButton(2)->unselected();
	// Shows the button as active in Windows / Linux / Mac
	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) ||
		Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
		controller->getButton(4)->selected();
	}
	else controller->getButton(4)->unselected();
			
	if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ||
		Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
		controller->getButton(6)->selected();
	}
	else controller->getButton(6)->unselected();
	*/
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
	for (cocos2d::Sprite* enemyLaser : *m_enemyLaserList) {
		if (!(enemyLaser->isVisible())) continue;

		if (enemyLaser->getPosition().x <= -enemyLaser->getContentSize().width)							// If the laser moves off screen it's own width
			//enemyLaser->stopAllActions();
			enemyLaser->setVisible(false);																// Hide the laser
	}

	if (m_powerUpLife->isVisible()) {
		if (player->getBoundingBox().intersectsRect(m_powerUpLife->getBoundingBox())) {					// If the ship collides with an asteroid
			player->runAction(Blink::create(1.0F, 9));													// Flash the Player ship
			m_powerUpLife->setVisible(false);
			Game::Instance()->addLife();
			Audio::Instance()->powerUpFX();																// Play the power up sound effect
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
				if (enemyShip->isVisible()) Game::Instance()->incrementEnemyShipKills();				// Increment the total number of enemy ships destroyed
				//enemyShip->setVisible(false);															// Hide the asteroid
				enemyShip->setLives(enemyShip->getLives() - 1);		// DECREMENT LIVES
				//enemyShip->updateBar( (enemyShip->getLives() * 100 )/ 3.0f);			
				if (enemyShip->getLives() < 1) enemyShip->setVisible(false);
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
			//spawnLasers(NUM_LASERS_TO_FIRE);															// Fire a laser
			spawnLasers(player->getWeaponStrength());															// Fire a laser		
	}
	CCLOG("Screen Touched");
}

void Level::endScene(EndReason endReason) {
	if (Game::Instance()->isGameOver()) return;															// If already game over, skip this function
	Game::Instance()->setGameOver(true);																// Set game over
	Game::Instance()->setLivesCarried(true);															// Carry over lives to next level
	
	const int TOTAL_LIST_ELEMENTS = 9;																	// Used to vertically space menu items

	///Game::Instance()->checkHighScore();																// The game has ended, check if the current score is the high score and save it if it is

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
	
	/*
	if (Game::Instance()->getAsteroidCount() > 0) {
		cocos2d::DrawNode* healthBar = createStatusBar(
			winSize.width * 0.5f, winSize.height - ((winSize.height / TOTAL_LIST_ELEMENTS) * 3.5f),					// Position
			(visibleSize.height == 720) ? 200 : 300, (visibleSize.height == 720) ? 20 : 30,							// Dimensions
			(float)Game::Instance()->getAsteroidKills() / (float)Game::Instance()->getAsteroidCount(),				// percentage
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
	//continueLbl->setBMFontSize(10);																	// Test set font size
	// Level Progression
	if (endReason != KENDREASONLOSE) {
		levelProgression(continueLbl);
	}
	else 
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::returnToMenu, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

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
	livesList[Game::Instance()->getLives()]->setVisible(false);												// Set the lives invisible (2,1,0)
	}

	// Update the enemy ship position
	//EnemyShip->setPosition(EnemyShip->getPosition().x - 2, EnemyShip->getPosition().y);
}
*/
