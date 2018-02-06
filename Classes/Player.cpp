#include "Player.h"
//#include "DPad.h"
#include "Input.h"

USING_NS_CC;

//DPad *controller;																	// Add directional pad for mobile device

#define PLAYER_SPEED 3.0f
#define PLAYER_COLLISION_BITMASK 0x000001

Player::Player(cocos2d::Layer *layer) {
	visibleSize = Director::getInstance()->getVisibleSize();						// screen size
	origin = Director::getInstance()->getVisibleOrigin();							// origin coorinate

	player = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
	player->setPosition(visibleSize.width * 0.1, visibleSize.height * 0.5);
/*
	auto playerBody = PhysicsBody::createCircle(player->getContentSize().width / 2);
	playerBody->setCollisionBitmask(PLAYER_COLLISION_BITMASK);
	playerBody->setContactTestBitmask(true);

	player->setPhysicsBody(playerBody);
*/
	//layer->addChild(player, 10);	// _batchNode->addChild(player->getSprite());
}

void Player::update() {
	// Move the player ship
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) {
		if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
			player->setPosition(player->getPosition().x - PLAYER_SPEED, player->getPosition().y);
		}
		else if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
			player->setPosition(player->getPosition().x + PLAYER_SPEED, player->getPosition().y);
		}

		if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW)) {
			player->setPosition(player->getPosition().x, player->getPosition().y + PLAYER_SPEED);
		}
		else if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
			player->setPosition(player->getPosition().x, player->getPosition().y - PLAYER_SPEED);
		}
	}
}