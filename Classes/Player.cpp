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
	player->m_visibleSize = Director::getInstance()->getVisibleSize();						// screen size
	//player->m_scale = (res.height == 720) ? 0.67f : 1.0f;									// This image has a higher res than the space game so is scaled down for PC, not up for mobile
	player->m_scale = (res.height == 1080) ? 1.0f : (res.height == 720) ? 0.67f : res.height / 1080;	// Kindle resolution is different
	
	if (player && player->initWithSpriteFrameName("PlayerShip3.png")) {						// Original Player on sprite sheet
	//if (player && player->initWithFile(PLAYER_IMG)) {
		if (Game::Instance()->getLevel() <= 1) {											// Set the amount of lasers to fire starting at level 1, based on difficulty
			if (Game::Instance()->getDifficulty() == EASY) {
				player->m_weaponStrength = 3;												// Start with 3 lasers easy, 2 medium, 1 hard
				player->m_maxHealth = 10.0f;												// Set max health (number of times player can be hit before losing a life)
				player->m_lives = 4;
			}
			else if (Game::Instance()->getDifficulty() == HARD) {
				player->m_weaponStrength = 1;												// Less laser beams initially
				player->m_maxHealth = 3.0f;
				player->m_lives = 2;
			}

			Game::Instance()->setHealth((unsigned int) player->m_maxHealth);				// Store the max health for the player
			Game::Instance()->setLives(player->m_lives);									// Init the number of lives
		}
		else
			player->m_weaponStrength = Game::Instance()->getCurrentWeapon();

		player->m_lives = Game::Instance()->getLives();
		//if (Game::Instance()->getLevel() == 1)
		//	Game::Instance()->setHealth(player->m_maxHealth);								// Initialise the player health
		player->m_health = Game::Instance()->getHealth();
		if (player->m_health <= 0) player->m_health = player->m_maxHealth;					// Had the health carry over as 0 in one test

		cocos2d::Color4F fgColour(0.6f, 0.77f, 0.96f, 1);
		player->initHealthBar(res, fgColour);												// Initialise the healthbar

		player->autorelease();																// Clear the object automatically from memory
		player->setPosition(res.width * 0.1, res.height * 0.5);								// Place in middle left of screen
		player->setScale(player->m_scale);													// Increase scale of player for Android (My phone anyway)

		// Weapon graphic
		player->m_pCanon = Sprite::create(PLAYER_CANON);									// The weapon on the players ship
		player->m_pCanon->setVisible(false);												// Hide the canon to begin with
		player->m_pCanon->setPosition((player->getContentSize().width / 2) +				// Set its position relative to the Player Sprite
			player->getContentSize().width * 0.4f, player->getContentSize().height / 2);
		player->addChild(player->m_pCanon);													// Attach to player

	}
	else {
		delete player;
	}

	return player;
}

/*
	Initialise the Player healthbar
*/
void Player::initHealthBar(cocos2d::Size res, cocos2d::Color4F fg) {
	cocos2d::Color4F bgColour(1, 0, 0, 0.5f);									// Background colour (Transparent red)
		
	m_pBar = HealthBar::create(
		getPosition().x + (getContentSize().width / 2),
		getPosition().y + getContentSize().height * 1.1f,						// Position
		(res.height == 720) ? 80 : 120, (res.height == 720) ? 10 : 15,			// Dimensions
		float(m_health / m_maxHealth),											// percentage  (Max 10 health)
		fg, bgColour);															// Set the bar colours
	addChild(m_pBar);
}

/*
	Get input from keyboard to update the players position
	This is platform dependent, when running unchecked on Android the virtual keyboard pops up
	expecting data to be input at the beginning of the level scene
*/
void Player::update() {
	// Move the player ship

	/****************************************************************************************
	*						KEYBOARD INPUT NOT HANDLED FOR MOBILE
	***************************************************************************************/
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX ||
		CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
		// Move player on X axis
		if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) ||
			Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
			moveLeft();
		}
		else if (Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ||
			Input::Instance()->isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
			moveRight();
		}

		// Move player on Y axis
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
	if (getPosition().y < m_visibleSize.height * 0.895f)
		setPosition(getPosition().x, getPosition().y + PLAYER_SPEED * m_scale);				// Need to cover more distance on larger screen
}
void Player::moveDown() {
	if (getPosition().y > m_visibleSize.height * 0.125f)
		setPosition(getPosition().x, getPosition().y - PLAYER_SPEED * m_scale);
}
void Player::moveLeft() {
	if (getPosition().x > getContentSize().width / 4)
		setPosition(getPosition().x - PLAYER_SPEED * m_scale, getPosition().y);
}
void Player::moveRight() {
	if (getPosition().x < m_visibleSize.width - (getContentSize().width / 2))
		setPosition(getPosition().x + PLAYER_SPEED * m_scale, getPosition().y);
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

/*
	What to do when the player is damaged
	If the player is flashing don't damage further
	Play damage effect if health > 0
	other wise play explode effect if losing a life
*/
void Player::damage() {
	bool playDamageSound = true;
	auto blink = cocos2d::Blink::create(1.0f, 9);
	blink->setTag(99);
	m_lives = Game::Instance()->getLives();

	if (getActionByTag(99) == nullptr) {																	// Only take health if not flashing
		m_health--;
		Game::Instance()->takeHealth();																		// Decrement stored health to carry over to next level
		if (m_health <= 0) {
			m_lives--;
			m_health = m_maxHealth;
			Game::Instance()->takeLife();																	// Decrement the number of lives store do carry over
			playDamageSound = false;
		}

		if (m_lives <= 0)
			setVisible(false);

		m_pBar->updateBar(m_health / m_maxHealth);

		runAction(blink);

		if (playDamageSound)
			Audio::Instance()->playFX(DAMAGE_PLAYER);														// Play the damage effect
		else
			Audio::Instance()->playFX(EXPLOSION_PLAYER);													// Play the explosion effect
	}	
}
