/*
	Menu.cpp

	Joe O'Regan
	K00203642
	23/02/2018

	Menu base class

	Each menu scene derived from this class, will inherit the game title, scene title, and player name text
*/

#include "MainMenu.h"
#include "MenuScene.h"
#include "EnterName.h"
#include "Game.h"
#include "Input.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include <SDL.h>										// For gamepad support on desktop
#endif
//#include "HealthBar.h"

MenuScene* MenuScene::m_pLayer;

cocos2d::Scene* MenuScene::createScene() {
	cocos2d::Scene* scene = cocos2d::Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	m_pLayer = MenuScene::create();						// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(m_pLayer);							// Add layer as a child to scene
		
	return scene;																											// Return the scene
}

// on "init" you need to initialize your instance
bool MenuScene::init() {
	if (!Layer::init()) return false;																						// Super init first


	Input::Instance()->init(this, this->_eventDispatcher);																	// Ship Movement

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//	// SDL Gamepad Support for Windows
//	if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
//		//std::cout << "SDL could not initialize! SDL Error: %s\n" << SDL_GetError() << std::endl;
//		cocos2d::log("SDL init failed");
//	}									// For gamepad support on desktop
//#endif
	
	m_def = cocos2d::UserDefault::getInstance();																			// User defaults used to store/load values
	
	// Screen size
	m_visibleSize = cocos2d::Director::getInstance()->getVisibleSize();														// Get visible screen size
	m_origin = cocos2d::Director::getInstance()->getVisibleOrigin();														// Get screen origin point		
	(m_visibleSize.height == 1080) ? m_scale = 1.0f : m_scale = 0.67f;														// Set scale factor
	
	// 1. Background image
	//backgroundSprite = cocos2d::Sprite::create((visibleSize.height == 1080) ? "BG1080p.png" : "BG720p.png");				// Choose Background image depending on resolution ONLY ONE IMAGE USED NOW
	m_pBackgroundSprite = cocos2d::Sprite::create("BG1080p.png");															// Use one high res background image for efficiency
	setYPosAndScale(m_pBackgroundSprite, 0.5f);																				// Set the Y position and scale
	this->addChild(m_pBackgroundSprite);																					// Add to layer
	
	// 2. Game Title
	m_pGameTitleSprite = cocos2d::Sprite::create("GameTitle.png");															// Game Title image
	m_pGameTitleSprite->setPosition(cocos2d::Point(m_visibleSize.width * 0.1f, m_visibleSize.height / 2 + m_origin.y));		// Set position on screen
	m_pGameTitleSprite->setPosition(cocos2d::Point(m_visibleSize.width / 2 + m_origin.x, 
		m_visibleSize.height * 0.9f + m_origin.y));																			// Set position on screen
	m_pGameTitleSprite->setScale(1.75 * m_scale);																			// Set the scale
	this->addChild(m_pGameTitleSprite);																						// Add to layer		
		
	// 3. Scene Title
	m_pTitleSprite = cocos2d::Sprite::create("MainMenu.png");																// Scene Title image
	setYPosAndScale(m_pTitleSprite, 0.75f);
	this->addChild(m_pTitleSprite);																							// Add to layer							

	// 4. Show current player name. Can select to switch to Enter Name state
	m_currentPlayerName = m_def->getStringForKey("CurrentPlayer", "Player");												// Get the current Player name stored in memory
	//if (Game::Instance()->getPlayerName() == "") 
	Game::Instance()->setPlayerName(m_currentPlayerName);																	// Set the players name to a default value
	m_changeNameLbl = cocos2d::Label::createWithBMFont("Arial.fnt", "Current Player:\n" + m_currentPlayerName);				// Display current players name
	m_changeNameLbl->setScale(m_scale);

	m_pCurrentPlayerLbl = cocos2d::MenuItemLabel::create(m_changeNameLbl, CC_CALLBACK_1(MenuScene::GoToEnterName, this));	// Go to enter name scene
	m_pCurrentPlayerLbl->setPosition(cocos2d::Point((m_visibleSize.width + m_origin.x) * 0.1f, 
		m_visibleSize.height * 0.95f + m_origin.y));
	m_pCurrentPlayerLbl->runAction(cocos2d::ScaleTo::create(0.5F, 1.0F));

	// 5. Back Button
	m_pBtnBackImg = cocos2d::MenuItemImage::create("btnBack.png", "btnBackSelect.png",
		CC_CALLBACK_1(MenuScene::returnToMenu, this));	// Set image for menu option
	setYPosAndScale(m_pBtnBackImg, 0.05f);																					// Set the scale

	// 6. Exit button (autorelease object)
	m_pCloseItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(MenuScene::menuCloseCallback, this));																	// Close game button, JOR replaced auto specifier
	m_pCloseItem->setPosition(Point(m_origin.x + m_visibleSize.width - m_pCloseItem->getContentSize().width / 2,			// Close button position
		m_origin.y + m_pCloseItem->getContentSize().height / 2));

	// Menu Items
	menu = cocos2d::Menu::create(m_pCurrentPlayerLbl, m_pBtnBackImg, m_pCloseItem, NULL);									// Create the menu items, Handles menu item touches
	menu->setPosition(cocos2d::Point::ZERO);																				// Set the position at origin
	this->addChild(menu);

	return true;
}

/*
	Check for keyboard input. This is platform specific
	Virtual keyboard is displayed on Android if not checked
*/
void MenuScene::update(float dt) {
	/****************************************************************************************
	 *						KEYBOARD INPUT NOT HANDLED FOR MOBILE
	 ***************************************************************************************/
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX ||								// Stops keyboard appearing for Android
		CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
		if (Input::Instance()->isKeyPressedMenu(EventKeyboard::KeyCode::KEY_UP_ARROW) ||									// Up arrow key
			Input::Instance()->isKeyPressedMenu(EventKeyboard::KeyCode::KEY_W)) {											// or W key
			nextButton();																									// Select next menu option (up the list)
			//CCLOG("next button: %d", m_currentBtn);
		}
		else if (Input::Instance()->isKeyPressedMenu(EventKeyboard::KeyCode::KEY_DOWN_ARROW) ||								// Down arrow key
			Input::Instance()->isKeyPressedMenu(EventKeyboard::KeyCode::KEY_S)) {											// or S key
			prevButton();																									// Select previous menu option (down the list)
			//CCLOG("prev button: %d", m_currentBtn);
		}
	}
}

/*
	Set Y position and scale for Sprites and Menu items in the scene
*/
void MenuScene::setYPosAndScale(cocos2d::Sprite* sprite, float y) {
	sprite->setScale(m_scale);																								// Set the scale
	sprite->setPosition(cocos2d::Point(m_visibleSize.width / 2 + m_origin.x, m_visibleSize.height * y + m_origin.y));		// Set backgound position
}

/*
	Set y position and scale of the button menu items
*/
void MenuScene::setYPosAndScale(cocos2d::MenuItemImage* sprite, float y) {
	//sprite->setScale(scale);
	sprite->setPosition(cocos2d::Point(m_visibleSize.width / 2 + m_origin.x, m_visibleSize.height * y + m_origin.y));		// Set backgound position
	//sprite->runAction(cocos2d::ScaleTo::create(0.5F, (visibleSize.height == 1080) ? scale * 1.5f : scale * 1.0f));		// Animate the sprites when clicked
	sprite->runAction(cocos2d::ScaleTo::create(0.5F, (m_visibleSize.height == 1080) ? m_scale : m_scale ));					// Animate the sprites when clicked
}

/* 
	The player can enter and store their name from this scene
*/
void MenuScene::GoToEnterName(cocos2d::Ref *sender) {
	Audio::Instance()->playFX(BUTTON_FX);																					// Play button sound effect
	cocos2d::Scene* scene = EnterName::createScene();																		// Create the enter name scene
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFadeDown::create(TRANSITION_TIME, scene));			// Load the enter name screen with transition
}

/* 
	Change to the main menu scene
*/
void MenuScene::returnToMenu(cocos2d::Ref *sender) {
	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Scene* scene = MainMenu::createScene();																		// Return to the main menu
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));				// Switch scenes with animated transition
}

/* 
	Exit the game
*/
void MenuScene::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");				// No exit button message
	return;
#endif

	cocos2d::Director::getInstance()->end();																				// Close the director

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);																												// Different exit function for IOS
#endif
}

/*
	Select and highlight the next menu button
*/
void MenuScene::nextButton() {
	if (Game::Instance()->getTimeTick() > m_nextBtnTime) {
		if (m_currentBtn < m_totalButtons) {
			m_currentBtn++;																									// Increment selected button
		}
		else {
			m_currentBtn = 1;																								// Back to first button
		}

		m_nextBtnTime = Game::Instance()->getTimeTick() + m_buttonRate;														// Set the next button select time
		CCLOG("NEXT BUTTON: highlight m_currentBtn: %d", m_currentBtn);
	}
}

/*
	Select and highlight the previous menu button
*/
void MenuScene::prevButton() {
	if (Game::Instance()->getTimeTick() > m_nextBtnTime) {
		if (m_currentBtn > 1) {
			m_currentBtn--;																									// Decrement the current button
		} else {
			m_currentBtn = m_totalButtons;																					// Loop back to end of buttons
		}

		m_nextBtnTime = Game::Instance()->getTimeTick() + m_buttonRate;														// Set next time to press a button
		//CCLOG("PREV BUTTON: highlight m_currentBtn: %d", m_currentBtn);
	}
}

/*
// Test status bar

// Draw Rect test
cocos2d::DrawNode* rectNode = DrawNode::create();
Vec2 rectangle[4];
rectangle[0] = Vec2(-50, -20);
rectangle[1] = Vec2(50, -20);
rectangle[2] = Vec2(50, 20);
rectangle[3] = Vec2(-50, 20);

Vec2 rectangle2[4];
rectangle2[0] = Vec2(-48, -20);
rectangle2[1] = Vec2(48, -20);
rectangle2[2] = Vec2(48, 20);
rectangle2[3] = Vec2(-48, 20);


Color4F white(1, 1, 1, 1);
Color4F red(1, 0, 0, 1);
Color4F trans(1, 0, 0, 0);
rectNode->drawPolygon(rectangle, 4, trans, 1, red);
rectNode->drawPolygon(rectangle2, 4, trans, 1, white);

rectNode->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.05f + origin.y));

this->addChild(rectNode);


Color4F red(1, 0, 0, 1);
Color4F green(0, 1, 0, 1);
cocos2d::DrawNode* healthBar = createStatusBar(100, 50, 100, 10, 0.8f, red, green);
this->addChild(healthBar);
*/