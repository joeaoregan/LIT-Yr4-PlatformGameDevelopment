/*
	PowerUp.h

	Joe O'Regan
	K00203642

	Create a power up, specifying a type to decide which image to use
	And what to do upon collision
*/

#include "PowerUp.h"
#include "Level.h"		// Use random value functions
#include "Audio.h"		// Play the pickup sound when the power up is collected
#include "Game.h"		// Update the score when the power up is collected

/*
	Create the power up depending on the type parameter specified
	enum with types in Defines.h
*/
PowerUp* PowerUp::create(cocos2d::Size res, int type) {
	PowerUp* powerUp = new PowerUp();
	float scale = (res.height == 720) ? 0.67f : 1.0f;

	std::string image;
	if (type == NEW_LIFE)
		//image = POWER_UP_LIFE_IMG;
		image = POWER_UP_LIFE_IMG2;
	else if (type == WEAPON_UPGRADE)
		//image = POWER_UP_WEAPON_IMG;
		image = POWER_UP_WEAPON_IMG2;
	
	//if (powerUp && powerUp->initWithFile(image)) {
	if (powerUp && powerUp->initWithSpriteFrameName(image)) {												// Create from sprite sheet
		powerUp->autorelease();																				// Clear up automatically when done
		
		powerUp->setVisible(false);																			// Hidden initially
		powerUp->setScale(scale);																			// Scale down the size for PC

		// Choose random spawn time
		float curTimeInit = Game::Instance()->getTimeTick();												// Current game time // Time to finish game	
		float lastSpawnTime = Game::Instance()->getLevelDuration() - 5000.0f;								// Give 5 seconds before the level ends so the player can collect it

		powerUp->m_spawnTime = curTimeInit + Level::Instance()->randomValueBetween(200.0f, lastSpawnTime);	// -5000 (5 secs before end Don't spawn it when the player has no chance of getting it	
		
		powerUp->m_duration = Level::Instance()->randomValueBetween(powerUp->m_speedMin, powerUp->m_speedMax);	// Duration on screen
				
		powerUp->m_randYPos = Level::Instance()->randomValueBetween(0.1f, 0.8f);							// Random Y position for asteroid
		powerUp->setPosition(res.width + powerUp->getContentSize().width, res.height * powerUp->m_randYPos);// Position to spawn the power up at
	}
	else {
		delete powerUp;
	}

	return powerUp;
}

/*
	Apply action to rotate the power up, and loop forever
	Random clockwise/counter clockwise is selected
*/
void PowerUp::rotate() {
	float degrees = 360.f;																					// The degrees to rotate by

	float direction = Level::Instance()->randomValueBetween(1.0f, 10.0f);									// Choose random rotate direction 
	int rotateDirection = ((int)direction % 2 == 0) ? 1.0f : -1.0f;											// Roatate clockwise even, counterclockwise for odd random number value

	CCLOG("PowerUp Rotate Direction %d", rotateDirection);

	auto rotate = cocos2d::RotateBy::create(m_duration, degrees * rotateDirection);							// Rotate over the specified amount of time
	auto repeat = cocos2d::RepeatForever::create(rotate);													// Loop the action
	this->runAction(repeat);																				// Assign the action
}

/*
	Clear / Reset the power up before moving again
*/
void PowerUp::initialise(cocos2d::Size res, float duration, float y) {
	stopAllActions();																						// Stop previous actions (not needed not reused like lasers and asteroids)
	setPosition(res.width + getContentSize().width / 2, y);													// Set position off screen at random y value
	setVisible(true);																						// Make visible
	setScale(((res.height == 720) ? 1.0f : 1.5f) * (duration / 10.0f) * 1.25f);								// Set scale

	rotate();																								// Set the rotate action
}

void PowerUp::collected() {
	setVisible(false);																						// Hide the power up
	Audio::Instance()->playFX(POWER_UP);																	// Play the power up sound effect
	Game::Instance()->updateScore(50);																		// Award 50 points for collecting a power up
}