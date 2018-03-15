/*
	Asteroid.h

	Joe O'Regan
	K00203642
	02/03/2018

	Create asteroid at random position, scaled to suit screen resolution
	Pic a movement speed based on a random range

*/

#include "Asteroid.h"
#include "Level.h"
#include "Game.h"
#include "Audio.h"

/*
	Create the asteroid, using the screen dimensions for positioning and scale
*/
Asteroid* Asteroid::create(cocos2d::Size res) {
	Asteroid* asteroid = new Asteroid();
	//float scale = (res.height == 720) ? 0.67f : 1.0f;								// Choose scale based on the screen resolution
	float scale = (res.height == 1080) ? 1.0f :
		(res.height == 720) ? 0.67f : res.height / 1080;							// Kindle resolution is different

	//if (asteroid && asteroid->initWithFile("Asteroid.png")) {						// Create asteroid from .png in resources
	if (asteroid && asteroid->initWithSpriteFrameName(ASTEROID2_IMG)) {				// Create asteroid from sprite sheet
		asteroid->autorelease();
		
		asteroid->m_winSize = res;													// Store the screen resolution
		asteroid->setVisible(false);												// Initially invisible until Game::spawnObjects() is called
		asteroid->setScale(scale);													// Scale down the size for PC

		asteroid->setPosition(res.width + asteroid->getContentSize().width, 
			res.height * asteroid->m_randYPos);										// Position to spawn the power up at
	}
	else {
		delete asteroid;
	}

	return asteroid;																// Return an object of type Asteroid
}

/*
	Initialise the asteroid
	Stop all actions, set the position, set visible, and scale accoring to speed
*/
void Asteroid::init(cocos2d::Size res) {
	m_duration = Level::Instance()->randomValueBetween(m_speedMin, m_speedMax);		// Set a random speed
	m_randYPos = Level::Instance()->randomValueBetween(0.0f, m_winSize.height);		// Random Y position for asteroid

	stopAllActions();																// Stop any previous actions (Asteroids are reused)
	setPosition(res.width + getContentSize().width / 2, m_randYPos);				// Set the postion off screen to the right, at random Y value
	setVisible(true);																// Set visible
	setScale(((res.height == 720) ? 1.0f : 1.5f) * (m_duration / 10.0f) * 1.25f);	// Scale the asteroid based on its speed

	rotateAsteroid();																// Start rotating
}

/*
	Rotate the asteroid with a random direction
*/
void Asteroid::rotateAsteroid() {
	float angle = 360.f;															// Angle to rotate to (360 = constanlty rotating)

	float direction = Level::Instance()->randomValueBetween(1.0f, 10.0f);			// Random number decides if rotating forwards or backwards

	int rotateDirection = ((int)direction % 2 == 0) ? 1.0f : -1.0f;					// multiply by -1 to revese rotation if odd number randomly generated
	//CCLOG("Asteroid Rotate Direction %d", rotateDirection);
	auto rotate = cocos2d::RotateBy::create(m_duration, angle * rotateDirection);	// Rotate the asteroid 360 degrees forwards/backwards
	auto repeat = cocos2d::RepeatForever::create(rotate);							// Set the action to loop
	this->runAction(repeat);														// Then run it
}

/*
	Destroy asteroid, update score, play sound effect, increment count of asteroids destroyed
*/
void Asteroid::damage() {
	if (isVisible()) {
		Game::Instance()->incrementAsteroidKills();									// Increment the number of asteroids destroyed
		Audio::Instance()->playFX(EXPLOSION_LARGE);									// Play the explosion effect
		Game::Instance()->updateScore(10);											// Award 10 points for destroying an asteroid
	}

	setVisible(false);																// Hide the asteroid
}