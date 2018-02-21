/*
	MainMenu.cpp

	Joe O'Regan
	K00203642

	Game main menu, from here the player can start the game, view the highscores, or exit the game
	The current high score is displayed at the bottom of the menu
*/

#include "Game.h"
#include "MainMenu.h"
#include "Level1.h"								// Menu Item: Start game
#include "HighScores.h"							// Menu Item: High Scores
#include "EnterName.h"
#include "Settings.h"

#define TRANSITION_TIME 0.5

Scene* MainMenu::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	MainMenu* layer = MainMenu::create();		// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool MainMenu::init() {
	if (!Layer::init()) return false;																									// Super init first

	if (Game::Instance()->getPlayerName() == "") Game::Instance()->setPlayerName("Player");												// Set the players name to a default value

	visibleSize = Director::getInstance()->getVisibleSize();																			// Get visible screen size
	origin = Director::getInstance()->getVisibleOrigin();																				// Get screen origin point
		
	if (visibleSize.height == 1080)
		backgroundSprite = Sprite::create("BG1080p.png");
	else
		backgroundSprite = Sprite::create("BG720p.png");																				// Background image

	// Background
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));							// Set backgound position
	this->addChild(backgroundSprite);																									// Add to layer

	// Scene Title
	titleSprite = Sprite::create("MainMenu.png");																						// Scene Title image
	//titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - titleSprite->getContentSize().height));		// Set position on screen
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.925f + origin.y));							// Set position on screen
	this->addChild(titleSprite);																										// Add to layer							
	
	gameTitleSprite = Sprite::create("GameTitle.png");																					// Game Title image
	gameTitleSprite->setPosition(Point(visibleSize.width * 0.1f, visibleSize.height / 2 + origin.y));						// Set position on screen
	gameTitleSprite->setRotation(-90);
	this->addChild(gameTitleSprite);																									// Add to layer		

	// 1. Start Game Button
	playItem = MenuItemImage::create("btnStart.png", "btnStartSelect.png", CC_CALLBACK_1(MainMenu::StartGame, this));					// Set image for menu option
	playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.8f + origin.y));								// Set image position

	menu1 = Menu::create(playItem, NULL);																								// Menu
	menu1->setPosition(Point::ZERO);																									// Set position on screen
	this->addChild(menu1);																												// Add to layer																											// Add to layer							

	// 2. High Scores Button
	scoreItem = MenuItemImage::create("btnHighScores.png", "btnHighScoresSelect.png", CC_CALLBACK_1(MainMenu::GoToScores, this));		// Set image for menu option
	scoreItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.6f + origin.y));								// Set image position

	menu2 = Menu::create(scoreItem, NULL);																								// Menu
	menu2->setPosition(Point::ZERO);																									// Set position on screen
	this->addChild(menu2);																												// Add to layer

	// 3. Game Options
	optionsItem = MenuItemImage::create("btnSettings.png", "btnSettingsSelect.png", CC_CALLBACK_1(MainMenu::GoToSettings, this));		// Set image for options option
	optionsItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.4f + origin.y));							// Set image position

	menu3 = Menu::create(optionsItem, NULL);																							// Menu
	menu3->setPosition(Point::ZERO);																									// Set position on screen
	this->addChild(menu3);																												// Add to layer

	// 4. Exit Button
	closeItem = MenuItemImage::create("btnExit.png", "btnExitSelect.png", CC_CALLBACK_1(MainMenu::menuCloseCallback, this));			// Set image for menu option
	closeItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.2f + origin.y));								// Set image position

	menu4 = Menu::create(closeItem, NULL);																								// Menu
	menu4->setPosition(Point::ZERO);																									// Set position on screen
	this->addChild(menu4);																												// Add to layer

	// Show current high score
	def = UserDefault::getInstance();
	//int highScore = def->getIntegerForKey("HIGHSCORE", 0);
	highScore = def->getIntegerForKey("Score1", 0);																						// Load the high score
	std::string playerName = def->getStringForKey("Name1");

	//tempScore = __String::createWithFormat("Top Score: %s %i", playerName, highScore);
	tempScore = __String::createWithFormat("Top Score: %d", highScore);

	// Other fonts: MarkerFelt.ttf arial.ttf" Zombiebites.ttf Mario.ttf
	highScoreLbl = LabelTTF::create(tempScore->getCString(), "fonts/SuperMarioBros.ttf", visibleSize.height * 0.1); 
	highScoreLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.1f + origin.y));
	highScoreLbl->setColor(Color3B::RED);
	this->addChild(highScoreLbl);

	// Show current player name
	cocos2d::Label* changeNameLbl = Label::createWithBMFont("Arial.fnt", "Current Player:\n" + Game::Instance()->getPlayerName());		// Display current players name
	cocos2d::MenuItemLabel* mainMenuItem = MenuItemLabel::create(changeNameLbl, CC_CALLBACK_1(MainMenu::GoToEnterName, this));			// Go to enter name scene
	
	cocos2d::Menu* menu2 = Menu::create(mainMenuItem, NULL);																			// JOR replaced auto specifier
	menu2->setPosition(Point::ZERO);
	this->addChild(menu2);

	mainMenuItem->setPosition(Point(visibleSize.width * 0.9f, visibleSize.height *.9 + origin.y));
	mainMenuItem->runAction(ScaleTo::create(0.5F, 1.0F));

	return true;
}

// Start the Game Scene
void MainMenu::StartGame(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = Level1::createScene();																						// Create the game scene, JOR replaced auto specifier

	Director::getInstance()->replaceScene(TransitionSlideInB::create(TRANSITION_TIME, scene));
}

// Start the High Scores Scene
void MainMenu::GoToScores(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = HighScores::createScene();																					// Create the high scores scene

	Director::getInstance()->replaceScene(TransitionFadeUp::create(TRANSITION_TIME, scene));											// Load the high scores screen with transition
}

// Start the Settings Scene
void MainMenu::GoToSettings(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = Settings::createScene();																					// Create the enter name scene

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));												// Load the enter name screen with transition
}

// Start the Settings Scene
void MainMenu::GoToEnterName(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = EnterName::createScene();																					// Create the enter name scene

	Director::getInstance()->replaceScene(TransitionFadeDown::create(TRANSITION_TIME, scene));												// Load the enter name screen with transition
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