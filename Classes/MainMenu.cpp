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
#include "AudioMenu.h"

//#define TRANSITION_TIME 0.5

Scene* MainMenu::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	layer = MainMenu::create();					// 'layer' is an autorelease object, JOR replaced auto specifier, CHANGED TO SINGLETON IN MenuScene CLASS FOR ACCESS		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool MainMenu::init() {
	MenuScene::init();	

	// 1. Make changes to base class
	titleSprite->setTexture("MainMenu.png");																							// Change the title image text
	btnBackImg->setVisible(false);																										// Hide back to main menu button, not needed
	MenuScene::closeItem->setVisible(false);																										// Hide close button

	// 2. Additions

	// Button 1. Start Game Button
	playItem = MenuItemImage::create("btnStart.png", "btnStartSelect.png", CC_CALLBACK_1(MainMenu::StartGame, this));					// Set image for menu option
	playItem->setScale(scale * 0.8f);
	playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.6f + origin.y));								// Set image position

	// Button 2. High Scores Button
	scoreItem = MenuItemImage::create("btnHighScores.png", "btnHighScoresSelect.png", CC_CALLBACK_1(MainMenu::GoToScores, this));		// Set image for menu option
	scoreItem->setScale(scale * 0.8f);
	scoreItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.5f + origin.y));								// Set image position

	// Button 3. Game Options
	optionsItem = MenuItemImage::create("btnSettings.png", "btnSettingsSelect.png", CC_CALLBACK_1(MainMenu::GoToSettings, this));		// Set image for options option
	optionsItem->setScale(scale * 0.8f);
	optionsItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.4f + origin.y));							// Set image position
	
	// Button 4. Exit Button
	exitItem = MenuItemImage::create("btnExit.png", "btnExitSelect.png", CC_CALLBACK_1(MenuScene::menuCloseCallback, this));			// Set image for menu option
	exitItem->setScale(scale * 0.8f);
	exitItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.3f + origin.y));								// Set image position

	// Show current high score
	//int highScore = def->getIntegerForKey("HIGHSCORE", 0);
	highScore = def->getIntegerForKey("Score1", 0);																						// Load the high score
	std::string playerName = def->getStringForKey("Name1");
	
	tempScore = __String::createWithFormat("Top Score: %s %d", playerName.c_str(), highScore);
	//tempScore = __String::createWithFormat("Top Score: %d", highScore);
	// Other fonts: MarkerFelt.ttf arial.ttf" Zombiebites.ttf Mario.ttf Super Mario Bros..ttf
	highScoreLbl = LabelTTF::create(tempScore->getCString(), "fonts/Super Mario Bros..ttf", visibleSize.height * 0.125f); 
	highScoreLbl->setPosition(Point(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.2f + origin.y));
	highScoreLbl->setColor(Color3B::RED);
	highScoreLbl->setScale(scale * 0.6f);
	this->addChild(highScoreLbl);
	
	// Menu Items
	menu = Menu::create(playItem, scoreItem, optionsItem, exitItem, NULL);												// Handles menu item touches
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Music Player
	mplayer = MusicPlayer::create(Point((visibleSize.width * 1.33) / 2, visibleSize.height * 0.15f));
	this->addChild(mplayer);

	this->scheduleUpdate();																												// Start updating the scene

	return true;
}

void MainMenu::update(float dt) {
	mplayer->update();
	/*
	if (mplayer->getButton(1)->isSelected()) {
		mplayer->playTrack();							// Swap play & pause button sprites
	}
	else if (mplayer->getButton(2)->isSelected()) {
		mplayer->pauseTrack();							// Swap play & pause button sprites
	}
	else if (mplayer->getButton(3)->isSelected()) {
		mplayer->nextTrack();							// Skip track forwards
	}
	else if (mplayer->getButton(4)->isSelected()) {
		mplayer->previousTrack();						// Skip track backwards
	}
	*/
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
	cocos2d::Scene* scene = AudioMenu::createScene();																					// Create the enter name scene

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));												// Load the enter name screen with transition
}

// Start the Enter Name Scene
void MainMenu::GoToEnterName(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = EnterName::createScene();																					// Create the enter name scene

	Director::getInstance()->replaceScene(TransitionFadeDown::create(TRANSITION_TIME, scene));												// Load the enter name screen with transition
}
