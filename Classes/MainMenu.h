#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:	
	static cocos2d::Scene* createScene();		// Returns class instance pointer
		
	virtual bool init();						// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
		
	CREATE_FUNC(MainMenu);						// Static create() method implemented manually

private:
	void StartGame(cocos2d::Ref *sender);		// Start the game
	void GoToHighScores(cocos2d::Ref *sender);	// Go to the high scores table
};

#endif // __MAIN_MENU_H__