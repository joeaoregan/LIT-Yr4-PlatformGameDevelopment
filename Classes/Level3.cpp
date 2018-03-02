/*
	Level3.cpp

	Joe O'Regan
	K00203642
	18/02/2018

	Added additional level
*/
#include "Level.h"
#include "Level3.h"
#include "Input.h"
#include "EnemyShipKling.h"
#include "EnemyShipWilKnot.h"

Scene* Level3::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level3::create();			// 'layer' is an autorelease object, JOR replaced auto specifier  
	//layer->setName("Level3");					// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
	scene->addChild(s_pLayerInstance);						// Add layer as a child to scene	    
	return scene;								// Return the scene
}

/*
	Initialisation specific to Level 3
*/
bool Level3::init() {
	Level::init();								// 20180221 Added Level base class

	Game::Instance()->setGameOver(false);																	// Needed for starting new level, or restarting game

	Game::Instance()->setLevel(3);				// Specific to level 2
	newHUD->setLevelLabel();					// Update HUD Level text display

	if (!Layer::init()) { return false; }		// super init first

	CCLOG("Clear enemy ship vector");

	initEnemyShips();							// Add the new enemy with double lasers to the list of enemies

	Game::Instance()->resetAsteroidKills();		// Reset the number of asteroids destroyed
	Game::Instance()->resetEnemyShipKIlls();	// Reset the number of enemy ships destroyed


	// ParallaxNode
	this->addChild(m_backgroundNode, -1);		// Add the parallax background
	m_backgroundNode->init();					// Initialise the parallax scrolling background

	m_enemyLaserList3 = new Vector<Sprite*>(NUM_LASERS);					// List of lasers
	for (int i = 0; i < NUM_LASERS; ++i) {
		cocos2d::Sprite* enemyLaser = Sprite::create(LASER_GREEN_IMG);		// Laser sprite, JOR replaced auto specifier
		enemyLaser->setVisible(false);
		this->addChild(enemyLaser);
		m_enemyLaserList3->pushBack(enemyLaser);
	}
	
	this->scheduleUpdate();						// Start updating the scene

	return true;
}

void Level3::initEnemyShips() {
	//m_enemyLaserList->clear();
	for (unsigned int i = 0; i <= m_enemyShipList->size()+1; ++i) {
		m_enemyShipList->popBack();
		CCLOG("Remove Enemy ship %d / %d from list", i, m_enemyShipList->size());
	}

	// Add 2 x Enemy Ship Type 2 - These will appear first on screen
	for (unsigned int i = 0; i < L3_NUM_ENEMY_2; ++i) {
		EnemyShip* enemyShip2 = EnemyShipKling::create(visibleSize);
		this->addChild(enemyShip2);
		m_enemyShipList->pushBack(enemyShip2);
	}

	// Add 1 x Enemy Ship Type 1 - This will appear second
	for (unsigned int i = 0; i < L3_NUM_ENEMY_1; ++i) {
		EnemyShip* enemyShip1 = EnemyShip::create(visibleSize);
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
	}

	// Add 3 x Enemy Ship Type 3
	for (unsigned int i = 0; i < L3_NUM_ENEMY_3; ++i) {
		EnemyShip* enemyShip3 = EnemyShipWilKnot::create(visibleSize);
		this->addChild(enemyShip3);
		m_enemyShipList->pushBack(enemyShip3);
	}

	CCLOG("Add enemy ships");
}

void Level3::update(float dt) {
	Level::update(dt);													// Call base class update function		
}

void Level3::checkCollisions() {
	Level::checkCollisions();											// Call base class function

	// Check collisions with different objects in different levels

	// Check collisions for new type of laser
	for (cocos2d::Sprite* enemyLaser : *m_enemyLaserList2) {
		if (!(enemyLaser->isVisible())) continue;

		if (enemyLaser->getPosition().x <= 0)							// If the laser moves off screen it's own width
			enemyLaser->setVisible(false);								// Hide the laser
	}
	for (cocos2d::Sprite* enemyLaser : *m_enemyLaserList3) {
		if (!(enemyLaser->isVisible())) continue;

		if (enemyLaser->getPosition().x <= 0)							// If the laser moves off screen it's own width
			enemyLaser->setVisible(false);								// Hide the laser
	}
}

void Level3::endScene(EndReason endReason) {
	Level::endScene(endReason);					// End the scene
}
