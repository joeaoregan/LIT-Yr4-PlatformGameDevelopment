#include "Player.h"
//#include "DPad.h"
#include "Input.h"
#include "Level.h"

//DPad *controller;																	// Add directional pad for mobile device

#define PLAYER_SPEED 3.0f
#define PLAYER_COLLISION_BITMASK 0x000001

Player::Player(cocos2d::Layer *layer) {
	visibleSize = Director::getInstance()->getVisibleSize();						// screen size
	origin = Director::getInstance()->getVisibleOrigin();							// origin coorinate

	player = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
	player->setPosition(visibleSize.width * 0.1, visibleSize.height * 0.5);			// Place in middle left of screen
	player->setScale((visibleSize.height == 720) ? 1.0f : 1.5f);					// Increase scale of player for Android (My phone anyway)
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
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
		if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) ||
            Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
			moveLeft();
		}
		else if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ||
                 Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
			moveRight();
		}

		if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) ||
            Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
			moveUp();
		}
		else if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) ||
                 Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
			moveDown();
		}
	}
/*
	// Android DPad
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (controller->getButton(8)->isSelected()) {
			moveUp();
		}
		else if (controller->getButton(2)->isSelected()) {
			moveDown();
		}
		if (controller->getButton(4)->isSelected()) {
			moveLeft();
		}
		else if (controller->getButton(6)->isSelected()) {
			moveRight();
		}
	} 
 // Android DPad
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (DPad::Instance()->getButton(8)->isSelected()) moveUp();
		else if (DPad::Instance()->getButton(2)->isSelected()) moveDown();

		if (DPad::Instance()->getButton(4)->isSelected()) moveLeft();
		else if (DPad::Instance()->getButton(6)->isSelected()) moveRight();
	}
	*/
}

void Player::moveUp() {
	player->setPosition(player->getPosition().x, player->getPosition().y + PLAYER_SPEED);
}
void Player::moveDown() {
	player->setPosition(player->getPosition().x, player->getPosition().y - PLAYER_SPEED);
}
void Player::moveLeft() {
	player->setPosition(player->getPosition().x - PLAYER_SPEED, player->getPosition().y);
}
void Player::moveRight() {
	player->setPosition(player->getPosition().x + PLAYER_SPEED, player->getPosition().y);
}
/*
void Player::spawnLasers(int amount, cocos2d::Layer *layer) {
	int i;
	for ((amount == 2) ? i = 1 : i = 0; i < amount; i++) {
		cocos2d::Sprite* shipLaser = Level::Instance()->getLaserList()->at(Level::Instance()->getNextShipLaser()+1);	// Next laser in the list, JOR replaced auto specifier
		Level::Instance()->setNextShipLaser(Level::Instance()->getNextShipLaser() + 1);
		if (Level::Instance()->getNextShipLaser() >= Level::Instance()->getLaserList()->size())
			Level::Instance()->setNextShipLaser(0);																		// Reset laser list index to 0 (go back to start of list)

		if (i == 0) shipLaser->setPosition(player->getPosition() + Point(shipLaser->getContentSize().width / 2, 0));	// middle
		if (i == 1) shipLaser->setPosition(player->getPosition() + Point(shipLaser->getContentSize().width / 2, -12));	// bottom
		if (i == 2) shipLaser->setPosition(player->getPosition() + Point(shipLaser->getContentSize().width / 2, 12));	// top

		shipLaser->setVisible(true);
		shipLaser->stopAllActions();

		shipLaser->runAction(
			Sequence::create(MoveBy::create(0.5, Point(Level::Instance()->getWinSize().width, 0)),						// change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, layer)), NULL));
	}
}
*/