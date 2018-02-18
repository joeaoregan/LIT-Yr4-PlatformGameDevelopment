#include "Game.h"
#include "HUD.h"

HUD* HUD::s_pInstance;							// Singleton for Heads Up Display

void HUD::init(unsigned int &time, cocos2d::Layer *layer) {
	//score = 0;
	//Game::Instance()->setScore(0);
	//level = 1;
	//Game::Instance()->setLevel(1);
	time = LEVEL_TIME / 1000;			// Should be 30 seconds

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

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
	/*
	// Score
	scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.05f);
	scoreLabel->setColor(Color3B::WHITE);
	scoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.95 + origin.y));
	layer->addChild(scoreLabel, 10000);
	*/
}