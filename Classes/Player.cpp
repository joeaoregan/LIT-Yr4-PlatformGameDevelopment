#include "Player.h"

USING_NS_CC;

#define PLAYER_MOVEMENT_SPEED 0.0025
#define PLAYER_COLLISION_BITMASK 0x000001

Player::Player(cocos2d::Layer *layer) {
	visibleSize = Director::getInstance()->getVisibleSize();										// screen size
	origin = Director::getInstance()->getVisibleOrigin();											// origin coorinate

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

}