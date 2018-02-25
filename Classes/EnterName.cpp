/*
	EnterName.cpp

	Joe O'Regan
	K00203642

	Enter the players name
*/

#include "Game.h"
#include "EnterName.h"
//#include <UIText.h>

#define TRANSITION_TIME 0.5

Scene* EnterName::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	layer = EnterName::create();				// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool EnterName::init() {
	MenuScene::init();																	// Initialise base class

	titleSprite->setTexture("EnterName.png");											// Change the title image text
		
	// Instructions (MarkerFelt.ttf Super Mario Bros..ttf) 
	//instructionLbl = LabelTTF::create("Enter up to 12 characters", 
	//	"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05f);						// Label to display current high score	
	instructionLbl = cocos2d::Label::createWithTTF("Enter up to 12 characters",
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05f);
	instructionLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, 
		visibleSize.height * 0.6f + origin.y));											// Set its position on screen
	instructionLbl->setColor(Color3B::WHITE);											// Set the text colour
	this->addChild(instructionLbl);														// Add it to the layer
	
	// Enter name
	txtField = ui::TextField::create("Enter Name Here", 
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05f);
	txtField->setMaxLength(12);	// Max username size
	txtField->setMaxLengthEnabled(true);

	//txtField->setPosition(Vec2(300, 400));
	txtField->setPosition(Point(visibleSize.width * 0.5 + origin.x, 
		visibleSize.height * 0.4f + origin.y));

	txtField->addEventListener(CC_CALLBACK_2(EnterName::textFieldEvent, this) );

	this->addChild(txtField);

	//Game::Instance()->setPlayerName(txtField->getString().c_str());
	
	return true;
}

void EnterName::textFieldEvent(cocos2d::Ref* sender, ui::TextField::EventType type) {
	cocos2d::ui::TextField* textField = dynamic_cast<ui::TextField *>(sender);

	def = UserDefault::getInstance();													// Used to store the current player name

	switch (type) {
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
		//log("Start typing");
		break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		//log("Text has stopped");
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		log("%s", textField->getString().c_str());
		Game::Instance()->setPlayerName(txtField->getString().c_str());
		def->setStringForKey("CurrentPlayer", txtField->getString().c_str());			// Store the current players name
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		log("%s", textField->getString().c_str());
		break;
	}
}
