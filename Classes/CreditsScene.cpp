/*
	CreditsScene.cpp

	Joe O'Regan
	K00203642
	06/03/2018

	Display the game over message for use with the game trailer
*/

#include "CreditsScene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "Game.h"
#include "Defines.h"
#include "Audio.h"

/*
	Create the scene
*/
cocos2d::Scene* CreditsScene::createScene() {
    cocos2d::Scene* scene = cocos2d::Scene::create();									// Create the scene. 'scene' is an autorelease object
        
	CreditsScene* layer = CreditsScene::create();										// Create the layer. 'layer' is an autorelease object
	
    scene->addChild(layer);																// Add layer as a child to scene
	    
    return scene;																		// Return the scene
}

/*
	Initialise the scene, displaying story and navigation buttons
*/
bool CreditsScene::init() {
    // 1. super init first
    if ( !Layer::init() ) { return false; }
	    
	m_pVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();				// Get screen resolution
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();		// Get screen origin

	// Display the game title image
	m_pGameTitleImg = cocos2d::Sprite::create("GameTitle.png");							// Game Title image
	m_pGameTitleImg->setPosition(cocos2d::Point(m_pVisibleSize.width * 0.1f,
		m_pVisibleSize.height / 2 + origin.y));											// Set position on screen
	m_pGameTitleImg->setPosition(cocos2d::Point(m_pVisibleSize.width / 2 + origin.x, 
		m_pVisibleSize.height * 0.9f + origin.y));										// Set position on screen
	m_pGameTitleImg->setScale(1.75 * (m_pVisibleSize.height == 1080) ? 1.0f : 0.67f);	// Set the scale of the title image based on the screen resolution
	this->addChild(m_pGameTitleImg);													// Add to layer		
	
	// Game Over label text
	m_pLevelLbl = cocos2d::Label::createWithTTF("Credits",
		"fonts/Super Mario Bros..ttf", m_pVisibleSize.height * 0.075);
	m_pLevelLbl->setPosition(m_pVisibleSize.width / 2, m_pVisibleSize.height * 0.75f);
	m_pLevelLbl->setColor(cocos2d::Color3B::RED);
	this->addChild(m_pLevelLbl);

	// Story lable text
	creditLbl = cocos2d::Label::createWithTTF(message, "fonts/Super Mario Bros..ttf",									// Display story decided by level number				
		m_pVisibleSize.height * 0.05);
	creditLbl->setPosition(m_pVisibleSize.width / 2, m_pVisibleSize.height / 2);
	creditLbl->setColor(cocos2d::Color3B::GRAY);																		// Use gray text
	this->addChild(creditLbl);

	auto fadeIn = FadeIn::create(1);
	auto fadeOut = FadeOut::create(1);

	auto func1 = CallFunc::create(CC_CALLBACK_0(CreditsScene::credit1, this));
	auto func2 = CallFunc::create(CC_CALLBACK_0(CreditsScene::credit2, this));
	auto func3 = CallFunc::create(CC_CALLBACK_0(CreditsScene::credit3, this));
	auto func4 = CallFunc::create(CC_CALLBACK_0(CreditsScene::credit4, this));

	auto delay = DelayTime::create(2.5f);

	auto sequence = Sequence::create(func1, delay, func2, delay, func3, delay, func4, NULL);
	//auto sequence = Sequence::create(func1, fadeOut, NULL);
	runAction(sequence);

	this->scheduleOnce(schedule_selector(CreditsScene::returnToMenu), DISPLAY_TIME_SPLASH_STORY);						// Automatically return to main menu after 10 seconds

	// Return to main menu button
	m_pReturnToMenuLbl = cocos2d::Label::createWithTTF(" Return\nTo Menu",												// Back to menu button label
		"fonts/Super Mario Bros..ttf", m_pVisibleSize.height * 0.05);
	m_pReturnToMenuLbl->setColor(cocos2d::Color3B::RED);

	m_pReturnToMenuItem = MenuItemLabel::create(m_pReturnToMenuLbl, CC_CALLBACK_1(CreditsScene::backToMenu, this));		// Go Back To Main Menu menu option
	m_pReturnToMenuItem->setPosition(m_pVisibleSize.width * 0.5f, m_pVisibleSize.height * 0.1f);
	
	// Menu items
	m_menu = Menu::create(m_pReturnToMenuItem, NULL);																	// Create the menu options
	m_menu->setPosition(Point::ZERO);
	this->addChild(m_menu);
			
    return true;
}

void CreditsScene::returnToMenu(float dt) {
	cocos2d::Scene* scene = MainMenu::createScene();																	// Create the Level4 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}

/*
	Restart the game from level 1
*/
void CreditsScene::restartGame(cocos2d::Ref* pSender) {
	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionZoomFlipX::create(0.5, Level1::createScene()));	// Restart the current scene	
	this->scheduleUpdate();																								// reschedule
}

void CreditsScene::backToMenu(cocos2d::Ref* pSender) {
	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionZoomFlipX::create(0.5, MainMenu::createScene()));	// Restart the current scene	
	this->scheduleUpdate();																								// reschedule
}

void CreditsScene::credit1() {
	//creditLbl->setOpacity(0);
	creditLbl->setString(" A Game By\nJoe O'Regan");
	creditLbl->setScale(2.5f);
	creditLbl->runAction(cocos2d::ScaleTo::create(1.0F, 1.0F));
}
void CreditsScene::credit2() {
	creditLbl->setString(" Story By\nJoe O'Regan");
	creditLbl->setScale(2.5f);
	creditLbl->runAction(cocos2d::ScaleTo::create(1.0F, 1.0F));
}
void CreditsScene::credit3() {
	creditLbl->setString("  Music By\nJoe O'Regan");
	creditLbl->setScale(2.5f);
	creditLbl->runAction(cocos2d::ScaleTo::create(1.0F, 1.0F));
}
void CreditsScene::credit4() {
	creditLbl->setString("Everything By\n Joe O'Regan");
	creditLbl->setScale(2.5f);
	creditLbl->runAction(cocos2d::ScaleTo::create(1.0F, 1.0F));
}