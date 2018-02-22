/*
	EnterName.h

	Joe O'Regan
	21/02/2018

	Enter the players name
*/

#ifndef __ENTER_NAME_H__
#define __ENTER_NAME_H__

#include "cocos2d.h"

#include "extensions/cocos-ext.h"										// UI
#include "ui/CocosGUI.h"

USING_NS_CC_EXT;
//using namespace ui;


class EnterName : public cocos2d::Layer {
public:	
	static cocos2d::Scene* createScene();								// Returns class instance pointer
		
	virtual bool init();												// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
		
	CREATE_FUNC(EnterName);												// Static create() method implemented manually

	void initBackground();												// Initialise the background image

private:
	void returnToMenu(cocos2d::Ref *sender);							// Return to the MainMenu
	Size visibleSize;													// Get the size of the screen, to set the image size for the current platform
	Vec2 origin;														// Screen origin point

	cocos2d::Sprite* backgroundSprite;									// Sprite for background image

	__String *tempScore;												// Text to display

	cocos2d::Sprite* titleSprite;
	cocos2d::MenuItemImage* btnBackImg;									// Set image for Back Button menu option
	cocos2d::Menu* btnBack;

	// High Score
	cocos2d::LabelTTF* highScoreLbl;									// Label to display current high score

	// Text Input
	cocos2d::ui::TextField* txtField;									// Text entered
	void textFieldEvent(Ref* sender, ui::TextField::EventType type);
	cocos2d::UserDefault* def;											// UserDefault used to save and load variables
};

#endif // __ENTER_NAME_H__