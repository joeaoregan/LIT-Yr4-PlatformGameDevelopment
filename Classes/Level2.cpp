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
	s_pLayerInstance = Level2::create();							// 'layer' is an autorelease object, JOR replaced auto specifier  
	//layer->setName("Level2");										// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(s_pLayerInstance);								// Add layer as a child to scene	    
    return scene;													// Return the scene
}

/*
	Initialisation specific to Level 2
*/
bool Level2::init() {
	Level::init();													// 20180221 Added Level base class

	Game::Instance()->setLevel(2);									// for parallax node init
	m_pHUD->setLevelLabel();										// Update HUD Level text display

	if (!Layer::init()) { return false; }							// super init first

	// 1) Create the ParallaxNode
	this->addChild(m_backgroundNode, -1);							// Add the parallax background
	m_backgroundNode->init();										// Initialise the parallax scrolling background
	
	Game::Instance()->resetAsteroidKills();							// Reset the number of asteroids destroyed
	Game::Instance()->resetEnemyShipKIlls();						// Reset the number of enemy ships destroyed
	
	CCLOG("Level %d: Background Initialised", 
		Game::Instance()->getLevel());
	
	this->scheduleUpdate();											// Start updating the scene

	CCLOG("Level %d: Initialised", 
		Game::Instance()->getLevel());

    return true;
}

/*
	Initialise the new enemies for level 2
*/
void Level2::initEnemyShips() {
	//CCLOG("LEVEL initEnemyShips() - Level %d: Enemy Ships Initialised", Game::Instance()->getLevel());	
	for (unsigned int i = 0; i < L2_NUM_ENEMY_2; ++i) {
		EnemyShip* enemyShip2 = EnemyShipKling::create(m_visibleSize);	// Second type of enemy ship, spawned first
		this->addChild(enemyShip2);
		m_enemyShipList->pushBack(enemyShip2);
		//CCLOG("LEVEL 2 - Add New Enemy ship at array index %d", i);
	}

	// First type of enemy ship
	for (unsigned int i = 0; i < L2_NUM_ENEMY_1; ++i) {
		EnemyShip* enemyShip1 = EnemyShip::create(m_visibleSize);		// First type of enemy appears second
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
	}
}

/*
	Update Level 2 scene
*/
void Level2::update(float dt) {
	Level::update(dt);													// Call base class update function		
}

/*
	Check collisions for level 2 ships and lasers NOT FULLY IMPLEMENTED - TESTING
*/
void Level2::checkCollisions() {
	Level::checkCollisions();											// Call base class function
	// Check collisions with different objects in different levels

	// Check collisions for new type of laser
	for (cocos2d::Sprite* enemyLaser : *m_enemyLaserList2) {
		if (!(enemyLaser->isVisible())) continue;

		if (enemyLaser->getPosition().x <= 0)							// If the laser moves off screen it's own width
			enemyLaser->setVisible(false);								// Hide the laser
	}
}

void Level2::endScene(EndReason endReason) {
	Level::endScene(endReason);											// End the scene
}
