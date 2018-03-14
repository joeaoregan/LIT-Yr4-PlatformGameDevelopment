/*
	PowerUp.h

	Joe O'Regan
	K00203642

	Create a power up, specifying a type to decide which image to use
	And what to do upon collision
*/

#ifndef __POWER_UP_H__
#define __POWER_UP_H__

#include "cocos2d.h"
#include "Defines.h"

class PowerUp : public cocos2d::Sprite {
public:
	PowerUp() {}
	~PowerUp() {}

	static PowerUp* create(cocos2d::Size res, int type);		// Create a power up by specifying the type

	virtual void update(float dt) {}							// Update the power up	

	void initialise(cocos2d::Size res, float duration, float y);// Reset the power up before moving
		
	void rotate();												// Rotate the power up
	void collected();											// Things to do when the Power Up is picked up, update score, and play sound

	float getSpeed() const { return m_duration; }				// Get the duration
	bool isSpawned() const { return m_spawned; }				// Is the power up spawned already (only spawn once)
	void setSpawned() { m_spawned = true; }						// Set spawned
	float getRandY() const { return m_randYPos; }				// Get a random value on the Y axis of the screen for spawning
	float getSpawnTime() const { return m_spawnTime; }			// Time to spawn the power up

protected:
	float m_duration = 0;										// Duration to remain on screen (speed0
	float m_speedMin = 2.0f;									// Minimum value for generating random duration
	float m_speedMax = 10.0f;									// Max value

	bool m_spawned = false;										// Is the power up spawned
	float m_randYPos;											// Random position to spawn on Y axis
	float m_spawnTime;											// Spawn time
};

#endif // __POWER_UP_H__