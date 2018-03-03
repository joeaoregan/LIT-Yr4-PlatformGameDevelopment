/*
	Asteroid.h

	Joe O'Regan
	K00203642
	02/03/2018

	Create asteroid at random position, scaled to suit screen resolution
	Pic a movement speed based on a random range
*/

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
	void init(cocos2d::Size res);

	void rotateAsteroid();								// Rotate the asteroid

	float minSpeed() const { return m_speedMin; }		// Get min duration
	float maxSpeed() const { return m_speedMax; }		// Get max duration

	float getRandY() const { return m_randYPos; }		// Get random Y axis spawn point

	float getDuration() const { return m_duration; }	// Random duration on screen, used in spawnObject() for applying action sequence

protected:
	cocos2d::Size m_winSize;							// Size of the window
	float m_duration = 0;								// Speed of the asteroid
	float m_speedMin = 2.0f;							// Set the min duration to travel across screen (Speed = faster)
	float m_speedMax = 10.0f;							// Set the max duration to travel across screen (Speed = slower)
	float m_randYPos;									// Spawn at random position on Y access

	//int m_type = STATIC_ROID;							// Type of asteroid (
};

#endif // __ASTEROID_H__