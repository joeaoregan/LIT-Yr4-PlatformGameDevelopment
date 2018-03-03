#include "PowerUp.h"
#include "Level.h"

PowerUp* PowerUp::create(cocos2d::Size res, int type) {
	PowerUp* powerUp = new PowerUp();
	float scale = (res.height == 720) ? 0.67f : 1.0f;

	std::string image;
	if (type == NEW_LIFE)
		image = POWER_UP_LIFE_IMG;
	else if (type == WEAPON_UPGRADE)
		image = POWER_UP_WEAPON_IMG;

	if (powerUp && powerUp->initWithFile(image)) {
		powerUp->autorelease();
		
		powerUp->setVisible(false);
		powerUp->setScale(scale);																			// Scale down the size for PC

		// Choose random spawn time
		float curTimeInit = Game::Instance()->getTimeTick();												// Current game time // Time to finish game	
		float lastSpawnTime = Game::Instance()->getLevelDuration() - 5000.0f;
		powerUp->m_spawnTime = curTimeInit + Level::Instance()->randomValueBetween(200.0f, lastSpawnTime);	// -5000 (5 secs before end Don't spawn it when the player has no chance of getting it	
		
		powerUp->m_speed = Level::Instance()->randomValueBetween(powerUp->m_speedMin, powerUp->m_speedMax);
				
		powerUp->m_randYPos = Level::Instance()->randomValueBetween(0.1f, 0.8f);							// Random Y position for asteroid
		powerUp->setPosition(res.width + powerUp->getContentSize().width, res.height * powerUp->m_randYPos);// Position to spawn the power up at
	}
	else {
		delete powerUp;
	}

	return powerUp;
}

void PowerUp::rotate() {
	float angle = 360.f;

	float direction = Level::Instance()->randomValueBetween(1.0f, 10.0f);
	int rotateDirection = ((int)direction % 2 == 0) ? 1.0f : -1.0f;

	CCLOG("PowerUp Rotate Direction %d", rotateDirection);

	auto rotate = cocos2d::RotateBy::create(m_speed, angle * rotateDirection);
	auto repeat = cocos2d::RepeatForever::create(rotate);
	this->runAction(repeat);
}


void PowerUp::move(cocos2d::Size res, float duration, float y) {
	stopAllActions();
	setPosition(res.width + getContentSize().width / 2, y);
	setVisible(true);
	setScale(((res.height == 720) ? 1.0f : 1.5f) * (duration / 10.0f) * 1.25f);

	rotate();
};