#ifndef __POWER_UP_H__
#define __POWER_UP_H__

#include "cocos2d.h"
#include "Defines.h"

class PowerUp : public cocos2d::Sprite {
public:
	PowerUp() {}
	~PowerUp() {}

	static PowerUp* create(cocos2d::Size res, int type);

	virtual void update(float dt) {}

	float minSpeed() { return m_speedMin; }
	float maxSpeed() { return m_speedMax; }

	void move(cocos2d::Size res, float duration, float y) {
		stopAllActions();
		setPosition(res.width + getContentSize().width / 2, y);
		setVisible(true);
		setScale(((res.height == 720) ? 1.0f : 1.5f) * (duration / 10.0f) * 1.25f);

		rotate();
	};

	void rotate();

	float getSpeed() { return m_speed; }
	bool isSpawned() { return m_spawned; }
	void setSpawned() { m_spawned = true; }
	float getRandY() { return m_powerUpY; }
	float getSpawnTime() { return m_spawnTime; }

protected:
	float m_speed = 0;
	float m_speedMin = 2.0f;
	float m_speedMax = 10.0f;

	bool m_spawned = false; 
	float m_powerUpY;
	float m_spawnTime;

	//int m_type = STATIC_ROID;
};

#endif // __POWER_UP_H__