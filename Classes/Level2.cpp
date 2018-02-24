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

// on "init" you need to initialize your instance
bool Level2::init() {
	Level::init();																							// 20180221 Added Level base class

    if ( !Layer::init() ) { return false; }																	// super init first

	Game::Instance()->setLevel(2);																			// Specific to level 2
	newHUD->setLevel(2);
		
	// 1) Create the ParallaxNode
	this->addChild(_backgroundNode, -1);
	_backgroundNode->init();																				// Initialise the parallax scrolling background
	

	this->scheduleUpdate();																					// Start updating the scene

    return true;
}

void Level2::update(float dt) {
	Level::update(dt);																						// Call base class update function		
}

void Level2::checkCollisions() {
	Level::checkCollisions();																				// Call base class function
	// Check collisions with different objects in different levels
}

void Level2::endScene(EndReason endReason) {
	if (Game::Instance()->isGameOver()) return;																				// If already game over, skip this function
	Game::Instance()->setGameOver(true);																						// Set game over

	cocos2d::Size winSize = Director::getInstance()->getWinSize();															// JOR replaced auto specifier

	Game::Instance()->checkHighScore();																						// The game has ended, check if the current score is the high score and save it if it is

	// Win / Lose Message
	char message[17] = "Level 2 Complete";
	if (endReason == KENDREASONLOSE) strcpy(message, "You Lose");

	cocos2d::Label* label = Label::createWithBMFont("Arial.fnt", message);													// JOR replaced auto specifier
	if (winSize.height == 720)
		label->setScale(0.2f);
	else
		label->setScale(0.3F);																								// JOR Use larger label for 1080p

	label->setPosition(winSize.width / 2, winSize.height*0.75f);
	this->addChild(label);
	
	// Restart Level
	strcpy(message, "Restart Level");	
	cocos2d::Label* restartLbl = Label::createWithBMFont("Arial.fnt", message);												// JOR replaced auto specifier
	cocos2d::MenuItemLabel* restartItem=MenuItemLabel::create(restartLbl,CC_CALLBACK_1(Level::restartTapped,this));			// JOR replaced auto specifier
	restartItem->setScale(0.1F);
	restartItem->setPosition(winSize.width / 2, winSize.height*0.5f);

	cocos2d::Menu* menu = Menu::create(restartItem, NULL);																	// JOR replaced auto specifier
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Continue To Next Level
	if (endReason != KENDREASONLOSE) {
		strcpy(message, "Continue");
		cocos2d::Label* continueLbl = Label::createWithBMFont("Arial.fnt", message);										// JOR replaced auto specifier
		cocos2d::MenuItemLabel* continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(Level::startLevel3,this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
																															//restartItem->setScale(0.1F);
		if (winSize.height == 720)
			continueItem->setScale(0.1F);
		else
			continueItem->setScale(0.15F);																					// JOR Use larger size for 1080p
		continueItem->setPosition(winSize.width / 2, winSize.height*0.25f);

		cocos2d::Menu* menu2 = Menu::create(continueItem, NULL);															// JOR replaced auto specifier
		menu2->setPosition(Point::ZERO);
		this->addChild(menu2);

		continueItem->runAction(ScaleTo::create(0.5F, 1.0F));
	} else {
		// Return To Main Menu
		strcpy(message, "Main Menu");
		cocos2d::Label* menuLbl = Label::createWithBMFont("Arial.fnt", message);											// JOR replaced auto specifier
		cocos2d::MenuItemLabel* mainMenuItem = MenuItemLabel::create(menuLbl, CC_CALLBACK_1(Level::returnToMenu, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		if (winSize.height == 720)
			mainMenuItem->setScale(0.1F);
		else
			mainMenuItem->setScale(0.15F);																					// JOR Use larger size for 1080p
		mainMenuItem->setPosition(winSize.width / 2, winSize.height*0.25f);

		cocos2d::Menu* menu2 = Menu::create(mainMenuItem, NULL);															// JOR replaced auto specifier
		menu2->setPosition(Point::ZERO);
		this->addChild(menu2);

		mainMenuItem->runAction(ScaleTo::create(0.5F, 1.0F));
	}

	// clear label and menu
	restartItem->runAction(ScaleTo::create(0.5F, 1.0F));
	label->runAction(ScaleTo::create(0.5F, 1.0F));
		
	this->unscheduleUpdate();																								// Terminate update callback
}
