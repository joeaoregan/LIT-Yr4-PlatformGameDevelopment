/*
	Level1.cpp

	Joe O'Regan
	K00203642
	02/02/2018

	The first level of the game
	Inherits from Level base class

	20180202	Added Audio class with singleton access
*/
#include "Level1.h"

Scene* Level1::createScene() {
	cocos2d::Scene* scene = Scene::create();				// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level1::create();					// 'layer' is an autorelease object, JOR replaced auto specifier  
	//Level1->setName("Level1");							// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(s_pLayerInstance);						// Add layer as a child to scene	    
    return scene;											// Return the scene
}

// on "init" you need to initialize your instance
bool Level1::init() {
	Game::Instance()->setLevel(1);							// for parallax node init

	Level::init();											// 20180221 Added Level base class

	newHUD->setLevelLabel();								// Update HUD Level text display

	if ( !Layer::init() ) { return false; }					// super init first
	
	//initLives();											// Choose the number of lives based on the game difficult
	
	Game::Instance()->resetAsteroidKills();					// Reset the number of asteroids destroyed
	Game::Instance()->resetEnemyShipKIlls();				// Reset the number of enemy ships destroyed
		
	// ParallaxNode
	this->addChild(m_backgroundNode, -1);					// Add the parallax background
	m_backgroundNode->init();								// ParallaxNodeExtras.cpp: Initialise the parallax scrolling background
	
	this->scheduleUpdate();
	
    return true;
}

/*
	Choose the number of lives based on the game difficulty
	// MOVED TO LEVEL
void Level1::initLives() {
	// Set the starting lives based on the difficulty
	if (Game::Instance()->getDifficulty() == EASY)
		Game::Instance()->setLives(5);						// Set the number of lives for the player
	else if (Game::Instance()->getDifficulty() == MEDIUM)
		Game::Instance()->setLives(3);						// Set the number of lives for the player
	else
		Game::Instance()->setLives(2);						// Set the number of lives for the player

	CCLOG("Level 2 - Init Lives: %d", Game::Instance()->getLives());
}
*/
void Level1::update(float dt) {
	Level::update(dt);										// Call base class update function	
}

void Level1::checkCollisions() {
	Level::checkCollisions();								// Call base class function
	// Check collisions with different objects in different levels
}

void Level1::endScene(EndReason endReason) {
	Level::endScene(endReason);								// End the scene
}