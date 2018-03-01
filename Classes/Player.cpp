#include "Player.h"
#include "Input.h"
#include "Level.h"

#define PLAYER_SPEED 3.0f
//#define PLAYER_COLLISION_BITMASK 0x000001

Player* Player::create(cocos2d::Size res) {
	Player* player = new Player();
	player->visibleSize = Director::getInstance()->getVisibleSize();					// screen size

	if (player && player->initWithSpriteFrameName("SpaceFlier_sm_1.png")) {

		player->autorelease();
		player->setPosition(res.width * 0.1, res.height * 0.5);			// Place in middle left of screen
		player->setScale((res.height == 720) ? 1.0f : 1.5f);					// Increase scale of player for Android (My phone anywa
	}
	else {
		delete player;
	}

	return player;
}

/*(
Player::Player(cocos2d::Layer *layer) {
	visibleSize = Director::getInstance()->getVisibleSize();						// screen size
	origin = Director::getInstance()->getVisibleOrigin();							// origin coorinate

	player = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
	player->setPosition(visibleSize.width * 0.1, visibleSize.height * 0.5);			// Place in middle left of screen
	player->setScale((visibleSize.height == 720) ? 1.0f : 1.5f);					// Increase scale of player for Android (My phone anyway)

	//auto playerBody = PhysicsBody::createCircle(player->getContentSize().width / 2);
	//playerBody->setCollisionBitmask(PLAYER_COLLISION_BITMASK);
	//playerBody->setContactTestBitmask(true);

	player->setPhysicsBody(playerBody);

	//layer->addChild(player, 10);	// _batchNode->addChild(player->getSprite());
}
*/

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
}

void Player::moveUp() {
	if (getPosition().y < visibleSize.height * 0.9f)
		setPosition(getPosition().x, getPosition().y + PLAYER_SPEED);
}
void Player::moveDown() {
	if (getPosition().y > visibleSize.height * 0.125f)
		setPosition(getPosition().x, getPosition().y - PLAYER_SPEED);
}
void Player::moveLeft() {
	if (getPosition().x > getContentSize().width / 4)
		setPosition(getPosition().x - PLAYER_SPEED, getPosition().y);
}
void Player::moveRight() {
	if (getPosition().x < visibleSize.width - (getContentSize().width / 2))
		setPosition(getPosition().x + PLAYER_SPEED, getPosition().y);
}

/*
void Player::moveUp() {
	if (getPosition().y < visibleSize.height * 0.9f)
		player->setPosition(player->getPosition().x, player->getPosition().y + PLAYER_SPEED);
}
void Player::moveDown() {
	if (player->getPosition().y > visibleSize.height * 0.125f)
		player->setPosition(player->getPosition().x, player->getPosition().y - PLAYER_SPEED);
}
void Player::moveLeft() {
	if (player->getPosition().x > player->getContentSize().width / 4)
		player->setPosition(player->getPosition().x - PLAYER_SPEED, player->getPosition().y);
}
void Player::moveRight() {
	if (player->getPosition().x < visibleSize.width - (player->getContentSize().width / 2))
		player->setPosition(player->getPosition().x + PLAYER_SPEED, player->getPosition().y);
}

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