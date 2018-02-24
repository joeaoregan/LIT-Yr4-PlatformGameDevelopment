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
#include "Level2.h"
#include "MainMenu.h"
#include "DPad.h"
#include <string>
#include <sstream>
#include "Input.h"


Scene* Level1::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier   
	layerInstance = Level1::create();			// 'layer' is an autorelease object, JOR replaced auto specifier  
	//Level1->setName("Level1");				// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(layerInstance);				// Add layer as a child to scene	    
    return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool Level1::init() {
	Level::init();																						// 20180221 Added Level base class

	if ( !Layer::init() ) { return false; }																// super init first

	Game::Instance()->setLevel(1);																		// for parallax node init

	Game::Instance()->setLives(3);																		// JOR set the number of lives for the player
	
	// 1) Create the ParallaxNode
	//_backgroundNode = ParallaxNodeExtras::create();
	this->addChild(_backgroundNode, -1);
	_backgroundNode->init();																			// ParallaxNodeExtras.cpp: Initialise the parallax scrolling background
	
	/*
	//controller->init(this);																									// NOT WORKING ANDROID: D-pad (Display on mobile device)
	//DPad::Instance()->init(this);																								// NOT WORKING ANDROID: D-pad (Display on mobile device)
	controller = DPad::Instance()->create("DPad/Base300.png", "DPad/Arrow96.png", "DPad/Arrow96Pressed.png", Point(250, 250));
	this->addChild(controller);
	*/
	
	this->scheduleUpdate();
	
    return true;
}

void Level1::update(float dt) {
	Level::update(dt);																						// Call base class update function	
}

void Level1::checkCollisions() {
	Level::checkCollisions();																				// Call base class function
	// Check collisions with different objects in different levels
}


void Level1::endScene(EndReason endReason) {
	Level::endScene(endReason);
}