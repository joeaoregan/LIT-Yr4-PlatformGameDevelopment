#include "Asteroid.h"
//#include "Game.h"
#include "Level.h"

Asteroid* Asteroid::create(cocos2d::Size res) {
	Asteroid* asteroid = new Asteroid();
	float scale = (res.height == 720) ? 0.67f : 1.0f;

	if (asteroid && asteroid->initWithFile("Asteroid.png")) {
		asteroid->autorelease();
		
		asteroid->setVisible(false);
		asteroid->setScale(scale);									// Scale down the size for PC

		asteroid->m_speed = Level::Instance()->randomValueBetween(asteroid->m_speedMin, asteroid->m_speedMax);
	}
	else {
		delete asteroid;
	}

	return asteroid;
}


void Asteroid::rotateAsteroid() {
	float angle = 360.f;

	float direction = Level::Instance()->randomValueBetween(1.0f, 10.0f);

	//float duration = Level::Instance()->randomValueBetween(
	int rotateDirection = ((int)direction % 2 == 0) ? 1.0f : -1.0f;
	CCLOG("Asteroid Rotate Direction %d", rotateDirection);
	auto rotate = cocos2d::RotateBy::create(m_speed, angle * rotateDirection);
	auto repeat = cocos2d::RepeatForever::create(rotate);
	this->runAction(repeat);
}

//void Asteroid::update(float curTimeMillis) {}