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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <SDL.h>										// For gamepad support on desktop
#endif
//#include "HealthBar.h"

MenuScene* MenuScene::layer;

cocos2d::Scene* MenuScene::createScene() {
	cocos2d::Scene* scene = cocos2d::Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	layer = MenuScene::create();						// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(layer);								// Add layer as a child to scene
		
	return scene;										// Return the scene
}

// on "init" you need to initialize your instance
bool MenuScene::init() {
	if (!Layer::init()) return false;																								// Super init first

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//	// SDL Gamepad Support for Windows
//	if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
//		//std::cout << "SDL could not initialize! SDL Error: %s\n" << SDL_GetError() << std::endl;
//		cocos2d::log("SDL init failed");
//	}									// For gamepad support on desktop
//#endif
	
	def = cocos2d::UserDefault::getInstance();																						// User defaults used to store/load values
	
	// Screen size
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();																// Get visible screen size
	origin = cocos2d::Director::getInstance()->getVisibleOrigin();																	// Get screen origin point		
	(visibleSize.height == 1080) ? scale = 1.0f : scale = 0.67f;
	
	// Background image
	//backgroundSprite = cocos2d::Sprite::create((visibleSize.height == 1080) ? "BG1080p.png" : "BG720p.png");						// Choose Background image depending on resolution
	backgroundSprite = cocos2d::Sprite::create("BG1080p.png");																		// Use one high res background image for efficiency
	backgroundSprite->setScale(scale);
	backgroundSprite->setPosition(cocos2d::Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));				// Set backgound position
	this->addChild(backgroundSprite);																								// Add to layer
	
	/*
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

	// Game Title
	gameTitleSprite = cocos2d::Sprite::create("GameTitle.png");																		// Game Title image
	gameTitleSprite->setPosition(cocos2d::Point(visibleSize.width * 0.1f, visibleSize.height / 2 + origin.y));						// Set position on screen
	gameTitleSprite->setPosition(cocos2d::Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.9f + origin.y));			// Set position on screen
	gameTitleSprite->setScale(1.75 * scale);
	this->addChild(gameTitleSprite);																								// Add to layer		
		
	// Scene Title
	titleSprite = cocos2d::Sprite::create("MainMenu.png");																			// Scene Title image
	titleSprite->setPosition(cocos2d::Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75f + origin.y));				// Set position on screen
	titleSprite->setScale(scale);
	this->addChild(titleSprite);																									// Add to layer							
	
	// Show current player name. Can select to switch to Enter Name state
	currentPlayerName = def->getStringForKey("CurrentPlayer", "Player");
	//if (Game::Instance()->getPlayerName() == "") 
	Game::Instance()->setPlayerName(currentPlayerName);																				// Set the players name to a default value
	changeNameLbl = cocos2d::Label::createWithBMFont("Arial.fnt", "Current Player:\n" + currentPlayerName);							// Display current players name
	changeNameLbl->setScale(scale);
	currentPlayerLbl = cocos2d::MenuItemLabel::create(changeNameLbl, CC_CALLBACK_1(MenuScene::GoToEnterName, this));				// Go to enter name scene
	currentPlayerLbl->setPosition(cocos2d::Point((visibleSize.width + origin.x) * 0.1f, visibleSize.height * 0.95f + origin.y));
	currentPlayerLbl->runAction(cocos2d::ScaleTo::create(0.5F, 1.0F));

	// Back Button
	btnBackImg = cocos2d::MenuItemImage::create("btnBack.png", "btnBackSelect.png", CC_CALLBACK_1(MenuScene::returnToMenu, this));	// Set image for menu option
	btnBackImg->setPosition(cocos2d::Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.05f + origin.y));				// Set image position
	btnBackImg->setScale(scale);																									// Set the scale

	// Exit button (autorelease object)
	closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(MenuScene::menuCloseCallback, this));																			// Close game button, JOR replaced auto specifier
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// Menu Items
	menu = cocos2d::Menu::create(currentPlayerLbl, btnBackImg, closeItem, NULL);													// Handles menu item touches
	menu->setPosition(cocos2d::Point::ZERO);
	this->addChild(menu);

	return true;
}

// The player can enter and store their name from this scene
void MenuScene::GoToEnterName(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = EnterName::createScene();																				// Create the enter name scene
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFadeDown::create(TRANSITION_TIME, scene));					// Load the enter name screen with transition
}

// Change to the main menu scene
void MenuScene::returnToMenu(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = MainMenu::createScene();																				// Return to the main menu
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));						// Switch scenes with animated transition
}

// Exit the game
void MenuScene::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	cocos2d::Director::getInstance()->end();	// Close the director

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}