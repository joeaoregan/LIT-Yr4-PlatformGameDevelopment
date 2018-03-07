/*
	HUD.h

	Joe O'Regan
	K00203642

	Heads up display
	Shows level number, score, and time remaining each level
*/

#include "Game.h"
#include "HUD.h"
#include "MainMenu.h"

HUD* HUD::s_pInstance;																							// Singleton for Heads Up Display

void HUD::init(cocos2d::Layer *layer) {
	m_currentTime = 0.0f;																						// Current game time, for timer, changed to float to solve Android timer issue
}

/*
	Create the heads up display
*/
HUD *HUD::create(cocos2d::Point position, cocos2d::Size res) {
	s_pInstance = new HUD();

	s_pInstance->m_visibleSize = Director::getInstance()->getVisibleSize();

	s_pInstance->m_time = Game::Instance()->getTimer();															// The timer is sit in level init

	if (true) {
		s_pInstance->autorelease();																				// Set to autorelease

		__String *tempLevel = __String::createWithFormat("Level: %i", Game::Instance()->getLevel());
		__String *tempScore = __String::createWithFormat("Score: %i", Game::Instance()->getScore());
		__String *tempTime = __String::createWithFormat("Time: %i", s_pInstance->m_time);

		// Scale up text for my Android phone
		float scaleUp, scaleDown;
		(res.height == 1080) ? scaleUp = 1.45f : scaleUp = 1.0f;
		(res.height == 1080) ? scaleDown = 1.0f : scaleDown = 0.67f;

		// Bottom panel
		cocos2d::Sprite* bottomPanel = Sprite::create("Panel.png");
		bottomPanel->setScale(scaleDown);
		bottomPanel->setPosition(cocos2d::Point(res.width / 2, (bottomPanel->getContentSize().height / 2) * scaleDown));
		s_pInstance->addChild(bottomPanel);

		// Top panel
		cocos2d::Sprite* topPanel = Sprite::create("Panel.png");
		topPanel->setScale(scaleDown);
		topPanel->setScaleY(topPanel->getScaleY() * 0.80f);
		topPanel->setPosition(cocos2d::Point(s_pInstance->m_visibleSize.width / 2, 
			(res.height * 0.995f) - ((topPanel->getContentSize().height * topPanel->getScaleY()) / 2)));
		s_pInstance->addChild(topPanel);

		// Menu Icon
		s_pInstance->m_pMenuItem = cocos2d::MenuItemImage::create("MenuWR.png", "MenuWG.png");
		s_pInstance->m_pMenuItem->setScale(scaleDown * 1.25f);
		s_pInstance->m_pMenuItem->setPosition(cocos2d::Point((position.x + res.width * 0.05f) * 0.99f, res.height * 0.95 + position.y));

		// Current Level
		s_pInstance->m_pLevelLabel = Label::createWithTTF(tempLevel->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->m_pLevelLabel->setTextColor(Color4B::RED);
		s_pInstance->m_pLevelLabel->enableOutline(Color4B::WHITE, 3);
		s_pInstance->m_pLevelLabel->setScale(scaleUp);																				// Scale up the image
		s_pInstance->m_pLevelLabel->setPosition(Point((position.x + res.width * 0.1f) + 
			s_pInstance->m_pMenuItem->getContentSize().width, position.y + res.height * 0.95));			// Then set the position
		s_pInstance->addChild(s_pInstance->m_pLevelLabel);

		// Current score
		s_pInstance->m_pScoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->m_pScoreLabel->setTextColor(Color4B::RED);
		s_pInstance->m_pScoreLabel->enableOutline(Color4B::WHITE, 3);
		s_pInstance->m_pScoreLabel->setScale(scaleUp);
		s_pInstance->m_pScoreLabel->setPosition(Point(res.width / 2 + position.x, position.y + res.height * 0.95));
		s_pInstance->addChild(s_pInstance->m_pScoreLabel, 10000);

		// Countdown Timer
		s_pInstance->m_pTimeLabel = Label::createWithTTF(tempTime->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->m_pTimeLabel->setTextColor(Color4B::RED);
		s_pInstance->m_pTimeLabel->enableOutline(Color4B::WHITE, 3);
		s_pInstance->m_pTimeLabel->setScale(scaleUp);
		s_pInstance->m_pTimeLabel->setPosition(Point(((res.width * 0.925f) - s_pInstance->m_pTimeLabel->getWidth()), 
			res.height * 0.95 + position.y));
		s_pInstance->addChild(s_pInstance->m_pTimeLabel);

		// Player number of lives
		for (unsigned int i = 0; i < MAX_PLAYER_LIVES; i++) {
			s_pInstance->m_pPlayerLife = Sprite::create("PlayerLifeNew2.png");
			//s_pInstance->playerLife->setPosition(visibleSize.width * 0.05 + (i * 52), visibleSize.height * 0.05);
			s_pInstance->m_pPlayerLife->setScale(scaleDown); // 20180302 Fixed
			s_pInstance->m_pPlayerLife->setPosition(position.x + res.width * 0.05f + 
				(i * (s_pInstance->m_pPlayerLife->getContentSize().width * scaleDown)), res.height * 0.05f);
			s_pInstance->addChild(s_pInstance->m_pPlayerLife);
			s_pInstance->m_pLivesList[i] = s_pInstance->m_pPlayerLife;																				// Add life sprite to list of lives
		}
		if (Game::Instance()->getDifficulty() > EASY) {
			s_pInstance->m_pLivesList[3]->setVisible(false);
			s_pInstance->m_pLivesList[4]->setVisible(false);
		}

		// Add exit button in bottom right corner. it's an autorelease object	
		s_pInstance->m_pCloseItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png");	
		s_pInstance->m_pCloseItem->setScale(scaleUp);
		s_pInstance->m_pCloseItem->setPosition(cocos2d::Point((position.x + res.width - s_pInstance->m_pCloseItem->getContentSize().width / 2) * 0.975f, res.height * 0.05f));

		// Create A Menu, it's an autorelease object
		s_pInstance->m_pMenuClose = cocos2d::Menu::create(s_pInstance->m_pCloseItem, s_pInstance->m_pMenuItem, NULL);															// JOR replaced auto specifier
		s_pInstance->m_pMenuClose->setPosition(cocos2d::Point::ZERO);
		s_pInstance->addChild(s_pInstance->m_pMenuClose, 1);

		return s_pInstance;
	}

	CC_SAFE_DELETE(s_pInstance);
	return NULL;
}

/*
	Update the heads up display, animating a new life
*/
void HUD::update(float curTimeMillis) {
	// If the players lives are less than max lives
	if (Game::Instance()->getLives() <= MAX_PLAYER_LIVES && !Game::Instance()->isGameOver()) {				// If the players lives are less than the max num lives
		for (unsigned int i = 0; i < MAX_PLAYER_LIVES; i++) {
			if (i < Game::Instance()->getLives()) {
				if (m_pLivesList[i]->isVisible()) continue;
				m_pLivesList[i]->setVisible(true);																// Set the lives invisible (2,1,0

				// Animate a new life on HUD
				auto action1 = cocos2d::MoveTo::create(0.25f,												// Move the life up 20% of its height
					cocos2d::Point(m_pLivesList[i]->getPosition().x, m_pLivesList[i]->getPosition().y +
						m_pLivesList[i]->getContentSize().height * 0.2f));
				auto action2 = cocos2d::ScaleTo::create(0.25f, (m_visibleSize.height == 1080) ? 1.5f : 1.0f);	// Increase the size by 50%
				auto action3 = cocos2d::MoveTo::create(0.25f, cocos2d::Point(m_pLivesList[i]->getPosition().x, 
					m_pLivesList[i]->getPosition().y));
				auto action4 = cocos2d::ScaleTo::create(0.25f, (m_visibleSize.height == 1080) ? 1.0f : 0.67f);	// Decrease the size to original values
				auto sequence = cocos2d::Sequence::create(Spawn::create(action1, action2, nullptr),			// Create a sequence, using spawn to run the move and scale actions concurrently
					Spawn::create(action3, action4, nullptr), nullptr);										// Run the reverse actions immediately after
				m_pLivesList[i]->runAction(sequence);
				
			}
			else {
				if (!m_pLivesList[i]->isVisible()) continue;
				m_pLivesList[i]->setVisible(false);															// Set the lives invisible (order 4,3,2,1,0)
			}
		}
	}

	m_pScoreLabel->setString("Score: " + StringUtils::toString(Game::Instance()->getScore()));				// Update the displayed score text
	m_pTimeLabel->setString("Time: " + StringUtils::toString(Game::Instance()->getTimer()));				// Update the countdown timer text

	updateTimer(curTimeMillis);																				// Update the timer

	if (s_pInstance->m_pCloseItem->isSelected()) {															// If the close menu button is selected
		Audio::Instance()->playFX(BUTTON_FX);
		cocos2d::Director::getInstance()->end();															// Close the director

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}

	if (s_pInstance->m_pMenuItem->isSelected()) {
		Audio::Instance()->playFX(BUTTON_FX);
		Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5, MainMenu::createScene()));	// Return to the main menu
	}
}

/*
	update the HUD countdown timer
*/
void HUD::updateTimer(float curTimeMillis) {
	if (curTimeMillis > m_currentTime) {																		// If the game timer has reached the indicated point (every 1 second)
		m_currentTime = curTimeMillis + 1000.0f;																// Set next time to reach
		m_time--;																								// Decrement the countdown timer
	}

	m_pTimeLabel->setString("Time: " + StringUtils::toString(m_time));											// Update the countdown timers display
}

/*
	Update the displayed level number at the start of each level
*/
void HUD::setLevelLabel() {
	m_pLevelLabel->setString("Level: " + StringUtils::toString(Game::Instance()->getLevel()));
	//CCLOG("Level Number Changed - Current Level: %d", Game::Instance()->getLevel());
}