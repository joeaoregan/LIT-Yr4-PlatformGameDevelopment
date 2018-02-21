#include "Game.h"
#include "MainMenu.h"
#include "Level1.h"								// Menu Item: Start game
#include "HighScores.h"							// Menu Item: High Scores

#define TRANSITION_TIME 0.5

Scene* MainMenu::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	MainMenu* layer = MainMenu::create();		// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool MainMenu::init() {
	if (!Layer::init()) return false;																											// Super init first

	Size visibleSize = Director::getInstance()->getVisibleSize();																				// Get visible screen size
	Vec2 origin = Director::getInstance()->getVisibleOrigin();																					// Get screen origin point

	cocos2d::Sprite* backgroundSprite;
	if (visibleSize.height == 1080)
		backgroundSprite = Sprite::create("BG1080p.png");
	else
		backgroundSprite = Sprite::create("BG720p.png");																						// Background image

	// Background
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));									// Set backgound position
	this->addChild(backgroundSprite);																											// Add to layer

	// Title
	cocos2d::Sprite* titleSprite = Sprite::create("MainMenu.png");																				// Title image
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - titleSprite->getContentSize().height));				// Set position on screen
	this->addChild(titleSprite);																												// Add to layer							

	// Start Game Button
	cocos2d::MenuItemImage* playItem = MenuItemImage::create("btnStart.png", "btnStartSelect.png", CC_CALLBACK_1(MainMenu::StartGame, this));	// Set image for menu option
	playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.7f + origin.y));										// Set image position

	cocos2d::Menu* menu1 = Menu::create(playItem, NULL);																							// Menu
	menu1->setPosition(Point::ZERO);																												// Set position on screen
	this->addChild(menu1);																														// Add to layer																											// Add to layer							

	// High Scores Button
	cocos2d::MenuItemImage* scoreItem = MenuItemImage::create("btnHighScores.png", "btnHighScoresSelect.png", CC_CALLBACK_1(MainMenu::GoToHighScores, this));	// Set image for menu option
	scoreItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.5f + origin.y));												// Set image position

	cocos2d::Menu* menu2 = Menu::create(scoreItem, NULL);																								// Menu
	menu2->setPosition(Point::ZERO);																														// Set position on screen
	this->addChild(menu2);																																// Add to layer

	// Exit Button
	cocos2d::MenuItemImage* closeItem = MenuItemImage::create("btnExit.png", "btnExitSelect.png", CC_CALLBACK_1(MainMenu::menuCloseCallback, this));	// Set image for menu option
	closeItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.3f + origin.y));												// Set image position

	cocos2d::Menu* menu3 = Menu::create(closeItem, NULL);																								// Menu
	menu3->setPosition(Point::ZERO);																														// Set position on screen
	this->addChild(menu3);																																// Add to layer



	// Show current high score
	UserDefault* def = UserDefault::getInstance();
	int highScore = def->getIntegerForKey("HIGHSCORE", 0);

	__String *tempScore = __String::createWithFormat("Highest Score: %i", highScore);

	//cocos2d::LabelTTF* highScoreLbl = LabelTTF::create(tempScore->getCString(), "fonts/MarkerFelt.ttf", visibleSize.height * 0.1);
	//cocos2d::LabelTTF* highScoreLbl = LabelTTF::create(tempScore->getCString(), "fonts/arial.ttf", visibleSize.height * 0.1);
	cocos2d::LabelTTF* highScoreLbl = LabelTTF::create(tempScore->getCString(), "fonts/Zombiebites.ttf", visibleSize.height * 0.1);
	highScoreLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.1f + origin.y));
	highScoreLbl->setColor(Color3B::RED);
	this->addChild(highScoreLbl);

	return true;
}

// Start the Game Scene
void MainMenu::StartGame(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = Level1::createScene();																							// Create the game scene, JOR replaced auto specifier

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

// Start the High Scores Scene
void MainMenu::GoToHighScores(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = HighScores::createScene();																						// Create the high scores scene

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

// Exit the game
void MainMenu::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}