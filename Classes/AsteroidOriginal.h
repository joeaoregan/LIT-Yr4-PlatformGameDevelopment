/*
	Asteroid Original.h
	
	Joe O'Regan
	K00203642
	02/03/2018

	Second type of asteroid using original image from Space Shooter tutorial
	The sprite is created with initWithSpriteFrameName() instead of create()
*/

#ifndef __ASTEROID_ORIGINAL_H__
#define __ASTEROID_ORIGINAL_H__

#include "Asteroid.h"

class AsteroidOriginal : public Asteroid {
public:

	static AsteroidOriginal* create(cocos2d::Size res);
		
	virtual void update(float dt) {};					// Update not necessary
};

#endif // __ASTEROID_ORIGINAL_H__