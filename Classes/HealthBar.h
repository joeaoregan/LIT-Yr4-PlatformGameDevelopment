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
cocos2d::DrawNode* createStatusBar(int x, int y, int w, int h, float percent, cocos2d::Color4F FGColor, cocos2d::Color4F BGColor) {
	percent = (percent > 1.0f) ? 1.0f : (percent < 0.0f) ? 0.0f : percent;		// If greater than 1 set to 1, if minus set to 0
	
	cocos2d::DrawNode* rectNode = cocos2d::DrawNode::create();
	cocos2d::Color4F white(1, 1, 1, 1);
	cocos2d::Vec2 foregroundRect[4];
	cocos2d::Vec2 backgroundRect[4];

	backgroundRect[0] = cocos2d::Vec2(-w / 2, -h / 2);
	backgroundRect[1] = cocos2d::Vec2(w / 2, -h / 2);
	backgroundRect[2] = cocos2d::Vec2(w / 2, h / 2);
	backgroundRect[3] = cocos2d::Vec2(-w / 2, h / 2);

	foregroundRect[0] = cocos2d::Vec2(-w / 2, -h / 2);
	//foregroundRect[1] = cocos2d::Vec2(w / 2 * percent, -h / 2);
	//foregroundRect[2] = cocos2d::Vec2(w / 2 * percent, h / 2);
	foregroundRect[1] = cocos2d::Vec2((-w / 2) + (w  * percent), -h / 2);
	foregroundRect[2] = cocos2d::Vec2((-w / 2) + (w  * percent), h / 2);
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


/*
void Level1::createStatusBar(int x, int y, int w, int h, float percent, Color4F FGColor, Color4F BGColor, int orientation, int startFrom) {	// Horizontal healthbar 2017/01/20 added if statement to decide orientation of health bar

percent = (percent > 1.0f) ? 1.0f : (percent < 0.f) ? 0.f : percent;		// If greater than 1 set to 1, if minus set to 0

cocos2d::DrawNode* rectNode = DrawNode::create();
Color4F white(1, 1, 1, 1);
Vec2 foregroundRect[4];
Vec2 backgroundRect[4];

foregroundRect[0] = Vec2(-w/2, -h/2);
foregroundRect[1] = Vec2(w/2, -h/2);
foregroundRect[2] = Vec2(w/2, h/2);
foregroundRect[3] = Vec2(-w/2, h/2);

backgroundRect[0] = Vec2(-w / 2, -h / 2);
backgroundRect[1] = Vec2(w / 2 * percent, -h / 2);
backgroundRect[2] = Vec2(w / 2 * percent, h / 2);
backgroundRect[3] = Vec2(-w, h / 2);

rectNode->drawPolygon(backgroundRect, 4, BGColor, 2, white);
rectNode->drawPolygon(foregroundRect, 4, BGColor, 2, white);
rectNode->setPosition(Point(x, y));


//SDL_Rect foregroundRect;													// The rectangle to
//Color4F old;

//	SDL_GetRenderDrawColor(Game::Instance()->getRenderer(), &old.r, &old.g, &old.g, &old.a);
//	SDL_Rect backgroundRect = { x, y, w, h };									// Background rectangle with coordinates and dimensions entered
//	SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a);	// Draw a Solid Rectangle with the RGB colour values assigned
//	SDL_RenderFillRect(Game::Instance()->getRenderer(), &backgroundRect);
//	SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);

// Decide if Healthbar is horizontal or vertical
if (orientation == HORIZONTAL) {
int newW = (int)((float)w * Percent);									// new width
int newX = x + (w - newW);												// new x coord
if (startFrom == START_LEFT) foregroundRect = { newX, y, newW, h };		// Decrease from left
else if (startFrom == START_RIGHT) foregroundRect = { x, y, newW, h };	// 2017/02/17 Decrease from right
}
else if (orientation == VERTICAL) {
int ph = (int)((float)h * Percent);
int py = y + (h - ph);
foregroundRect = { x, py, w, ph };
}

//SDL_RenderFillRect(Game::Instance()->getRenderer(), &foregroundRect);
//SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), old.r, old.g, old.b, old.a);

//}
*/
