/*
	HealthBar.cpp

	Joe O'Regan
	K00203642
	24/02/2018

	Draw a status bar / health bar
*/

#include "HealthBar.h"

HealthBar* HealthBar::create(int x, int y, int w, int h, float pc, cocos2d::Color4F FGColor, cocos2d::Color4F BGColor, bool showLabel) {
	HealthBar* hbar = new HealthBar();
	hbar->autorelease();
	hbar->width = w;
	hbar->height = h;

	hbar->labelPresent = showLabel;

	if (true) {
		hbar->percent = (pc > 1.0f) ? 1.0f : (pc < 0.0f) ? 0.0f : pc;					// If greater than 1 set to 1, if minus set to 0

		hbar->rectNode = cocos2d::DrawNode::create();									// Create the nodes needed
		hbar->frontNode = cocos2d::DrawNode::create();

		cocos2d::Color4F white(1, 1, 1, 1);												// Border colour
		cocos2d::Vec2 backgroundRect[4];												// Background polygon, transparent red

		// Create the background polygon
		backgroundRect[0] = cocos2d::Vec2(-hbar->width / 2, -hbar->height / 2);
		backgroundRect[1] = cocos2d::Vec2(hbar->width / 2, -hbar->height / 2);
		backgroundRect[2] = cocos2d::Vec2(hbar->width / 2, hbar->height / 2);
		backgroundRect[3] = cocos2d::Vec2(-hbar->width / 2, hbar->height / 2);
			
		// Create the points for the foreground shape
		hbar->foregroundRect[0] = cocos2d::Vec2(-hbar->width / 2, -hbar->height / 2);
		hbar->foregroundRect[1] = cocos2d::Vec2((-hbar->width / 2) + 
			(hbar->width  * hbar->percent), -hbar->height / 2);
		hbar->foregroundRect[2] = cocos2d::Vec2((-hbar->width / 2) + 
			(hbar->width  * hbar->percent), hbar->height / 2);
		hbar->foregroundRect[3] = cocos2d::Vec2(-hbar->width / 2, hbar->height / 2);

		// Draw the polygons and set their positions
		hbar->rectNode->drawPolygon(backgroundRect, 4, BGColor, 1, white);
		hbar->frontNode->drawPolygon(hbar->foregroundRect, 4, FGColor, 1, white);
		hbar->rectNode->setPosition(cocos2d::Point(x, y));
		hbar->frontNode->setPosition(cocos2d::Point(x - 
			hbar->rectNode->getContentSize().width + hbar->frontNode->getContentSize().width, y));

		// Add the nodes
		hbar->addChild(hbar->rectNode);
		hbar->addChild(hbar->frontNode);
		
		// Create a label in the status bar
		if (showLabel) {
			hbar->percentLbl = cocos2d::Label::createWithTTF(hbar->labelTag +
				cocos2d::StringUtils::toString((int)(hbar->percent * 100)) + "%",
				"fonts/Super Mario Bros..ttf", h * 1.0f);
			hbar->percentLbl->setColor(cocos2d::Color3B::WHITE);
			hbar->percentLbl->setPosition(cocos2d::Point(x, y));
			hbar->addChild(hbar->percentLbl);
		}
	}

	return hbar;
}

void HealthBar::updateBar(float percent) {
	frontNode->setScaleX(percent);

	frontNode->setPosition(cocos2d::Point(rectNode->getPosition().x - (width/2) + (width/2 * percent), frontNode->getPosition().y));

	//if (percentLbl->isVisible())
	//if (isVisible())
	//if (percentLbl)
	if (labelPresent)
		percentLbl->setString(labelTag + cocos2d::StringUtils::toString((int)(percent * 100)) + "%");	// Null pointer error -> no labels on some, only update the ones that do
}