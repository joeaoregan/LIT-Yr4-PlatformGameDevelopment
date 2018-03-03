/*
	EnterName.h

	Joe O'Regan
	21/02/2018

	Enter the players name
*/

#ifndef __ENTER_NAME_H__
#define __ENTER_NAME_H__

#include "MenuScene.h"

#include "extensions/cocos-ext.h"										// UI
#include "ui/CocosGUI.h"

USING_NS_CC_EXT;														// Required for entering text

class EnterName : public MenuScene {
public:	
	static cocos2d::Scene* createScene();								// Returns class instance pointer
		
	virtual bool init();												// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
		
	CREATE_FUNC(EnterName);												// Static create() method implemented manually
	
private:

	cocos2d::__String *m_pTempScore;										// Text to display

	// High Score
	cocos2d::Label* m_pInstructionLbl;										// Label to display current high score

	// Text Input
	cocos2d::ui::TextField* m_pTxtField;									// Text entered
	void textFieldEvent(Ref* sender, cocos2d::ui::TextField::EventType type);
	cocos2d::UserDefault* m_pDef;											// UserDefault used to save and load variables
};

#endif // __ENTER_NAME_H__