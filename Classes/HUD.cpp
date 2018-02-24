#include "Game.h"
#include "HUD.h"

#define SCREEN_RESOLUTION_HEIGHT 720

HUD* HUD::s_pInstance;							// Singleton for Heads Up Display

void HUD::init(cocos2d::Layer *layer) {
	//score = 0;
	//Game::Instance()->setScore(0);
	//level = 1;
	//Game::Instance()->setLevel(1);
	//time = LEVEL_TIME / 1000;					// Should be 30 seconds
	/*
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	__String *tempLevel = __String::createWithFormat("Level: %i", Game::Instance()->getLevel());
	//__String *tempScore = __String::createWithFormat("Score: %i", score);

	// Level
	if (visibleSize.height == 1080) {
        levelLabel = Label::createWithTTF(tempLevel->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.075f);
        levelLabel->setPosition(Point(125 + origin.x, visibleSize.height * 0.95 + origin.y));
    }
	else {
        levelLabel = Label::createWithTTF(tempLevel->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.05f);
        levelLabel->setPosition(Point(75 + origin.x, visibleSize.height * 0.95 + origin.y));
    }
	levelLabel->setColor(Color3B::WHITE);
	layer->addChild(levelLabel, 10000);
	
	// Score
	scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.05f);
	scoreLabel->setColor(Color3B::WHITE);
	scoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.95 + origin.y));
	layer->addChild(scoreLabel, 10000);
	*/
	currentTime = 0.0f;																							// Current game time, for timer, changed to float to solve Android timer issue
}

HUD *HUD::create(cocos2d::Point position, cocos2d::Size res) {
	s_pInstance = new HUD();

	s_pInstance->time = Game::Instance()->getTimer();															// The timer is sit in level init

	if (true) {
		s_pInstance->autorelease();																				// Set to autorelease

		__String *tempLevel = __String::createWithFormat("Level: %i", Game::Instance()->getLevel());
		__String *tempScore = __String::createWithFormat("Score: %i", Game::Instance()->getScore());
		__String *tempTime = __String::createWithFormat("Time: %i", s_pInstance->time);

		// Current Level
		s_pInstance->levelLabel = Label::createWithTTF(tempLevel->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->levelLabel->setPosition(Point(position.x + res.width * 0.05f, position.y + res.height * 0.95));
		s_pInstance->levelLabel->setColor(Color3B::WHITE);
		s_pInstance->addChild(s_pInstance->levelLabel);

		// Current score
		s_pInstance->scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->scoreLabel->setColor(Color3B::WHITE);
		s_pInstance->scoreLabel->setPosition(Point(res.width / 2 + position.x, res.height * 0.95 + position.y));
		s_pInstance->addChild(s_pInstance->scoreLabel, 10000);

		// Countdown Timer
		s_pInstance->timeLabel = Label::createWithTTF(tempTime->getCString(), "fonts/Marker Felt.ttf", res.height * 0.05f);
		s_pInstance->timeLabel->setPosition(Point((res.width - s_pInstance->timeLabel->getWidth()) * 0.95f, res.height * 0.95 + position.y));
		s_pInstance->timeLabel->setColor(Color3B::WHITE);
		s_pInstance->addChild(s_pInstance->timeLabel);

		// Player number of lives
		for (unsigned int i = 0; i < MAX_LIVES; i++) {
			s_pInstance->playerLife = Sprite::create("PlayerLife.png");
			//s_pInstance->playerLife->setPosition(visibleSize.width * 0.05 + (i * 52), visibleSize.height * 0.05);
			s_pInstance->playerLife->setPosition(position.x + SCREEN_RESOLUTION_HEIGHT * 0.05f + (i * 52), SCREEN_RESOLUTION_HEIGHT * 0.05f);
			s_pInstance->addChild(s_pInstance->playerLife);
			s_pInstance->livesList[i] = s_pInstance->playerLife;																				// Add life sprite to list of lives
		}
		s_pInstance->livesList[3]->setVisible(false);
		s_pInstance->livesList[4]->setVisible(false);

		return s_pInstance;
	}

	CC_SAFE_DELETE(s_pInstance);
	return NULL;
}

void HUD::update(float curTimeMillis) {
	// If the players lives are less than 3
	if (Game::Instance()->getLives() < MAX_LIVES && !Game::Instance()->isGameOver()) {		// If the players lives are less than the max num lives
		livesList[Game::Instance()->getLives()]->setVisible(false);							// Set the lives invisible (2,1,0)
	}
	
	scoreLabel->setString("Score: " + to_string(Game::Instance()->getScore()));				// Update the displayed score
	timeLabel->setString("Time: " + to_string(Game::Instance()->getTimer()));

	updateTimer(curTimeMillis);
}

/*
	Updates the displayed lives when the player collides with an object
*/
void HUD::updateLives() {
	livesList[Game::Instance()->getLives()]->setVisible(false);										// Set the player lives invisible (in the order 2,1,0)
}

/*
update the HUD countdown timer
*/
void HUD::updateTimer(float curTimeMillis) {
	if (curTimeMillis > currentTime) {						// If the game timer has reached the indicated point (every 1 second)
		currentTime = curTimeMillis + 1000.0f;				// Set next time to reach
		time--;												// Decrement the countdown timer
	}

	timeLabel->setString("Time: " + to_string(time));		// Update the countdown timers display
}

void HUD::setLevel(unsigned int set) {
	levelLabel->setString("Level: " + to_string(set));
}