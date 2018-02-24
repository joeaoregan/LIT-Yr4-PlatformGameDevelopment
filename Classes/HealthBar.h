/*
	HealthBar.h

	Joe O'Regan
	K00203642
	24/02/2018

	Draw a healthbar
*/

#include "cocos2d.h"

#ifndef __HEALTH_BAR__
#define __AUDIO__

// Horizontal health bar
cocos2d::DrawNode* MenuScene::createStatusBar(int x, int y, int w, int h, float percent, cocos2d::Color4F FGColor, cocos2d::Color4F BGColor) {
	percent = (percent > 1.0f) ? 1.0f : (percent < 0.f) ? 0.f : percent;		// If greater than 1 set to 1, if minus set to 0

	cocos2d::DrawNode* rectNode = cocos2d::DrawNode::create();
	cocos2d::Color4F white(1, 1, 1, 1);
	cocos2d::Vec2 foregroundRect[4];
	cocos2d::Vec2 backgroundRect[4];

	backgroundRect[0] = cocos2d::Vec2(-w / 2, -h / 2);
	backgroundRect[1] = cocos2d::Vec2(w / 2, -h / 2);
	backgroundRect[2] = cocos2d::Vec2(w / 2, h / 2);
	backgroundRect[3] = cocos2d::Vec2(-w / 2, h / 2);

	foregroundRect[0] = cocos2d::Vec2(-w / 2, -h / 2);
	foregroundRect[1] = cocos2d::Vec2(w / 2 * percent, -h / 2);
	foregroundRect[2] = cocos2d::Vec2(w / 2 * percent, h / 2);
	foregroundRect[3] = cocos2d::Vec2(-w / 2, h / 2);


	rectNode->drawPolygon(backgroundRect, 4, BGColor, 1, white);
	rectNode->drawPolygon(foregroundRect, 4, FGColor, 1, white);
	rectNode->setPosition(cocos2d::Point(x, y));

	return rectNode;
}


/*
class HealthBar : public cocos2d::Node {
public:

	void init();						// Play music and preload effects

private:
	HealthBar() {};							// Constructor is private for use as a Singleton. Initializes the variables
	~HealthBar() {};						// Private destructor shuts down and cleans up the mixer API
};
*/
#endif // __HEALTH_BAR__
