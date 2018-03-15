/*
	AsteroidOriginal.cpp
	
	Joe O'Regan
	K00203642
	02/03/2018

	Second type of asteroid using original image from Space Shooter tutorial
	The sprite is created with initWithSpriteFrameName() instead of create()
*/

#include "AsteroidOriginal.h"
#include "Game.h"
#include "Level.h"

/*
	Only difference to Asteroid base class is it uses initWithSpriteFrameName() to create the Sprite
*/
AsteroidOriginal* AsteroidOriginal::create(cocos2d::Size res) {
	AsteroidOriginal* asteroid = new AsteroidOriginal();
	//float scale = (res.height == 720) ? 0.67f : 1.0f;					// Scale objects to suit screen resolution
	float scale = (res.height == 1080) ? 1.0f :
		(res.height == 720) ? 0.67f : res.height / 1080;							// Kindle resolution is different
	
	if (asteroid && asteroid->initWithSpriteFrameName(ASTEROID_IMG)) {
		asteroid->autorelease();
		asteroid->m_winSize = res;										// Store the screen size (future use)
		asteroid->setVisible(false);									// Set invisible to begin with
		asteroid->setScale(scale);										// Scale down the size for PC
	}
	else {	
		delete asteroid;
	}
	
	return asteroid;													// Return Asteroid Original object
}
