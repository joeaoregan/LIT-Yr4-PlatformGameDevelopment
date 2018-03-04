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
	hbar->m_width = w;																		// Store width
	hbar->m_height = h;																		// Store height

	hbar->m_labelPresent = showLabel;														// Specify to use the text label or not

	if (true) {
		hbar->m_percent = (pc > 1.0f) ? 1.0f : (pc < 0.0f) ? 0.0f : pc;						// If greater than 1 set to 1, if minus set to 0

		hbar->m_pRectNode = cocos2d::DrawNode::create();									// Create the nodes needed
		hbar->m_pFrontNode = cocos2d::DrawNode::create();

		cocos2d::Color4F white(1, 1, 1, 1);													// Border colour
		cocos2d::Vec2 backgroundRect[4];													// Background polygon, transparent red

		// Create the background polygon
		backgroundRect[0] = cocos2d::Vec2(-hbar->m_width / 2, -hbar->m_height / 2);			// Set points for background rectangle
		backgroundRect[1] = cocos2d::Vec2(hbar->m_width / 2, -hbar->m_height / 2);
		backgroundRect[2] = cocos2d::Vec2(hbar->m_width / 2, hbar->m_height / 2);
		backgroundRect[3] = cocos2d::Vec2(-hbar->m_width / 2, hbar->m_height / 2);
			
		// Create the points for the foreground shape
		hbar->m_foregroundRect[0] = cocos2d::Vec2(-hbar->m_width / 2, -hbar->m_height / 2);	// Set the foreground rectangle points
		hbar->m_foregroundRect[1] = cocos2d::Vec2((-hbar->m_width / 2) + 
			(hbar->m_width  * hbar->m_percent), -hbar->m_height / 2);
		hbar->m_foregroundRect[2] = cocos2d::Vec2((-hbar->m_width / 2) + 
			(hbar->m_width  * hbar->m_percent), hbar->m_height / 2);
		hbar->m_foregroundRect[3] = cocos2d::Vec2(-hbar->m_width / 2, hbar->m_height / 2);

		// Draw the polygons and set their positions
		hbar->m_pRectNode->drawPolygon(backgroundRect, 4, BGColor, 1, white);				// Draw the background rect with white border
		hbar->m_pFrontNode->drawPolygon(hbar->m_foregroundRect, 4, FGColor, 1, white);		// Foreground rect with white border
		hbar->m_pRectNode->setPosition(cocos2d::Point(x, y));								// Set positions
		hbar->m_pFrontNode->setPosition(cocos2d::Point(x - 
			hbar->m_pRectNode->getContentSize().width + hbar->m_pFrontNode->getContentSize().width, y));

		// Add the nodes
		hbar->addChild(hbar->m_pRectNode);
		hbar->addChild(hbar->m_pFrontNode);
		
		// Create a label in the status bar
		if (showLabel) {
			hbar->m_pPercentLbl = cocos2d::Label::createWithTTF(hbar->m_labelTag +			// Create percentage text label
				cocos2d::StringUtils::toString((int)(hbar->m_percent * 100)) + "%",
				"fonts/Super Mario Bros..ttf", h * 1.0f);
			hbar->m_pPercentLbl->setColor(cocos2d::Color3B::WHITE);
			hbar->m_pPercentLbl->setPosition(cocos2d::Point(x, y));
			hbar->addChild(hbar->m_pPercentLbl);
		}
	}

	return hbar;
}

/*
	Update the health bar
*/
void HealthBar::updateBar(float percent) {
	m_pFrontNode->setScaleX(percent);

	m_pFrontNode->setPosition(cocos2d::Point(m_pRectNode->getPosition().x - (m_width/2) + (m_width/2 * percent), m_pFrontNode->getPosition().y));

	if (m_labelPresent)
		m_pPercentLbl->setString(m_labelTag + cocos2d::StringUtils::toString((int)(percent * 100)) + "%");	// Null pointer error -> no labels on some, only update the ones that do (forgot it was optional)
}