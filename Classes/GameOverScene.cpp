/*
	GameOverScene.cpp

	Joe O'Regan
	K00203642
	06/03/2018

	Display the game over message for use with the game trailer
*/

#include "GameOverScene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "CreditsScene.h"
#include "Game.h"
#include "Defines.h"
#include "Audio.h"

/*
	Create the scene
*/
cocos2d::Scene* GameOverScene::createScene() {
    cocos2d::Scene* scene = cocos2d::Scene::create();									// Create the scene. 'scene' is an autorelease object
        
	GameOverScene* layer = GameOverScene::create();										// Create the layer. 'layer' is an autorelease object
	
    scene->addChild(layer);																// Add layer as a child to scene
	    
    return scene;																		// Return the scene
}

/*
	Initialise the scene, displaying story and navigation buttons
*/
bool GameOverScene::init() {
    // 1. super init first
    if ( !Layer::init() ) { return false; }
	    
	m_pVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();				// Get screen resolution
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();		// Get screen origin

	// Display the game title image
	m_pGameTitleImg = cocos2d::Sprite::create("GameTitle.png");							// Game Title image
	m_pGameTitleImg->setPosition(cocos2d::Point(m_pVisibleSize.width * 0.1f,
		m_pVisibleSize.height / 2 + origin.y));											// Set position on screen
	m_pGameTitleImg->setPosition(cocos2d::Point(m_pVisibleSize.width / 2 + origin.x, 
		m_pVisibleSize.height * 0.9f + origin.y));										// Set position on screen
	m_pGameTitleImg->setScale(1.75 * (m_pVisibleSize.height == 1080) ? 1.0f : 0.67f);	// Set the scale of the title image based on the screen resolution
	this->addChild(m_pGameTitleImg);													// Add to layer		
	
	std::string message;																// Message to display, depending on the level
	
	// Select the message and callback based on the current level number
	// and set the destination for the continue button being pressed
	if (Game::Instance()->getLevel() == 1) {
		message = "The Player has failed to beat\nthe first round of Enemies";
	}
	else if (Game::Instance()->getLevel() == 2) {
		message = "The Player has failed to beat\nthe second round of Enemies";
	}
	else if (Game::Instance()->getLevel() == 3) {
		message = "The Player has failed to beat\nthe third round of Enemies";
	}
	else if (Game::Instance()->getLevel() == 4) {
		//if (m_victorious)
		if (Game::Instance()->getWon())
			message = "You have destroyed the enemy battle base\n\nIt is time to return home";
		else
			message = "You failed to destroy the enemy battle base\n\nYou went down fighting!";
	}
	
	Game::Instance()->setLevel(1);

	this->scheduleOnce(schedule_selector(GameOverScene::returnToMenu), DISPLAY_TIME_SPLASH_STORY);						// Automatically return to main menu

	// Game Over label text
	m_pLevelLbl = cocos2d::Label::createWithTTF("Game Over",
		"fonts/Super Mario Bros..ttf", m_pVisibleSize.height * 0.075);
	m_pLevelLbl->setPosition(m_pVisibleSize.width / 2, m_pVisibleSize.height * 0.75f);
	m_pLevelLbl->setColor(cocos2d::Color3B::RED);
	this->addChild(m_pLevelLbl);

	// Story lable text
	m_pStoryLbl =  cocos2d::Label::createWithTTF(message, "fonts/Super Mario Bros..ttf",										// Display story decided by level number				
		m_pVisibleSize.height * 0.05);
	m_pStoryLbl->setPosition(m_pVisibleSize.width / 2, m_pVisibleSize.height / 2);
	m_pStoryLbl->setColor(cocos2d::Color3B::GRAY);																				// Use gray text
	this->addChild(m_pStoryLbl);

	UserDefault* def = UserDefault::getInstance();
	unsigned int highScore = def->getIntegerForKey("Score1", 0);	// Get current saved high score

	std::string scoreTxt = "Your Score: " + cocos2d::StringUtils::toString(Game::Instance()->getScore()) +
		"\nTop Score: " + cocos2d::StringUtils::toString(highScore);


	scoreTxt = scoreAchievement(highScore);																						// Check has the player set a new high score

	Label* scoreLbl = cocos2d::Label::createWithTTF(scoreTxt, "fonts/Super Mario Bros..ttf", m_pVisibleSize.height * 0.075);
	scoreLbl->setPosition(m_pVisibleSize.width / 2, m_pVisibleSize.height * 0.25f);
	scoreLbl->setColor(cocos2d::Color3B::WHITE);
	this->addChild(scoreLbl);


	// Restart the game from the start		
	m_pContinueLbl = cocos2d::Label::createWithTTF("Press To\nRestart", "fonts/Super Mario Bros..ttf",
		m_pVisibleSize.height * 0.05);
	m_pContinueLbl->setColor(cocos2d::Color3B::RED);

	m_pContinueItem = MenuItemLabel::create(m_pContinueLbl, CC_CALLBACK_1(GameOverScene::restartGame, this));					// Continue to the next level menu option
	m_pContinueItem->setPosition(m_pVisibleSize.width * 0.1f, m_pVisibleSize.height * 0.1f);									// Restart the game from the start

	// Return to main menu button
	m_pReturnToMenuLbl = cocos2d::Label::createWithTTF(" Return\nTo Menu",														// Back to menu button label
		"fonts/Super Mario Bros..ttf", m_pVisibleSize.height * 0.05);
	m_pReturnToMenuLbl->setColor(cocos2d::Color3B::RED);

	m_pReturnToMenuItem = MenuItemLabel::create(m_pReturnToMenuLbl, CC_CALLBACK_1(GameOverScene::backToMenu, this));			// Go Back To Main Menu menu option
	m_pReturnToMenuItem->setPosition(m_pVisibleSize.width * 0.9f, m_pVisibleSize.height * 0.1f);
	
	Label * creditsLbl = cocos2d::Label::createWithTTF("Credits", "fonts/Super Mario Bros..ttf",
		m_pVisibleSize.height * 0.05);
	creditsLbl->setColor(cocos2d::Color3B::RED);

	cocos2d::MenuItemLabel* creditsItem = MenuItemLabel::create(creditsLbl, CC_CALLBACK_1(GameOverScene::showCredits, this));	// Continue to the next level menu option
	creditsItem->setPosition(m_pVisibleSize.width * 0.5f, m_pVisibleSize.height * 0.1f);										// Restart the game from the start

	// Menu items
	m_menu = Menu::create(m_pContinueItem, creditsItem, m_pReturnToMenuItem, NULL);												// Create the menu options
	m_menu->setPosition(Point::ZERO);
	this->addChild(m_menu);
			
    return true;
}

void GameOverScene::returnToMenu(float dt) {
	cocos2d::Scene* scene = MainMenu::createScene();																			// Create the Level4 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));					// Change the scene with transition
}

/*
	Restart the game from level 1
*/
void GameOverScene::restartGame(cocos2d::Ref* pSender) {
	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionZoomFlipX::create(0.5, Level1::createScene()));			// Restart the current scene	
	this->scheduleUpdate();																										// reschedule
}

void GameOverScene::backToMenu(cocos2d::Ref* pSender) {
	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionZoomFlipX::create(0.5, MainMenu::createScene()));			// Restart the current scene	
	this->scheduleUpdate();																										// reschedule
}

void GameOverScene::showCredits(cocos2d::Ref* pSender) {
	Audio::Instance()->playFX(BUTTON_FX);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionZoomFlipY::create(0.5, CreditsScene::createScene()));		// Restart the current scene	
	this->scheduleUpdate();																										// reschedule
}

/*
	Achievement: Player has achieved a new high score
*/
std::string GameOverScene::scoreAchievement(int highScore) {
	std::string scoreTxt = "";

	if (Game::Instance()->getScore() > highScore) {
		scoreTxt = "New High Score: " + cocos2d::StringUtils::toString(Game::Instance()->getScore());

		Achievement::Instance()->analyticNewHighScore();																		// Achievement: New high score set
	}

	return scoreTxt;
}