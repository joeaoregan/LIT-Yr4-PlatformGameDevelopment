/*
	Joe O'Regan
	HelloWorldScene.cpp
	02/02/2018

	20180202	Added Audio class with singleton access
*/
#include "HelloWorldScene.h"
#include "Audio.h"

USING_NS_CC;

Audio* Audio::s_pInstance;				// Singleton so only one instance of Audio exists in the game, for easy access

#define KNUMASTEROIDS 15				// Number of asteroids
#define KNUMLASERS 5					// Number of lasers

Scene* HelloWorld::createScene() {    
    auto scene = Scene::create();		// 'scene' is an autorelease object        
    auto layer = HelloWorld::create();	// 'layer' is an autorelease object	    
    scene->addChild(layer);				// add layer as a child to scene	    
    return scene;						// return the scene
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    // super init first
    if ( !Layer::init() ) { return false; }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);

	//  GALAXY

	_batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");
	this->addChild(_batchNode);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");

	_ship = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
	_ship->setPosition(visibleSize.width * 0.1, visibleSize.height * 0.5);
	_batchNode->addChild(_ship, 1);

	// 1) Create the ParallaxNode
	_backgroundNode = ParallaxNodeExtras::create();
	this->addChild(_backgroundNode, -1);

	// 2) Create the sprites will be added to the ParallaxNode
	_spaceDust1 = Sprite::create("bg_front_spacedust.png");
	_spaceDust2 = Sprite::create("bg_front_spacedust.png");
	_planetSunrise = Sprite::create("bg_planetsunrise.png");
	_galaxy = Sprite::create("bg_galaxy.png");
	_spatialAnomaly1 = Sprite::create("bg_spacialanomaly.png");
	_spatialAnomaly2 = Sprite::create("bg_spacialanomaly2.png");

	// 3) Determine relative movement speeds for space dust and background
	auto dustSpeed = Point(0.1F, 0.1F);
	auto bgSpeed = Point(0.05F, 0.05F);

	// 4) Add children to ParallaxNode
	_backgroundNode->addChild(_spaceDust1, 0, dustSpeed, Point(0, visibleSize.height / 2));
	_backgroundNode->addChild(_spaceDust2, 0, dustSpeed, Point(_spaceDust1->getContentSize().width, visibleSize.height / 2));
	_backgroundNode->addChild(_galaxy, -1, bgSpeed, Point(0, visibleSize.height * 0.7));
	_backgroundNode->addChild(_planetSunrise, -1, bgSpeed, Point(600, visibleSize.height * 0));
	_backgroundNode->addChild(_spatialAnomaly1, -1, bgSpeed, Point(900, visibleSize.height * 0.3));
	_backgroundNode->addChild(_spatialAnomaly2, -1, bgSpeed, Point(1500, visibleSize.height * 0.9));

	HelloWorld::addChild(ParticleSystemQuad::create("Stars1.plist"));
	HelloWorld::addChild(ParticleSystemQuad::create("Stars2.plist"));
	HelloWorld::addChild(ParticleSystemQuad::create("Stars3.plist"));

	_asteroids = new Vector<Sprite*>(KNUMASTEROIDS);
	for (int i = 0; i < KNUMASTEROIDS; ++i) {
		auto *asteroid = Sprite::createWithSpriteFrameName("asteroid.png");
		asteroid->setVisible(false);
		_batchNode->addChild(asteroid);
		_asteroids->pushBack(asteroid);
	}

	_shipLasers = new Vector<Sprite*>(KNUMLASERS);
	for (int i = 0; i < KNUMLASERS; ++i) {
		auto shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");		// Laser sprite
		shipLaser->setVisible(false);
		_batchNode->addChild(shipLaser);
		_shipLasers->pushBack(shipLaser);
	}

	Device::setAccelerometerEnabled(true);
	auto accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(accelerationListener, this);

	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	_lives = 3;																								// Number of lives
	double curTime = getTimeTick();																			// Current game time
	_gameOverTime = curTime + 30000;																		// Time to finish game

	this->scheduleUpdate();

	Audio::Instance()->init();																				// Initialise the game audio

    return true;
}

void HelloWorld::update(float dt) {
	float curTimeMillis = getTimeTick();																	// Current game time
	winSize = Director::getInstance()->getWinSize();														// Dimensions of game screen

	scrollBackground(dt);																					// Scroll the background objects
	moveShip(dt);																							// Move the player ship
	spawnAsteroids(curTimeMillis);																			// Spawn asteroids
	checkCollisions();																						// Check have game objects collided with each other
	checkGameOver(curTimeMillis);																			// Check is the game over or not
}

void HelloWorld::scrollBackground(float dt) {
	auto backgroundScrollVert = Point(-1000, 0);
	_backgroundNode->setPosition(_backgroundNode->getPosition() + (backgroundScrollVert * dt));

	// Parallax
	auto spaceDusts = new Vector<Sprite*>(2);
	spaceDusts->pushBack(_spaceDust1);
	spaceDusts->pushBack(_spaceDust2);
	for (auto spaceDust : *spaceDusts) {
		float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
		float size = spaceDust->getContentSize().width;
		if (xPosition < -size / 2) {
			_backgroundNode->incrementOffset(Point(spaceDust->getContentSize().width * 2, 0), spaceDust);
		}
	}

	auto backGrounds = new Vector<Sprite*>(4);
	backGrounds->pushBack(_galaxy);
	backGrounds->pushBack(_planetSunrise);
	backGrounds->pushBack(_spatialAnomaly1);
	backGrounds->pushBack(_spatialAnomaly2);

	for (auto background : *backGrounds) {
		float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x;
		float size = background->getContentSize().width;
		if (xPosition < -size) {
			_backgroundNode->incrementOffset(Point(2000, 0), background);
		}
	}
}

void HelloWorld::spawnAsteroids(float curTimeMillis) {
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
				CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
				NULL /* DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)*/)
		);
	}
}

void HelloWorld::moveShip(float dt) {
	// Acceleration
	float maxY = winSize.height - _ship->getContentSize().height / 2;										// The maximum distance the ship can move down
	float minY = _ship->getContentSize().height / 2;														// The maximum distance the ship can move up
	float diff = (_shipPointsPerSecY * dt);
	float newY = _ship->getPosition().y + diff;
	newY = MIN(MAX(newY, minY), maxY);
	_ship->setPosition(_ship->getPosition().x, newY);
}

void HelloWorld::checkCollisions() {
	// Asteroids Collisions
	for (auto asteroid : *_asteroids) {
		if (!(asteroid->isVisible())) continue;

		for (auto shipLaser : *_shipLasers) {
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {
				Audio::Instance()->explodeFX();
				shipLaser->setVisible(false);
				asteroid->setVisible(false);
			}
		}

		if (_ship->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {							// If the ship collides with an asteroid
			asteroid->setVisible(false);																	// Destroy the asteroid
			_ship->runAction(Blink::create(1.0F, 9));														// Flash the Player ship
			_lives--;																						// Decrement the number of lives
		}
	}
}

void HelloWorld::checkGameOver(float currenTime) {
	if (_lives <= 0) {																						// If the player has run out of lives
		_ship->stopAllActions();																			// CCNode.cpp
		_ship->setVisible(false);																			// Destroy the ship
		this->endScene(KENDREASONLOSE);																		// Player has died
	}
	else if (currenTime >= _gameOverTime) {
		this->endScene(KENDREASONWIN);																		// Player stays playing for the length of time
	}
}

void HelloWorld::onAcceleration(Acceleration* acc, Event* event) {
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTSPERSEC (winSize.height*0.5)        
#define KMAXDIFFX 0.2

	double rollingX;

	// Cocos2DX inverts X and Y accelerometer depending on device orientation
	// in landscape mode right x=-y and y=x !!! (Strange and confusing choice)
	acc->x = acc->y;
	rollingX = (acc->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));
	float accelX = acc->x - rollingX;
	Size winSize = Director::getInstance()->getWinSize();
	float accelDiff = accelX - KRESTACCELX;
	float accelFraction = accelDiff / KMAXDIFFX;
	_shipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;
}

float HelloWorld::randomValueBetween(float low, float high) {
	// from http://stackoverflow.com/questions/686353/c-random-float-number-generation
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

float HelloWorld::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
}

void HelloWorld::setInvisible(Node * node) {
	node->setVisible(false);
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event){
	Audio::Instance()->laserFX();
	auto winSize = Director::getInstance()->getWinSize();
	auto shipLaser = _shipLasers->at(_nextShipLaser++);
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;
	shipLaser->setPosition(_ship->getPosition() + Point(shipLaser->getContentSize().width / 2, 0));
	shipLaser->setVisible(true);
	shipLaser->stopAllActions();
	shipLaser->runAction(
		Sequence::create(
		MoveBy::create(0.5, Point(winSize.width, 0)), 
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
		NULL));
}

void HelloWorld::restartTapped(Ref* pSender) {
	Director::getInstance()->replaceScene
		(TransitionZoomFlipX::create(0.5, this->createScene()));
	// reschedule
	this->scheduleUpdate();
}

void HelloWorld::endScene(EndReason endReason) {
	if (_gameOver)
		return;
	_gameOver = true;

	auto winSize = Director::getInstance()->getWinSize();
	char message[10] = "You Win";
	if (endReason == KENDREASONLOSE)
		strcpy(message, "You Lose");
	auto label = Label::createWithBMFont("Arial.fnt", message);
	label->setScale(0.1F);
	label->setPosition(winSize.width / 2, winSize.height*0.6F);
	this->addChild(label);

	strcpy(message, "Restart");
	auto restartLabel = Label::createWithBMFont("Arial.fnt", message);
	auto restartItem = MenuItemLabel::create(restartLabel, CC_CALLBACK_1(HelloWorld::restartTapped, this));
	restartItem->setScale(0.1F);
	restartItem->setPosition(winSize.width / 2, winSize.height*0.4);

	auto *menu = Menu::create(restartItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// clear label and menu
	restartItem->runAction(ScaleTo::create(0.5F, 1.0F));
	label->runAction(ScaleTo::create(0.5F, 1.0F));
	
	// Terminate update callback
	this->unscheduleUpdate();
}


void HelloWorld::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
