/*
	Level4.cpp

	Joe O'Regan
	K00203642
	18/02/2018

	Added additional level
*/
#include "Level.h"
#include "Level4.h"
#include "Input.h"
//#include "EnemyShipDerpStar.h"

Scene* Level4::createScene() {
	cocos2d::Scene* scene = Scene::create();			// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level4::create();				// 'layer' is an autorelease object, JOR replaced auto specifier  
	//layer->setName("Level4");							// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
	scene->addChild(s_pLayerInstance);					// Add layer as a child to scene	    
	return scene;										// Return the scene
}

/*
Initialisation specific to Level 4
*/
bool Level4::init() {
	Level::init();										// 20180221 Added Level base class

	Game::Instance()->setLevel(4);						// Specific to level 2
	newHUD->setLevelLabel();							// Update HUD Level text display

	if (!Layer::init()) { return false; }				// super init first
	
	// ParallaxNode
	this->addChild(m_backgroundNode, -1);				// Add the parallax background
	m_backgroundNode->init();							// Initialise the parallax scrolling background

	derpStar = EnemyShipDerpStar::create(visibleSize);	// Create boss enemy
	m_enemyShipList->pushBack(derpStar);
	this->addChild(derpStar);							// Add to scene

	this->scheduleUpdate();								// Start updating the scene

	return true;
}

void Level4::update(float dt) {
	Level::update(dt);									// Call base class update function		
}

void Level4::checkCollisions() {
	Level::checkCollisions();							// Call base class function
	// Check collisions with different objects in different levels
}

void Level4::endScene(EndReason endReason) {
	Level::endScene(endReason);							// End the scene
}
