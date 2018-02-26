/*
	MainMenu.cpp

	Joe O'Regan
	K00203642

	Child of MenuScene, adds to base class, that includes background image, game name, scene title, 
	player identity button that accesses EnterName scene, and back button
	
	Game main menu, from here the player can start the game, view the highscores, or exit the game
	The current high score is displayed at the bottom of the menu
*/

#include "Game.h"
#include "MainMenu.h"
#include "Level1.h"								// Menu Item: Start game WAS LEVEL1
#include "HighScores.h"							// Menu Item: High Scores
#include "EnterName.h"							// Accessed by selecting current player menu text item
#include "Settings.h"							// Menu Item
#include "AudioMenu.h"							// Menu Item

Scene* MainMenu::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	layer = MainMenu::create();					// 'layer' is an autorelease object, JOR replaced auto specifier, CHANGED TO SINGLETON IN MenuScene CLASS FOR ACCESS		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool MainMenu::init() {
	MenuScene::init();							// Initialise base class
	

	/********************************************/
	/*		1. Make changes to base class		*/
	/********************************************/
	titleSprite->setTexture("MainMenu.png");	// Change the title image text
	btnBackImg->setVisible(false);				// Hide back to main menu button, not needed
	MenuScene::closeItem->setVisible(false);	// Hide close button

	/********************************************/
	//		2. Scene Specific Additions			*/
	/********************************************/
	
	scale *= 0.8f;																													// Adjust the text size

	// Button 1. Start Game Button
	playItem = MenuItemImage::create("btnStart.png", "btnStartSelect.png", CC_CALLBACK_1(MainMenu::StartGame, this));				// Set image for menu option
	setYPosAndScale(playItem, 0.6f);

	// Button 2. High Scores Button
	scoreItem = MenuItemImage::create("btnHighScores.png", "btnHighScoresSelect.png", CC_CALLBACK_1(MainMenu::GoToScores, this));	// Set image for menu option
	setYPosAndScale(scoreItem, 0.5f);

	// Button 3. Game Options
	optionsItem = MenuItemImage::create("btnSettings.png", "btnSettingsSelect.png", CC_CALLBACK_1(MainMenu::GoToSettings, this));	// Set image for options option
	setYPosAndScale(optionsItem, 0.4f);
	
	// Button 4. Exit Button
	exitItem = MenuItemImage::create("btnExit.png", "btnExitSelect.png", CC_CALLBACK_1(MenuScene::menuCloseCallback, this));		// Set image for menu option
	setYPosAndScale(exitItem, 0.3f);

	// Show current high score
	//int highScore = def->getIntegerForKey("HIGHSCORE", 0);
	highScore = def->getIntegerForKey("Score1", 0);																					// Load the high score
	std::string playerName = def->getStringForKey("Name1");																			// and the name of the player who got the score
	
	tempScore = __String::createWithFormat("Top Score: %s %d", playerName.c_str(), highScore);										// String to display the top score & player	

	float scorePosition;
	(Game::Instance()->musicPlayerVisible()) ? scorePosition = 0.2f : scorePosition = 0.1f;											// Move label to fill space if music player invisible
	highScoreLbl = cocos2d::Label::createWithTTF(tempScore->getCString(), "fonts/Super Mario Bros..ttf", visibleSize.height * 0.125f);// Label to display current high score (Label replaces LabelTTF causing warnings)
	highScoreLbl->setPosition(Point(visibleSize.width * 0.5f + origin.x, visibleSize.height * scorePosition + origin.y));
	highScoreLbl->setColor(Color3B::RED);
	highScoreLbl->setScale(scale * 0.75f);
	this->addChild(highScoreLbl);
	
	// Menu Items
	menu = Menu::create(playItem, scoreItem, optionsItem, exitItem, NULL);															// Handles menu item touches (Can do more than one at a time)
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Music Player
	if (Game::Instance()->musicPlayerVisible()) {
		mplayer = MusicPlayer::create(Point((visibleSize.width * 1.33) / 2, visibleSize.height * 0.15f));							// Create the music control buttons
		this->addChild(mplayer);																									// Add the music player to the layer
	}
	this->scheduleUpdate();																											// Start updating the scene

	return true;
}

/*
	Update needed to swap pause and play buttons on music player controls
*/
void MainMenu::update(float dt) {
	if (Game::Instance()->musicPlayerVisible())
		mplayer->update();																											// Update the music player
}

/* 
	Callback: Start the Game Scene
*/
void MainMenu::StartGame(cocos2d::Ref *sender) {
	Audio::Instance()->selectMenuOption();
	cocos2d::Scene* scene = Level1::createScene();																					// Create the game scene, JOR replaced auto specifier
	Director::getInstance()->replaceScene(TransitionSlideInB::create(TRANSITION_TIME, scene));										// Create scene and transition
}

/* 
	Callback: Start the High Scores Scene
*/
void MainMenu::GoToScores(cocos2d::Ref *sender) {
	Audio::Instance()->selectMenuOption();
	cocos2d::Scene* scene = HighScores::createScene();																				// Create the high scores scene
	Director::getInstance()->replaceScene(TransitionFadeUp::create(TRANSITION_TIME, scene));										// Load the high scores screen with transition
}

/* 
	Callback: Start the Settings Scene
*/
void MainMenu::GoToSettings(cocos2d::Ref *sender) {
	Audio::Instance()->selectMenuOption();
	cocos2d::Scene* scene = Settings::createScene();																				// Create the enter name scene
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));											// Load the enter name screen with transition
}

/* 
	Callback: Start the Enter Name Scene
*/
void MainMenu::GoToEnterName(cocos2d::Ref *sender) {
	Audio::Instance()->selectMenuOption();
	cocos2d::Scene* scene = EnterName::createScene();																				// Create the enter name scene
	Director::getInstance()->replaceScene(TransitionFadeDown::create(TRANSITION_TIME, scene));										// Load the enter name screen with transition
}
