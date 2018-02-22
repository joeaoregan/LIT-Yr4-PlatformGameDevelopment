/*
	EnterName.cpp

	Joe O'Regan
	K00203642

	Enter the players name
*/

#include "Game.h"
#include "EnterName.h"
#include "MainMenu.h"
//#include "Level1.h"

#include "HUD.h"
// UI

//#include <UIText.h>

#define TRANSITION_TIME 0.5

Scene* EnterName::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	EnterName* layer = EnterName::create();		// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool EnterName::init() {
	if (!Layer::init()) return false;																								// Super init first

	visibleSize = Director::getInstance()->getVisibleSize();																		// Get visible screen size
	origin = Director::getInstance()->getVisibleOrigin();																			// Get screen origin point

	// Initialise the background sprite image
	initBackground();																												// Decide which background image to use, based on the platforms screen resolution
	this->addChild(backgroundSprite);																								// Add to layer

	// High Scores Title
	titleSprite = Sprite::create("EnterName.png");																					// Title image
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - titleSprite->getContentSize().height));	// Set position on screen
	this->addChild(titleSprite);																									// Add to layer							

	// Back Button
	btnBackImg = MenuItemImage::create("btnBack.png","btnBackSelect.png",CC_CALLBACK_1(EnterName::returnToMenu,this));				// Set image for menu option
	btnBackImg->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.1f + origin.y));							// Set image position
	btnBack = Menu::create(btnBackImg, NULL);																						// Menu
	btnBack->setPosition(Point::ZERO);																								// Set position on screen
	this->addChild(btnBack);																										// Add to layer
	
	// Instructions
	//MarkerFelt.ttf Super Mario Bros..ttf 
	highScoreLbl = LabelTTF::create("Enter up to 12 characters", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.05f);		// Label to display current high score	
	highScoreLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.75f + origin.y));					// Set its position on screen
	highScoreLbl->setColor(Color3B::WHITE);																							// Set the text colour
	this->addChild(highScoreLbl);																									// Add it to the layer
	
	// Enter name
	txtField = ui::TextField::create("Enter Name Here", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.05f);
	txtField->setMaxLength(12);	// Max username size
	txtField->setMaxLengthEnabled(true);

	//txtField->setPosition(Vec2(300, 400));
	txtField->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.5f + origin.y));

	txtField->addEventListener(CC_CALLBACK_2(EnterName::textFieldEvent, this) );

	this->addChild(txtField);

	//Game::Instance()->setPlayerName(txtField->getString().c_str());
	
	return true;
}


void EnterName::textFieldEvent(cocos2d::Ref* sender, ui::TextField::EventType type) {
	cocos2d::ui::TextField* textField = dynamic_cast<ui::TextField *>(sender);

	switch (type) {
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
		log("Start typing rev rev");
		break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		log("Text has stopped");
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		log("%s", textField->getString().c_str());
		Game::Instance()->setPlayerName(txtField->getString().c_str());
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		log("s", textField->getString().c_str());
		break;
	}
}

void EnterName::returnToMenu(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = MainMenu::createScene();																				// Return to the main menu

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));											// Switch scenes with animated transition
}

// Choose a background based on the screen resolution
void EnterName::initBackground() {
	// Decide which background image to use, based on the platforms screen resolution
	if (visibleSize.height == 1080)
		backgroundSprite = Sprite::create("BG1080p.png");
	else
		backgroundSprite = Sprite::create("BG720p.png");																			// Background image

	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));						// Set backgound position
}