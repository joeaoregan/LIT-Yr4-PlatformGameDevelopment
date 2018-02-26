/*
	Settings.cpp

	Joe O'Regan
	K00203642
	25/12/2018

	Adjust game settings
	* Set visibility of music player
	* Change game difficulty
	* Access audio settings menu
*/

#include "Game.h"
#include "Settings.h"
#include "Audio.h"
#include "AudioMenu.h"

cocos2d::Scene* Settings::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	Settings* layer = Settings::create();		// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool Settings::init() {
	MenuScene::init();																									// Menu Scene base class function
	if (!Layer::init()) return false;																					// Super init first

	titleSprite->setTexture("Settings.png");																			// Change the title image text

	visibleSize = Director::getInstance()->getVisibleSize();															// Get visible screen size
	origin = Director::getInstance()->getVisibleOrigin();																// Get screen origin point

	// Instructions
	instructionLbl = cocos2d::Label::createWithTTF("Adjust Game Settings", 
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);													// Label to display current high score

	instructionLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.65f + origin.y));		// Set its position on screen
	instructionLbl->setColor(Color3B::WHITE);																			// Set the text colour
	instructionLbl->setScale((visibleSize.height == 1080) ? 1.5f : 1.0f);												// Scale the lable depending on platform resolution																		// Set the scale
	this->addChild(instructionLbl);																						// Add it to the layer

	// 1. Show Hide Music Player
	musicLbl = cocos2d::Label::createWithTTF(playerLabelText(), 
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);		
	musicLbl->setColor(Color3B::RED);

	showMPlayerLbl = cocos2d::MenuItemLabel::create(musicLbl, CC_CALLBACK_1(Settings::showHideMusicControls, this));	// Show/Hide Music Player
	setLabelProperties(showMPlayerLbl, 0.5f);

	// 2. Set The Game Difficulty
	difficultyLbl = cocos2d::Label::createWithTTF(gameDifficulty(), 
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);
	difficultyLbl->setColor(Color3B::RED);

	setDifficultyLbl = cocos2d::MenuItemLabel::create(difficultyLbl, CC_CALLBACK_1(Settings::changeDifficulty, this));	// Set the game difficulty
	setLabelProperties(setDifficultyLbl, 0.35f);

	// 3. Go To Audio Menu
	audioMenuLbl = cocos2d::Label::createWithTTF("Audio Settings", 
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);
	audioMenuLbl->setColor(Color3B::RED);

	goToAudioLbl = cocos2d::MenuItemLabel::create(audioMenuLbl, CC_CALLBACK_1(Settings::goToAudioMenu, this));			// Go to the Audio settings scene
	setLabelProperties(goToAudioLbl, 0.20f);

	// Add the Menu options
	menu = Menu::create(showMPlayerLbl, goToAudioLbl, setDifficultyLbl, NULL);											// Menu options to add
	menu->setPosition(Point::ZERO);																						// Set position on screen
	this->addChild(menu);

	return true;
}

/*
	Because the properties are mostly the same for each label
*/
void Settings::setLabelProperties(MenuItemLabel* label, float yPos) {
	label->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * yPos + origin.y));
	//label->setScale((visibleSize.height == 1080) ? 1.5f : 1.0f);
	label->runAction(cocos2d::ScaleTo::create(0.5F, (visibleSize.height == 1080) ? 1.5f : 1.0f));
}

/*
	Alter button label text to match the current difficulty
*/
std::string Settings::gameDifficulty() {
	if (Game::Instance()->getDifficulty() == EASY)
		return "Difficulty Level: Easy";
	else if (Game::Instance()->getDifficulty() == MEDIUM)
		return "Difficulty Level: Medium";

	return"Difficulty Level: Hard";																						// Show the difficulty level
}

/*
	Alter the button label for music player visibility
*/
std::string Settings::playerLabelText() {
	if (!Game::Instance()->musicPlayerVisible())
		return "Enable Music Player";								

	return"Disable Music Player";																							
}

// Callbacks
/*
	Set the visibility of the music label, and adjust the label text
*/
void Settings::showHideMusicControls(cocos2d::Ref *sender) {
	Game::Instance()->hideMusicPlayer(!Game::Instance()->musicPlayerVisible());
	musicLbl->setString(playerLabelText());
}
/*
	Adjust the game difficulty, and set the label text to match
*/
void Settings::changeDifficulty(cocos2d::Ref *sender) {
	if (Game::Instance()->getDifficulty() == EASY)
		Game::Instance()->setDifficulty(MEDIUM);
	else if (Game::Instance()->getDifficulty() == MEDIUM)
		Game::Instance()->setDifficulty(HARD);
	else
		Game::Instance()->setDifficulty(EASY);

	setDifficultyLbl->setString(gameDifficulty());
}
/* 
	Change scene to the Audio menu scene
*/
void Settings::goToAudioMenu(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = AudioMenu::createScene();																	// Return to the Audio menu
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Switch scenes with animated transition
}