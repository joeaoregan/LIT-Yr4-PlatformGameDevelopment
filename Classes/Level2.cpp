/*
	Joe O'Regan
	Level2.cpp
	18/02/2018

	Added additional level
*/
#include "Level.h"
#include "Level2.h"
#include "MainMenu.h"
#include <string>
#include <sstream>
#include "Input.h"

Scene* Level2::createScene() {  
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier   
	Level2* layer = Level2::create();			// 'layer' is an autorelease object, JOR replaced auto specifier  
	layer->setName("Level2");					// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(layer);						// Add layer as a child to scene	    
    return scene;								// Return the scene
}

/*
	Initialisation specific to Level 2
*/
bool Level2::init() {
	Level::init();								// 20180221 Added Level base class

    if ( !Layer::init() ) { return false; }		// super init first

	Game::Instance()->setLevel(2);				// Specific to level 2
	newHUD->setLevel(2);						// Update HUD Level text display
		
	// 1) Create the ParallaxNode
	this->addChild(_backgroundNode, -1);		// Add the parallax background
	_backgroundNode->init();					// Initialise the parallax scrolling background
	
	this->scheduleUpdate();						// Start updating the scene

    return true;
}

void Level2::update(float dt) {
	Level::update(dt);							// Call base class update function		
}

void Level2::checkCollisions() {
	Level::checkCollisions();					// Call base class function
	// Check collisions with different objects in different levels
}

void Level2::endScene(EndReason endReason) {
	Level::endScene(endReason);					// End the scene
}
