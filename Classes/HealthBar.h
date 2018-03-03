/*
	HealthBar.h

	Joe O'Regan
	K00203642
	24/02/2018

	Draw a healthbar
*/

#include "cocos2d.h"
#include <string>

#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__

class HealthBar : public cocos2d::DrawNode {
public:
	static HealthBar* create(int x, int y, int w, int h, float pc, cocos2d::Color4F FGColor, cocos2d::Color4F BGColor, bool showLabel = false);

	void updateBar(float percent);							// Update the health bar

	void setLabelTag(std::string set) { m_labelTag = set; }	// Update the bar label text

private:
	float m_percent;										// Health bar percentage

	float m_width, m_height;								// Health bar dimensions

	cocos2d::Vec2 m_foregroundRect[4];						// Foreground rectangle scaled to show percentage

	cocos2d::DrawNode* m_pRectNode;							// Background rectangle
	cocos2d::DrawNode* m_pFrontNode;						// Foreground rectangle

	// Label
	cocos2d::Label* m_pPercentLbl;							// Show the percentage in text
	bool m_labelPresent;									// Is a label being used on the health bar
	std::string m_labelTag = "Percent: ";					// String used to set the label
};


#endif // __HEALTH_BAR__


/*
class HealthBar : public cocos2d::DrawNode {
public:
	static HealthBar* create(int x, int y, int w, int h, float pc, cocos2d::Color4F FGColor, cocos2d::Color4F BGColor, bool showLabel = false) {
	// Horizontal health bar
	//cocos2d::DrawNode* createStatusBar(int x, int y, int w, int h, float percent, cocos2d::Color4F FGColor, cocos2d::Color4F BGColor) {
		HealthBar* hbar = new HealthBar();
		hbar->autorelease();
		hbar->width = w;
		hbar->height = h;
		
		if (true) {
			hbar->percent = (pc > 1.0f) ? 1.0f : (pc < 0.0f) ? 0.0f : pc;		// If greater than 1 set to 1, if minus set to 0

			//cocos2d::DrawNode* 
			hbar->rectNode = cocos2d::DrawNode::create();
			hbar->frontNode = cocos2d::DrawNode::create();

			cocos2d::Color4F white(1, 1, 1, 1);
			cocos2d::Vec2 backgroundRect[4];

			backgroundRect[0] = cocos2d::Vec2(-hbar->width / 2, -hbar->height / 2);
			backgroundRect[1] = cocos2d::Vec2(hbar->width / 2, -hbar->height / 2);
			backgroundRect[2] = cocos2d::Vec2(hbar->width / 2, hbar->height / 2);
			backgroundRect[3] = cocos2d::Vec2(-hbar->width / 2, hbar->height / 2);
			
			//hbar->schedule([=](float dt) {
				hbar->foregroundRect[0] = cocos2d::Vec2(-hbar->width / 2, -hbar->height / 2);
				//foregroundRect[1] = cocos2d::Vec2(w / 2 * percent, -h / 2);
				//foregroundRect[2] = cocos2d::Vec2(w / 2 * percent, h / 2);
				hbar->foregroundRect[1] = cocos2d::Vec2((-hbar->width / 2) + (hbar->width  * hbar->percent), -hbar->height / 2);
				hbar->foregroundRect[2] = cocos2d::Vec2((-hbar->width / 2) + (hbar->width  * hbar->percent), hbar->height / 2);
				hbar->foregroundRect[3] = cocos2d::Vec2(-hbar->width / 2, hbar->height / 2);
			//}, 1.0f, "callback");

			hbar->rectNode->drawPolygon(backgroundRect, 4, BGColor, 1, white);
			hbar->frontNode->drawPolygon(hbar->foregroundRect, 4, FGColor, 1, white);
			hbar->rectNode->setPosition(cocos2d::Point(x, y));
			hbar->frontNode->setPosition(cocos2d::Point(x - hbar->rectNode->getContentSize().width + hbar->frontNode->getContentSize().width, y));
			//hbar->frontNode->setAnchorPoint(cocos2d::Point(hbar->frontNode->getContentSize().width / 2, hbar->frontNode->getContentSize().height / 2));
			hbar->addChild(hbar->rectNode);
			hbar->addChild(hbar->frontNode);
			if (showLabel) {
				cocos2d::Label* percentLbl = cocos2d::Label::createWithTTF("Percent: " + cocos2d::StringUtils::toString((int)(hbar->percent * 100)) + "%",
					"fonts/Super Mario Bros..ttf", h * 1.0f);
				percentLbl->setColor(cocos2d::Color3B::WHITE);
				percentLbl->setPosition(cocos2d::Point(x, y));
				hbar->addChild(percentLbl);
			}
		}
		//return rectNode;
		return hbar;
	}

	//HealthBar* getDrawNode() { return hbar;  };

	//void setPercent(float set) { percent = set; }

	void updateBar(float percent) {
		//foregroundRect[1] = cocos2d::Vec2((width / 2) + (width  * percent), -width / 2);
		//foregroundRect[2] = cocos2d::Vec2((width / 2) + (width  * percent), width / 2);
		//foregroundRect->scale
		frontNode->setScaleX(percent);
		//frontNode->setPosition(cocos2d::Point(rectNode->getPosition().x -
			//(rectNode->getContentSize().width / 2) - 
		//	(frontNode->getContentSize().width / 2), rectNode->getPosition().y));

		//frontNode->setPosition(cocos2d::Point(rectNode->getPosition().x - (rectNode->getContentSize().width - frontNode->getContentSize().width), rectNode->getPosition().y));
		//float offset = frontNode->getContentSize().width - (frontNode->getContentSize().width * percent);

		//frontNode->setPosition(cocos2d::Point((rectNode->getPosition().x - (rectNode->getContentSize().width / 2.0f) + (frontNode->getContentSize().width * percent)), frontNode->getPosition().y));
		frontNode->setPosition(cocos2d::Point(rectNode->getPosition().x - (width/2) + (width/2 * percent), frontNode->getPosition().y));
	}

private:
	float percent;

	float width, height;

	cocos2d::Vec2 foregroundRect[4];

	cocos2d::DrawNode* rectNode;
	cocos2d::DrawNode* frontNode;

	//static HealthBar* hbar;
};

HealthBar.h

Joe O'Regan
K00203642
24/02/2018

Draw a healthbar


#include "cocos2d.h"

#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__

class HealthBar : public cocos2d::DrawNode {
public:

	static HealthBar* HealthBar::create(int x, int y, int w, int h, float pc, cocos2d::Color4F FGColor, cocos2d::Color4F BGColor) {
		// Horizontal health bar
		//cocos2d::DrawNode* createStatusBar(int x, int y, int w, int h, float percent, cocos2d::Color4F FGColor, cocos2d::Color4F BGColor) {
		HealthBar* hbar = new HealthBar();
		hbar->autorelease();
		hbar->width = w;
		hbar->height = h;

		if (true) {
			hbar->percent = (pc > 1.0f) ? 1.0f : (pc < 0.0f) ? 0.0f : pc;		// If greater than 1 set to 1, if minus set to 0

			cocos2d::DrawNode* rectNode = cocos2d::DrawNode::create();
			cocos2d::Color4F white(1, 1, 1, 1);
			//cocos2d::Vec2 foregroundRect[4];
			cocos2d::Vec2 backgroundRect[4];

			backgroundRect[0] = cocos2d::Vec2(-hbar->width / 2, -hbar->height / 2);
			backgroundRect[1] = cocos2d::Vec2(hbar->width / 2, -hbar->height / 2);
			backgroundRect[2] = cocos2d::Vec2(hbar->width / 2, hbar->height / 2);
			backgroundRect[3] = cocos2d::Vec2(-hbar->width / 2, hbar->height / 2);

			//hbar->schedule([=](float dt) {
			hbar->foregroundRect[0] = cocos2d::Vec2(-hbar->width / 2, -hbar->height / 2);
			//foregroundRect[1] = cocos2d::Vec2(w / 2 * percent, -h / 2);
			//foregroundRect[2] = cocos2d::Vec2(w / 2 * percent, h / 2);
			hbar->foregroundRect[1] = cocos2d::Vec2((-hbar->width / 2) + (hbar->width  * hbar->percent), -hbar->height / 2);
			hbar->foregroundRect[2] = cocos2d::Vec2((-hbar->width / 2) + (hbar->width  * hbar->percent), hbar->height / 2);
			hbar->foregroundRect[3] = cocos2d::Vec2(-hbar->width / 2, hbar->height / 2);
			//}, 1.0f, "callback");

			rectNode->drawPolygon(backgroundRect, 4, BGColor, 1, white);
			rectNode->drawPolygon(hbar->foregroundRect, 4, FGColor, 1, white);
			rectNode->setPosition(cocos2d::Point(x, y));
			hbar->addChild(rectNode);
		}
		//return rectNode;
		return hbar;
	}

	//HealthBar* getDrawNode() { return hbar;  };

	void setPercent(float set) { percent = set; }

	void updateBar(float percent) {
		//foregroundRect[1]->u
		//foregroundRect[2]->clear();
		foregroundRect->setZero();
		foregroundRect[0] = cocos2d::Vec2(-width / 2, -height / 2);
		foregroundRect[1] = cocos2d::Vec2((width / 2) + (width  * percent), -height / 2);
		foregroundRect[2] = cocos2d::Vec2((width / 2) + (width  * percent), height / 2);
		foregroundRect[3] = cocos2d::Vec2(-width / 2, height / 2);
		drawPolygon(foregroundRect, 4, cocos2d::Color4F(1, 0, 0, 1), 1, cocos2d::Color4F(1, 1, 1, 1));
		//this->addChild(foregroundRect);
	}

private:
	float percent;

	float width, height;

	cocos2d::Vec2 foregroundRect[4];

	//static HealthBar* hbar;
};


#endif // __HEALTH_BAR__





class HealthBar : public cocos2d::Node {
public:

	void init();						// Play music and preload effects

private:
	HealthBar() {};							// Constructor is private for use as a Singleton. Initializes the variables
	~HealthBar() {};						// Private destructor shuts down and cleans up the mixer API
};
*/
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
