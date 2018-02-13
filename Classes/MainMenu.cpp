#include "MainMenu.h"
#include "GameScene.h"

USING_NS_CC;

#define TRANSITION_TIME 0.5

Scene* MainMenu::createScene() {
	auto scene = Scene::create();			// 'scene' is an autorelease object
	auto layer = MainMenu::create();		// 'layer' is an autorelease object
		
	scene->addChild(layer);					// Add layer as a child to scene
		
	return scene;							// Return the scene
}

// on "init" you need to initialize your instance
bool MainMenu::init() {
	if (!Layer::init()) return false;																											// Super init first

	Size visibleSize = Director::getInstance()->getVisibleSize();																				// Get visible screen size
	Vec2 origin = Director::getInstance()->getVisibleOrigin();																					// Get screen origin point

	cocos2d::Sprite* backgroundSprite = Sprite::create("BG720p.png");																			// Background image
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));									// Set backgound position
	this->addChild(backgroundSprite);																											// Add to layer

	cocos2d::Sprite* titleSprite = Sprite::create("GameTitle.png");																				// Title image
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - titleSprite->getContentSize().height));				// Set position on screen
	this->addChild(titleSprite);																												// Add to layer							

	cocos2d::MenuItemImage* playItem = MenuItemImage::create("BtnStart.png", "BtnStartClicked.png", CC_CALLBACK_1(MainMenu::StartGame, this));	// Set image for menu option
	playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));											// Set image position

	cocos2d::Menu* menu = Menu::create(playItem, NULL);																							// Menu
	menu->setPosition(Point::ZERO);																												// Set position on screen
	this->addChild(menu);																														// Add to layer

	return true;
}

void MainMenu::StartGame(cocos2d::Ref *sender) {
	auto scene = GameScene::createScene();																										// Create the game scene

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}