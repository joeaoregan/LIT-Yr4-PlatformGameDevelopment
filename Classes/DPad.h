/*
	DPad.h

	Modified by: 
	Joe O'Regan
	K00203642

	Added fire buttons

	===================================

	RPGCollab

	Modified version of the DPad.h

	Easy Arrows by Felix Mo
	Rewritten by Jordan Brown on 9/1/14.
*/

#ifndef __DPAD_H__
#define __DPAD_H__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class DPad: public cocos2d::Node{

public:

    static DPad *create(std::string base, std::string buttonImage, std::string pressedButtonImage, cocos2d::Point position);

	DPad() {};													// Constructor
	~DPad() {};													// Destructor

	bool init(cocos2d::Layer *layer);							// JOR 13/12/2018 initialise the directional pad
	cocos2d::MenuItemImage *getButton(int button);
	cocos2d::MenuItemImage *getFireButton(int button);
    void setCorner(int corner);									// Set corner to position in

private:

	cocos2d::MenuItemImage *m_pUpItem;							// Up arrow
	cocos2d::MenuItemImage *m_pDownItem;						// Down arrow
	cocos2d::MenuItemImage *m_pLeftItem;						// Left Arrow
	cocos2d::MenuItemImage *m_pRightItem;						// Right Arrow
	cocos2d::MenuItemImage *m_pABtnItem;						// JOR A Button
	cocos2d::MenuItemImage *m_pBBtnItem;						// JOR B Button
	cocos2d::Sprite *m_pBGSprite;								// Background for DPad arrows

	cocos2d::Size m_visibleSize;								// Screen resolution
};
#endif /* defined(__DPAD_H__) */
