/*
	Joe O'Regan
	ParallaxNodeExtras.h
	02/02/2018
*/
#include "cocos2d.h"

class ParallaxNodeExtras : public cocos2d::ParallaxNode {

public:		
	ParallaxNodeExtras();												// Need to provide a constructor
		
	static ParallaxNodeExtras * create();								// just to avoid ugly later cast and also for safety
		
	void incrementOffset(cocos2d::Point offset, cocos2d::Node* node);	// Facility method (it’s expected to have it soon in COCOS2DX)

	bool init();
	void update(float dt);

private:
	cocos2d::Sprite *_spaceDust1, *_spaceDust2, *_planetSunrise;
	cocos2d::Sprite *_galaxy, *_spatialAnomaly1, *_spatialAnomaly2;
};
