#include "Game.h"
#include "HighScores.h"
#include "MainMenu.h"
#include "Level1.h"

#define TRANSITION_TIME 0.5

Scene* HighScores::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	HighScores* layer = HighScores::create();	// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool HighScores::init() {
	if (!Layer::init()) return false;																											// Super init first

	visibleSize = Director::getInstance()->getVisibleSize();																					// Get visible screen size
	origin = Director::getInstance()->getVisibleOrigin();																						// Get screen origin point

	initBackground();																															// Decide which background image to use, based on the platforms screen resolution

	this->addChild(backgroundSprite);																											// Add to layer

	cocos2d::Sprite* titleSprite = Sprite::create("GameTitle.png");																				// Title image
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - titleSprite->getContentSize().height));				// Set position on screen
	this->addChild(titleSprite);																												// Add to layer							

	cocos2d::MenuItemImage* playItem = MenuItemImage::create("BtnStart.png","BtnStartClicked.png",CC_CALLBACK_1(HighScores::returnToMenu,this));// Set image for menu option
	playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));											// Set image position

	cocos2d::Menu* menu = Menu::create(playItem, NULL);																							// Menu
	menu->setPosition(Point::ZERO);																												// Set position on screen
	this->addChild(menu);																														// Add to layer


	// Show current high score
	UserDefault* def = UserDefault::getInstance();
	int highScore = def->getIntegerForKey("HIGHSCORE", 0);

	__String *tempScore = __String::createWithFormat("High Score: %i", highScore);

	//cocos2d::LabelTTF* highScoreLbl = LabelTTF::create(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.1);
	cocos2d::LabelTTF* highScoreLbl = LabelTTF::create(tempScore->getCString(), "fonts/arial.ttf", visibleSize.height * 0.1);
	highScoreLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.2f + origin.y));
	highScoreLbl->setColor(Color3B::RED);
	this->addChild(highScoreLbl);

	return true;
}

void HighScores::returnToMenu(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = MainMenu::createScene();											// Return to the main menu

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));		// Switch scenes with animated transition
}

void HighScores::initBackground() {
	// Decide which background image to use, based on the platforms screen resolution
	if (visibleSize.height == 1080)
		backgroundSprite = Sprite::create("BG1080p.png");
	else
		backgroundSprite = Sprite::create("BG720p.png");																						// Background image

	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));									// Set backgound position
}