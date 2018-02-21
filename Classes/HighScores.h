/*
	HighScores.h

	Joe O'Regan
	20/02/2018

	Display the stored high scores
*/

#ifndef __HIGH_SCORES_H__
#define __HIGH_SCORES_H__

#include "cocos2d.h"

class HighScores : public cocos2d::Layer {
public:	
	static cocos2d::Scene* createScene();		// Returns class instance pointer
		
	virtual bool init();						// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
		
	CREATE_FUNC(HighScores);					// Static create() method implemented manually

	void initBackground();						// Initialise the background image

	void sortScores();							// Sort the high scores

private:
	void returnToMenu(cocos2d::Ref *sender);	// Return to the MainMenu
	Size visibleSize;							// Get the size of the screen, to set the image size for the current platform

	cocos2d::Sprite* backgroundSprite;			// Sprite for background image
	Vec2 origin;								// Screen origin point

	__String *tempScore;						// Text to display

	cocos2d::Sprite* titleSprite;
	cocos2d::MenuItemImage* btnBackImg;			// Set image for Back Button menu option
	cocos2d::Menu* btnBack;
};

#endif // __HIGH_SCORES_H__