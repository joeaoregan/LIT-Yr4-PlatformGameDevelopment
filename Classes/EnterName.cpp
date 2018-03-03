/*
	EnterName.cpp

	Joe O'Regan
	K00203642

	Enter the players name
*/

#include "Game.h"
#include "EnterName.h"
//#include <UIText.h>

Scene* EnterName::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	m_pLayer = EnterName::create();				// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(m_pLayer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool EnterName::init() {
	MenuScene::init();																	// Initialise base class

	m_pTitleSprite->setTexture("EnterName.png");											// Change the title image text
		
	// Instructions (MarkerFelt.ttf Super Mario Bros..ttf) 
	//instructionLbl = LabelTTF::create("Enter up to 12 characters", 
	//	"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05f);						// Label to display current high score	
	m_pInstructionLbl = cocos2d::Label::createWithTTF("Enter up to 12 characters",
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05f);
	m_pInstructionLbl->setPosition(Point(m_visibleSize.width * 0.5 + m_origin.x, 
		m_visibleSize.height * 0.6f + m_origin.y));											// Set its position on screen
	m_pInstructionLbl->setColor(Color3B::WHITE);										// Set the text colour
	this->addChild(m_pInstructionLbl);													// Add it to the layer
	
	// Enter name
	m_pTxtField = ui::TextField::create("Enter Name Here",								// Text field
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05f);
	m_pTxtField->setMaxLength(12);														// Max username size
	m_pTxtField->setMaxLengthEnabled(true);												// Enable text field maximum length
	
	m_pTxtField->setPosition(Point(m_visibleSize.width * 0.5 + m_origin.x, 
		m_visibleSize.height * 0.4f + m_origin.y));

	m_pTxtField->addEventListener(CC_CALLBACK_2(EnterName::textFieldEvent, this) );

	this->addChild(m_pTxtField);
		
	return true;
}

/*
	Update the text field
*/
void EnterName::textFieldEvent(cocos2d::Ref* sender, ui::TextField::EventType type) {
	cocos2d::ui::TextField* textField = dynamic_cast<ui::TextField *>(sender);

	m_pDef = UserDefault::getInstance();												// Used to store the current player name

	switch (type) {
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
		//CCLOG("Started typing");
		break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		//CCLOG("Text input has stopped");
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		CCLOG("%s", textField->getString().c_str());									// Display the text
		Game::Instance()->setPlayerName(m_pTxtField->getString().c_str());				// Store the player name in the Game class
		m_pDef->setStringForKey("CurrentPlayer", m_pTxtField->getString().c_str());		// Store the current players name
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		log("%s", textField->getString().c_str());
		break;
	}
}
