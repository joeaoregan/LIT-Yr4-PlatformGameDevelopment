#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "cocos2d.h"
#include "Defines.h"

class Asteroid : public cocos2d::Sprite {
public:
	Asteroid() {}
	~Asteroid() {}
	
	static Asteroid* create(cocos2d::Size res);
		
	virtual void update(float dt) {}

	float minSpeed() { return m_speedMin; }
	float maxSpeed() { return m_speedMax; }

	//void move(cocos2d::Size res, float duration, float y, cocos2d::Layer* layer) {
	//void move(cocos2d::Size res, float duration, float y, cocos2d::Layer &layer) {
	void move(cocos2d::Size res, float duration, float y) {
		stopAllActions();
		setPosition(res.width + getContentSize().width / 2, y);
		setVisible(true);
		setScale(((res.height == 720) ? 1.0f : 1.5f) * (duration / 10.0f) * 1.25f);
		/*
		runAction(Sequence::create(
			MoveBy::create(duration, Point(-res.width - getContentSize().width, 0)),
			cocos2d::CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, layer)), NULL)	// DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
		);		
		*/
		rotateAsteroid();
	};

	void rotateAsteroid();

	float getSpeed() { return m_speed; }

protected:
	float m_speed = 0;
	float m_speedMin = 2.0f;
	float m_speedMax = 10.0f;

	int m_type = STATIC_ROID;
};

#endif // __ASTEROID_H__