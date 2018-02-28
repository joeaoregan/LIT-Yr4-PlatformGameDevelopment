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

Scene* Level3::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier   
	layerInstance = Level3::create();			// 'layer' is an autorelease object, JOR replaced auto specifier  
	//layer->setName("Level3");					// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
	scene->addChild(layerInstance);						// Add layer as a child to scene	    
	return scene;								// Return the scene
}

/*
Initialisation specific to Level 2
*/
bool Level3::init() {
	Level::init();								// 20180221 Added Level base class

	Game::Instance()->setLevel(3);				// Specific to level 2
	newHUD->setLevelLabel();					// Update HUD Level text display

	if (!Layer::init()) { return false; }		// super init first

	// ParallaxNode
	this->addChild(_backgroundNode, -1);		// Add the parallax background
	_backgroundNode->init();					// Initialise the parallax scrolling background

	this->scheduleUpdate();						// Start updating the scene

	return true;
}

void Level3::update(float dt) {
	Level::update(dt);							// Call base class update function		
}

void Level3::checkCollisions() {
	Level::checkCollisions();					// Call base class function
												// Check collisions with different objects in different levels
}

void Level3::endScene(EndReason endReason) {
	Level::endScene(endReason);					// End the scene
}
