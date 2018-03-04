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
	MenuScene::init();																							// Menu Scene base class function
	if (!Layer::init()) return false;																			// Super init first

	m_pTitleSprite->setTexture("Settings.png");																	// Change the title image text

	m_visibleSize = Director::getInstance()->getVisibleSize();													// Get visible screen size
	m_origin = Director::getInstance()->getVisibleOrigin();														// Get screen origin point

	// Instructions
	m_pInstructionLbl = cocos2d::Label::createWithTTF("Adjust Game Settings", 
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.045f);											// Label to display current high score

	m_pInstructionLbl->setPosition(Point(m_visibleSize.width * 0.5 + m_origin.x, 
		m_visibleSize.height * 0.65f + m_origin.y));															// Set its position on screen
	m_pInstructionLbl->setColor(Color3B::WHITE);																// Set the text colour
	m_pInstructionLbl->setScale((m_visibleSize.height == 1080) ? 1.5f : 1.0f);									// Scale the lable depending on platform resolution																		// Set the scale
	this->addChild(m_pInstructionLbl);																			// Add it to the layer

	// 1. Show Hide Music Player
	m_pMusicLbl = cocos2d::Label::createWithTTF(playerLabelText(),												// Hide music player label 
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05);		
	m_pMusicLbl->setColor(Color3B::RED);

	m_pShowMPlayerLbl = cocos2d::MenuItemLabel::create(m_pMusicLbl, 
		CC_CALLBACK_1(Settings::showHideMusicControls, this));													// Show/Hide Music Player
	setLabelProperties(m_pShowMPlayerLbl, 0.5f);

	// 2. Set The Game Difficulty
	m_pDifficultyLbl = cocos2d::Label::createWithTTF(gameDifficulty(),											// Difficulty label
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05);
	m_pDifficultyLbl->setColor(Color3B::RED);

	m_pSetDifficultyLbl = cocos2d::MenuItemLabel::create(m_pDifficultyLbl, 
		CC_CALLBACK_1(Settings::changeDifficulty, this));														// Set the game difficulty
	setLabelProperties(m_pSetDifficultyLbl, 0.35f);

	// 3. Go To Audio Menu
	m_pAudioMenuLbl = cocos2d::Label::createWithTTF("Audio Settings",											// Create audio label
		"fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.05);
	m_pAudioMenuLbl->setColor(Color3B::RED);

	m_pGoToAudioLbl = cocos2d::MenuItemLabel::create(m_pAudioMenuLbl, 
		CC_CALLBACK_1(Settings::goToAudioMenu, this));															// Go to the Audio settings scene
	setLabelProperties(m_pGoToAudioLbl, 0.20f);

	// Add the Menu options
	m_menu = Menu::create(m_pShowMPlayerLbl, m_pGoToAudioLbl, m_pSetDifficultyLbl, NULL);						// Menu options to add
	m_menu->setPosition(Point::ZERO);																			// Set position on screen
	this->addChild(m_menu);

	return true;
}

/*
	Because the properties are mostly the same for each label
*/
void Settings::setLabelProperties(MenuItemLabel* label, float yPos) {
	label->setPosition(Point(m_visibleSize.width / 2 + m_origin.x, m_visibleSize.height * yPos + m_origin.y));	// Set the position on screen
	//label->setScale((visibleSize.height == 1080) ? 1.5f : 1.0f);
	label->runAction(cocos2d::ScaleTo::create(0.5F, (m_visibleSize.height == 1080) ? 1.5f : 1.0f));				// Run scaling action on labels
}

/*
	Alter button label text to match the current difficulty
*/
std::string Settings::gameDifficulty() {
	if (Game::Instance()->getDifficulty() == EASY)
		return "Difficulty Level: Easy";																		// Change label text
	else if (Game::Instance()->getDifficulty() == MEDIUM)
		return "Difficulty Level: Medium";

	return"Difficulty Level: Hard";																				// Show the difficulty level
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
	Audio::Instance()->selectMenuOption();
	Game::Instance()->hideMusicPlayer(!Game::Instance()->musicPlayerVisible());
	m_pMusicLbl->setString(playerLabelText());
}

/*
	Adjust the game difficulty, and set the label text to match
*/
void Settings::changeDifficulty(cocos2d::Ref *sender) {
	Audio::Instance()->selectMenuOption();

	if (Game::Instance()->getDifficulty() == EASY)
		Game::Instance()->setDifficulty(MEDIUM);
	else if (Game::Instance()->getDifficulty() == MEDIUM)
		Game::Instance()->setDifficulty(HARD);
	else
		Game::Instance()->setDifficulty(EASY);

	m_pSetDifficultyLbl->setString(gameDifficulty());
}

/* 
	Change scene to the Audio menu scene
*/
void Settings::goToAudioMenu(cocos2d::Ref *sender) {
	Audio::Instance()->selectMenuOption();

	cocos2d::Scene* scene = AudioMenu::createScene();															// Return to the Audio menu
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));	// Switch scenes with animated transition
}