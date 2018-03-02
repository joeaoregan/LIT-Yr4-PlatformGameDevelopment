/*
	Player.cpp

	Joe O'Regan
	K00203642

	Player class is a sub class of Sprite
*/
#include "Player.h"
#include "Input.h"
#include "Level.h"

/*
	Create a player sprite, initiallising the number of lasers, 
	and scale depending on screen resolution
*/
Player* Player::create(cocos2d::Size res) {
	Player* player = new Player();
	player->visibleSize = Director::getInstance()->getVisibleSize();						// screen size
	player->scale = (res.height == 720) ? 1.0f : 1.5f;
	
	//if (player && player->initWithSpriteFrameName("SpaceFlier_sm_1.png")) { //xxxxxxxxxx
	if (player && player->initWithFile("PlayerShip3.png")) {
		// Set the amount of lasers to fire at the beginning
		if (Game::Instance()->getLevel() <= 1) {
			if (Game::Instance()->getDifficulty() == EASY)
				player->m_weaponStrength = 3;													// Start with 3 lasers easy, 2 medium, 1 hard
			else if (Game::Instance()->getDifficulty() == HARD) {
				player->m_weaponStrength = 1;													// Less laser beams initially
			}
		}
		else
			player->m_weaponStrength = Game::Instance()->getCurrentWeapon();

		player->autorelease();
		player->setPosition(res.width * 0.1, res.height * 0.5);								// Place in middle left of screen
		player->setScale(player->scale);													// Increase scale of player for Android (My phone anyway)

		player->canon = Sprite::create("PlayerShipGun.png");
		player->canon->setVisible(false);
		//player->canon->setPosition((player->getPosition().x / 2) + player->getContentSize().width * 0.25f, (player->getPosition().y) / 2);
		player->canon->setPosition((player->getContentSize().width / 2) + player->getContentSize().width * 0.4f, player->getContentSize().height / 2);
		player->addChild(player->canon);
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

/*
	Get input from keyboard to update the players position
*/
void Player::update() {
	// Move the player ship
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX ||
		CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
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

/*
	Move the player
*/
void Player::moveUp() {
	if (getPosition().y < visibleSize.height * 0.895f)
		setPosition(getPosition().x, getPosition().y + PLAYER_SPEED * scale);				// Need to cover more distance on larger screen
}
void Player::moveDown() {
	if (getPosition().y > visibleSize.height * 0.125f)
		setPosition(getPosition().x, getPosition().y - PLAYER_SPEED * scale);
}
void Player::moveLeft() {
	if (getPosition().x > getContentSize().width / 4)
		setPosition(getPosition().x - PLAYER_SPEED * scale, getPosition().y);
}
void Player::moveRight() {
	if (getPosition().x < visibleSize.width - (getContentSize().width / 2))
		setPosition(getPosition().x + PLAYER_SPEED * scale, getPosition().y);
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