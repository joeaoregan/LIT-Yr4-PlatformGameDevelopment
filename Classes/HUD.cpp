#include "Game.h"
#include "HUD.h"
#include "MainMenu.h"

HUD* HUD::s_pInstance;																							// Singleton for Heads Up Display

void HUD::init(cocos2d::Layer *layer) {
	currentTime = 0.0f;																							// Current game time, for timer, changed to float to solve Android timer issue
}

HUD *HUD::create(cocos2d::Point position, cocos2d::Size res) {
	s_pInstance = new HUD();

	s_pInstance->visibleSize = Director::getInstance()->getVisibleSize();

	s_pInstance->time = Game::Instance()->getTimer();															// The timer is sit in level init

	if (true) {
		s_pInstance->autorelease();																				// Set to autorelease

		__String *tempLevel = __String::createWithFormat("Level: %i", Game::Instance()->getLevel());
		__String *tempScore = __String::createWithFormat("Score: %i", Game::Instance()->getScore());
		__String *tempTime = __String::createWithFormat("Time: %i", s_pInstance->time);

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
		topPanel->setPosition(cocos2d::Point(s_pInstance->visibleSize.width / 2, (res.height * 0.995f) - ((topPanel->getContentSize().height * topPanel->getScaleY()) / 2)));
		s_pInstance->addChild(topPanel);

		// Menu Icon
		s_pInstance->menuItem = cocos2d::MenuItemImage::create("MenuWR.png", "MenuWG.png");
		s_pInstance->menuItem->setScale(scaleDown * 1.25f);
		s_pInstance->menuItem->setPosition(cocos2d::Point((position.x + res.width * 0.05f) * 0.99f, res.height * 0.95 + position.y));

		// Current Level
		s_pInstance->levelLabel = Label::createWithTTF(tempLevel->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->levelLabel->setTextColor(Color4B::RED);
		s_pInstance->levelLabel->enableOutline(Color4B::WHITE, 3);
		s_pInstance->levelLabel->setScale(scaleUp);																				// Scale up the image
		s_pInstance->levelLabel->setPosition(Point((position.x + res.width * 0.1f) + s_pInstance->menuItem->getContentSize().width, position.y + res.height * 0.95));			// Then set the position
		s_pInstance->addChild(s_pInstance->levelLabel);

		// Current score
		s_pInstance->scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->scoreLabel->setTextColor(Color4B::RED);
		s_pInstance->scoreLabel->enableOutline(Color4B::WHITE, 3);
		s_pInstance->scoreLabel->setScale(scaleUp);
		s_pInstance->scoreLabel->setPosition(Point(res.width / 2 + position.x, position.y + res.height * 0.95));
		s_pInstance->addChild(s_pInstance->scoreLabel, 10000);

		// Countdown Timer
		s_pInstance->timeLabel = Label::createWithTTF(tempTime->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->timeLabel->setTextColor(Color4B::RED);
		s_pInstance->timeLabel->enableOutline(Color4B::WHITE, 3);
		s_pInstance->timeLabel->setScale(scaleUp);
		s_pInstance->timeLabel->setPosition(Point(((res.width * 0.925f) - s_pInstance->timeLabel->getWidth()), res.height * 0.95 + position.y));
		s_pInstance->addChild(s_pInstance->timeLabel);

		// Player number of lives
		for (unsigned int i = 0; i < MAX_PLAYER_LIVES; i++) {
			s_pInstance->playerLife = Sprite::create("PlayerLifeNew2.png");
			//s_pInstance->playerLife->setPosition(visibleSize.width * 0.05 + (i * 52), visibleSize.height * 0.05);
			s_pInstance->playerLife->setScale(scaleDown); // 20180302 Fixed
			s_pInstance->playerLife->setPosition(position.x + res.width * 0.05f + (i * (s_pInstance->playerLife->getContentSize().width * scaleDown)), res.height * 0.05f);
			s_pInstance->addChild(s_pInstance->playerLife);
			s_pInstance->livesList[i] = s_pInstance->playerLife;																				// Add life sprite to list of lives
		}
		if (Game::Instance()->getDifficulty() > EASY) {
			s_pInstance->livesList[3]->setVisible(false);
			s_pInstance->livesList[4]->setVisible(false);
		}

		// Add exit button in bottom right corner. it's an autorelease object	
		s_pInstance->closeItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png");	
		s_pInstance->closeItem->setScale(scaleUp);
		s_pInstance->closeItem->setPosition(cocos2d::Point((position.x + res.width - s_pInstance->closeItem->getContentSize().width / 2) * 0.975f, res.height * 0.05f));

		// Create A Menu, it's an autorelease object
		s_pInstance->menuClose = cocos2d::Menu::create(s_pInstance->closeItem, s_pInstance->menuItem, NULL);															// JOR replaced auto specifier
		s_pInstance->menuClose->setPosition(cocos2d::Point::ZERO);
		s_pInstance->addChild(s_pInstance->menuClose, 1);

		return s_pInstance;
	}

	CC_SAFE_DELETE(s_pInstance);
	return NULL;
}

void HUD::update(float curTimeMillis) {
	// If the players lives are less than max lives
	if (Game::Instance()->getLives() <= MAX_PLAYER_LIVES && !Game::Instance()->isGameOver()) {				// If the players lives are less than the max num lives
		for (unsigned int i = 0; i < MAX_PLAYER_LIVES; i++) {
			if (i < Game::Instance()->getLives()) {
				if (livesList[i]->isVisible()) continue;
				livesList[i]->setVisible(true);																// Set the lives invisible (2,1,0

				// Animate a new life on HUD
				auto action1 = cocos2d::MoveTo::create(0.25f,												// Move the life up 20% of its height
					cocos2d::Point(livesList[i]->getPosition().x, livesList[i]->getPosition().y +
						livesList[i]->getContentSize().height * 0.2f));
				auto action2 = cocos2d::ScaleTo::create(0.25f, (visibleSize.height == 1080) ? 1.5f : 1.0f);	// Increase the size by 50%
				auto action3 = cocos2d::MoveTo::create(0.25f, cocos2d::Point(livesList[i]->getPosition().x, 
					livesList[i]->getPosition().y));
				auto action4 = cocos2d::ScaleTo::create(0.25f, (visibleSize.height == 1080) ? 1.0f : 0.67f);	// Decrease the size to original values
				auto sequence = cocos2d::Sequence::create(Spawn::create(action1, action2, nullptr),			// Create a sequence, using spawn to run the move and scale actions concurrently
					Spawn::create(action3, action4, nullptr), nullptr);										// Run the reverse actions immediately after
				livesList[i]->runAction(sequence);
				
			}
			else {
				if (!livesList[i]->isVisible()) continue;
				livesList[i]->setVisible(false);															// Set the lives invisible (order 4,3,2,1,0)
			}
		}
	}

	scoreLabel->setString("Score: " + StringUtils::toString(Game::Instance()->getScore()));					// Update the displayed score text
	timeLabel->setString("Time: " + StringUtils::toString(Game::Instance()->getTimer()));					// Update the countdown timer text

	updateTimer(curTimeMillis);																				// Update the timer

	if (s_pInstance->closeItem->isSelected()) {																// If the close menu button is selected
		Audio::Instance()->selectMenuOption();
		cocos2d::Director::getInstance()->end();															// Close the director

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}

	if (s_pInstance->menuItem->isSelected()) {
		Audio::Instance()->selectMenuOption();
		Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5, MainMenu::createScene()));	// Return to the main menu
	}
}

/*
	update the HUD countdown timer
*/
void HUD::updateTimer(float curTimeMillis) {
	if (curTimeMillis > currentTime) {																		// If the game timer has reached the indicated point (every 1 second)
		currentTime = curTimeMillis + 1000.0f;																// Set next time to reach
		time--;																								// Decrement the countdown timer
	}

	timeLabel->setString("Time: " + StringUtils::toString(time));											// Update the countdown timers display
}

/*
	Update the displayed level number at the start of each level
*/
void HUD::setLevelLabel() {
	levelLabel->setString("Level: " + StringUtils::toString(Game::Instance()->getLevel()));
	//CCLOG("Level Number Changed - Current Level: %d", Game::Instance()->getLevel());
}