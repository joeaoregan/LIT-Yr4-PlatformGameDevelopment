/*
	AsteroidOriginal.cpp
	
	Joe O'Regan
	K00203642
	02/03/2018

	Second type of asteroid using original image from Space Shooter tutorial
*/

#include "AsteroidOriginal.h"
#include "Game.h"
#include "Level.h"

AsteroidOriginal* AsteroidOriginal::create(cocos2d::Size res) {
	AsteroidOriginal* asteroid = new AsteroidOriginal();
	float scale = (res.height == 720) ? 0.67f : 1.0f;
	
	if (asteroid && asteroid->initWithSpriteFrameName(ASTEROID_IMG)) {
		asteroid->autorelease();

		asteroid->winSize = res;
		asteroid->m_type = DYNAMIC_ROID;								// Set the asteroid type
		asteroid->setVisible(false);
		asteroid->setScale(scale);										// Scale down the size for PC

		asteroid->m_speed = Level::Instance()->randomValueBetween(asteroid->m_speedMin, asteroid->m_speedMax);
	}
	else {	
		delete asteroid;
	}



	return asteroid;
}
