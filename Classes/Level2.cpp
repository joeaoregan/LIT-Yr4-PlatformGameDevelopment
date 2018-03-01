/*
	Joe O'Regan
	Level2.cpp
	18/02/2018

	Added additional level
*/

#include "Level2.h"
#include "EnemyShipKling.h"

Scene* Level2::createScene() {  
	cocos2d::Scene* scene = Scene::create();						// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level2::create();								// 'layer' is an autorelease object, JOR replaced auto specifier  
	//layer->setName("Level2");										// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(s_pLayerInstance);									// Add layer as a child to scene	    
    return scene;													// Return the scene
}

/*
	Initialisation specific to Level 2
*/
bool Level2::init() {
	Level::init();													// 20180221 Added Level base class

	Game::Instance()->setLevel(2);									// for parallax node init
	newHUD->setLevelLabel();										// Update HUD Level text display

	if (!Layer::init()) { return false; }							// super init first
	
	initEnemyShips();												// Add the new enemy with double lasers to the list of enemies

	// 1) Create the ParallaxNode
	this->addChild(_backgroundNode, -1);							// Add the parallax background
	_backgroundNode->init();										// Initialise the parallax scrolling background


	Game::Instance()->resetAsteroidKills();							// Reset the number of asteroids destroyed
	Game::Instance()->resetEnemyShipKIlls();						// Reset the number of enemy ships destroyed


	CCLOG("Level %d: Background Initialised", 
		Game::Instance()->getLevel());
	
	this->scheduleUpdate();											// Start updating the scene

	CCLOG("Level %d: Initialised", 
		Game::Instance()->getLevel());

    return true;
}

void Level2::initEnemyShips() {
	//EnemyShips = new Vector<EnemyShip*>(3);
	for (unsigned int i = 0; i < m_numEnemyShips; ++i) {
		EnemyShip* enemyShip2 = EnemyShipKling::create(visibleSize);
		this->addChild(enemyShip2);
		m_enemyShipList->pushBack(enemyShip2);
		//CCLOG("Add New Enemy ship at array index %d", i);
	}
}

void Level2::update(float dt) {
	Level::update(dt);												// Call base class update function		
}

void Level2::checkCollisions() {
	Level::checkCollisions();										// Call base class function
	// Check collisions with different objects in different levels
}

void Level2::endScene(EndReason endReason) {
	Level::endScene(endReason);										// End the scene
}
