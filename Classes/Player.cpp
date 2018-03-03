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
	Create a player sprite, Initialise the number of lasers spawned based on the game difficulty
	and scale depending on screen resolution
	

*/
Player* Player::create(cocos2d::Size res) {
	Player* player = new Player();
	player->visibleSize = Director::getInstance()->getVisibleSize();						// screen size
	player->scale = (res.height == 720) ? 0.67f : 1.0f;										// This image has a higher res than the space game so is scaled down for PC, not up for mobile
	
	//if (player && player->initWithSpriteFrameName(PLAYER_OLD_IMG)) {						// Original Player on sprite sheet
	if (player && player->initWithFile(PLAYER_IMG)) {
		if (Game::Instance()->getLevel() <= 1) {											// Set the amount of lasers to fire starting at level 1, based on difficulty
			if (Game::Instance()->getDifficulty() == EASY)
				player->m_weaponStrength = 3;												// Start with 3 lasers easy, 2 medium, 1 hard
			else if (Game::Instance()->getDifficulty() == HARD) {
				player->m_weaponStrength = 1;												// Less laser beams initially
			}
		}
		else
			player->m_weaponStrength = Game::Instance()->getCurrentWeapon();

		player->autorelease();																// Clear the object automatically from memory
		player->setPosition(res.width * 0.1, res.height * 0.5);								// Place in middle left of screen
		player->setScale(player->scale);													// Increase scale of player for Android (My phone anyway)

		// Weapon graphic
		player->m_pCanon = Sprite::create(PLAYER_CANON);										// The weapon on the players ship
		player->m_pCanon->setVisible(false);													// Hide the canon to begin with
		player->m_pCanon->setPosition((player->getContentSize().width / 2) +					// Set its position relative to the Player Sprite
			player->getContentSize().width * 0.4f, player->getContentSize().height / 2);
		player->addChild(player->m_pCanon);													// Attach to player
	}
	else {
		delete player;
	}

	return player;
}

/*
	Get input from keyboard to update the players position
	This is platform dependent, when running unchecked on Android the virtual keyboard pops up
	expecting data to be input at the beginning of the level scene
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
	Upgrade the players weapon, set currently used weapon (up to level 4), 
	set the canon visible the first time the weapon is upgraded
*/
void Player::upgradeWeapon() {
	weaponUpgrade();
	m_pCanon->setVisible(true);
	Game::Instance()->setCurrentWeapon(m_weaponStrength);
}
