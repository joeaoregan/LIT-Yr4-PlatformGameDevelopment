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
#include "StoryScene.h"
#include "Level1.h"								// Menu Item: Start game WAS LEVEL1
#include "HighScores.h"							// Menu Item: High Scores
#include "EnterName.h"							// Accessed by selecting current player menu text item
#include "Settings.h"							// Menu Item
#include "AudioMenu.h"							// Menu Item
#include "Input.h"

/*
	Create the scene
*/
Scene* MainMenu::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	m_pLayer = MainMenu::create();				// 'layer' is an autorelease object, JOR replaced auto specifier, CHANGED TO SINGLETON IN MenuScene CLASS FOR ACCESS		
	scene->addChild(m_pLayer);					// Add layer as a child to scene
		
	return scene;								// Return the scene
}

/*
	Initialise the Main Menu scene
	on "init" you need to initialize your instance
*/
bool MainMenu::init() {
	MenuScene::init();							// Initialise base class
	
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	if (!Game::Instance()->getSignedIn()) {		// If not already signed in
//		sdkbox::PluginSdkboxPlay::signin();		// Sign in for leaderboard
//		Game::Instance()->setSignedIn(true);	// Make sure it only does this once																
//	}
//#endif
	
	/********************************************/
	/*		1. Make changes to base class		*/
	/********************************************/
	m_pTitleSprite->setTexture("MainMenu.png");	// Change the title image text
	m_pBtnBackImg->setVisible(false);			// Hide back to main menu button, not needed
	MenuScene::m_pCloseItem->setVisible(false);	// Hide close button

	/********************************************/
	//		2. Scene Specific Additions			*/
	/********************************************/
	
	CCLOG("MainMenu init scale: %f", m_scale);

	m_scale *= 0.8f;																															// Adjust the text size (Scaled even smaller)

	// Button 1. Start Game Button
	m_pPlayItem = MenuItemImage::create("btnStart.png", "btnStartSelect.png", CC_CALLBACK_1(MainMenu::StartGame, this));						// Set image for menu option
	setYPosAndScale(m_pPlayItem, 0.6f);

	// Button 2. High Scores Button
	m_pScoreItem = MenuItemImage::create("btnHighScores.png", "btnHighScoresSelect.png", CC_CALLBACK_1(MainMenu::GoToScores, this));			// Set image for menu option
	setYPosAndScale(m_pScoreItem, 0.5f);

	// Button 3. Game Options
	m_pOptionsItem = MenuItemImage::create("btnSettings.png", "btnSettingsSelect.png", CC_CALLBACK_1(MainMenu::GoToSettings, this));			// Set image for options option
	setYPosAndScale(m_pOptionsItem, 0.4f);
	
	// Button 4. Exit Button
	m_pExitItem = MenuItemImage::create("btnExit.png", "btnExitSelect.png", CC_CALLBACK_1(MenuScene::menuCloseCallback, this));					// Set image for menu option
	setYPosAndScale(m_pExitItem, 0.3f);

	/*
		Additional buttons for social media and analytics
	*/
	m_pAchievmentItem = MenuItemImage::create("Achievements.png", "AchievementsSelected.png", CC_CALLBACK_1(MenuScene::GoToAchievements, this));// Set button graphics and callback
	m_pAchievmentItem->setPosition(cocos2d::Point(m_visibleSize.width * 0.15f + m_origin.x, m_visibleSize.height * 0.45f));						// Set achievement button position
	m_pAchievmentItem->setScale(m_visibleSize.height / 1080.0f);																				// Set scale factor

	m_pLeaderboardsItem = MenuItemImage::create("Leaderboards.png", "LeaderboardsSelected.png", CC_CALLBACK_1(MenuScene::GoToLeaderboards, this));	// Set button graphics and callback
	m_pLeaderboardsItem->setPosition(cocos2d::Point(m_visibleSize.width * 0.85f + m_origin.x, m_visibleSize.height * 0.45f));					// Set leaderboard button position
	m_pLeaderboardsItem->setScale(m_visibleSize.height / 1080.0f);																				// Set scale factor

	// Show current high score
	highScore = m_def->getIntegerForKey("Score1", 0);																							// Load the high score
	std::string playerName = m_def->getStringForKey("Name1");																					// and the name of the player who got the score
	
	/*
		Set the player name/identifier
	*/
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	if (sdkbox::SdkboxPlay::getPlayerId() != "" || sdkbox::SdkboxPlay::getPlayerId() != NULL) {													
//		playerName = sdkbox::SdkboxPlay::getPlayerId();
//	}
//#endif

	m_pTempScore = __String::createWithFormat("Top Score: %s %d", playerName.c_str(), highScore);												// String to display the top score & player	

	float scorePosition;
	(Game::Instance()->musicPlayerVisible()) ? scorePosition = 0.2f : scorePosition = 0.1f;														// Move label to fill space if music player invisible
	m_pHighScoreLbl = cocos2d::Label::createWithTTF(m_pTempScore->getCString(), "fonts/Super Mario Bros..ttf", m_visibleSize.height * 0.125f);	// Label to display current high score (Label replaces LabelTTF causing warnings)
	m_pHighScoreLbl->setPosition(Point(m_visibleSize.width * 0.5f + m_origin.x, m_visibleSize.height * scorePosition + m_origin.y));
	m_pHighScoreLbl->setColor(Color3B::RED);
	m_pHighScoreLbl->setScale((m_visibleSize.height == 1080) ? 0.5f : 0.4f);
	this->addChild(m_pHighScoreLbl);
	
	// Menu Items - added achievements and leaderboard buttons
	menu = Menu::create(m_pPlayItem, m_pScoreItem, m_pOptionsItem, m_pExitItem, m_pAchievmentItem, m_pLeaderboardsItem, NULL);					// Handles menu item touches (Can do more than one at a time)
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Music Player
	if (Game::Instance()->musicPlayerVisible()) {
		m_pPlayer = MusicPlayer::create(Point(m_visibleSize.width / 2, m_visibleSize.height * 0.125f));											// Create the music control buttons
		this->addChild(m_pPlayer);																												// Add the music player to the layer
	}																																			// Start updating the scene
	
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX ||													// Mobile platform doesn't need to handle keyboard input
		CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
		m_nextBtnTime = Game::Instance()->getTimeTick() + m_buttonRate;
		m_totalButtons = MAIN_MENU_BUTTONS;
		m_currentBtn = 4;
	}
			
	/*
	//playItem->selected();
	//CCLOG("MainMenu: init");
	//CCLOG("m_currentBtn: %d", m_currentBtn);
	exitItem->pause();
	optionsItem->pause();
	scoreItem->pause();
	playItem->pause();
	//CCLOG("MainMenu Init Complete");
	//SELECTED = false;
	//CCLOG("Main Menu Init Completed");
	*/

	this->scheduleUpdate();

	return true;
}

/*
	Update needed to swap pause and play buttons on music player controls
*/
void MainMenu::update(float dt) {
	
	MenuScene::update(dt);
	//CCLOG("MainMenu: update");

	if (Game::Instance()->musicPlayerVisible())
		m_pPlayer->update();																													// Update the music player

	//CCLOG("m_currentBtn: %d", m_currentBtn);


	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX ||													// Stop keyboard appearing android
		CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {

		highlightButton(m_currentBtn);																											// Highlight the active button

		//if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_ENTER) ||
		//	Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_RETURN)) {
		if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_ENTER)) {
			if (Game::Instance()->getTimeTick() > Game::Instance()->getButtonTimer()) {															// Set time between button presses
				//CCLOG("Enter Pressed");
				//nextBtnTime = Game::Instance()->getTimeTick() + buttonRate;
				Game::Instance()->setButtonTimer(Game::Instance()->getTimeTick() + m_buttonRate);												// Stored in game, think there was an issue between scenes

				if (m_currentBtn == 1 && !m_selected) {
					if (!m_selected) m_pExitItem->activate();
					m_selected = true;
					//exitItem->
				}
				else if (m_currentBtn == 2) {
					m_pOptionsItem->activate();
					m_selected = true;
				}
				else if (m_currentBtn == 3) {
					m_pScoreItem->activate();
					m_selected = true;
				}
				else if (m_currentBtn == 4) {
					m_pPlayItem->activate();
					m_selected = true;
				}
				else if (m_currentBtn == 5) {
					m_pLeaderboardsItem->activate();
					//m_selected = true;
				}
				else if (m_currentBtn == 6) {
					m_pAchievmentItem->activate();
					//m_selected = true;
				}
			}
		}
	}
}

/*
	Highlight the selected menu option
*/
void MainMenu::highlightButton(unsigned int btn) {
		if (btn == 1) m_pExitItem->selected();
		else m_pExitItem->unselected();

		if (btn == 2) m_pOptionsItem->selected();
		else m_pOptionsItem->unselected();

		if (btn == 3) m_pScoreItem->selected();
		else m_pScoreItem->unselected();

		if (btn == 4) m_pPlayItem->selected();
		else m_pPlayItem->unselected();

		if (btn == 5) m_pLeaderboardsItem->selected();
		else m_pLeaderboardsItem->unselected();

		if (btn == 6) m_pAchievmentItem->selected();
		else m_pAchievmentItem->unselected();
};

/* 
	Callback: Start the Game Scene
*/
void MainMenu::StartGame(cocos2d::Ref *sender) {
	if (!m_selected) {
		CCLOG("Start Game Selected");
		Audio::Instance()->playFX(BUTTON_FX);	
		cocos2d::Scene* scene = StoryScene::createScene();																						// Create the game scene, JOR replaced auto specifier
		Director::getInstance()->replaceScene(TransitionSlideInB::create(TRANSITION_TIME, scene));												// Create scene and transition
	}
}

/* 
	Callback: Start the High Scores Scene
*/
void MainMenu::GoToScores(cocos2d::Ref *sender) {
	if (!m_selected) {
		CCLOG("High Scores Selected");
		Audio::Instance()->playFX(BUTTON_FX);
		cocos2d::Scene* scene = HighScores::createScene();																						// Create the high scores scene
		Director::getInstance()->replaceScene(TransitionFadeUp::create(TRANSITION_TIME, scene));												// Load the high scores screen with transition
	}
}

/* 
	Callback: Start the Settings Scene
*/
void MainMenu::GoToSettings(cocos2d::Ref *sender) {
	if (!m_selected) {
		CCLOG("Settings Selected");
		Audio::Instance()->playFX(BUTTON_FX);
		cocos2d::Scene* scene = Settings::createScene();																						// Create the enter name scene
		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));													// Load the enter name screen with transition
	}
}

/* 
	Callback: Start the Enter Name Scene
*/
void MainMenu::GoToEnterName(cocos2d::Ref *sender) {
	if (!m_selected) {
		CCLOG("Enter Name Selected");
		Audio::Instance()->playFX(BUTTON_FX);
		cocos2d::Scene* scene = EnterName::createScene();																						// Create the enter name scene
		Director::getInstance()->replaceScene(TransitionFadeDown::create(TRANSITION_TIME, scene));												// Load the enter name screen with transition
	}
}
