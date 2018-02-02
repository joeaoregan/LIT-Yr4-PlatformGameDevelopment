/*
	Joe O'Regan
	ParallaxNodeExtras.h
	02/02/2018
*/
#include "cocos2d.h"

USING_NS_CC;

class ParallaxNodeExtras : public ParallaxNode {

public:		
	ParallaxNodeExtras();							// Need to provide a constructor
		
	static ParallaxNodeExtras * create();			// just to avoid ugly later cast and also for safety
		
	void incrementOffset(Point offset, Node* node);	// Facility method (it’s expected to have it soon in COCOS2DX)
};
