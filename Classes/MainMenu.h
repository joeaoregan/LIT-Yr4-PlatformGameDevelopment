/*
	MainMenu.h

	Joe O'Regan
	K00203642

	Game main menu, from here the player can start the game, view the highscores, or exit the game
*/

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
	void GoToScores(cocos2d::Ref *sender);		// Go to the high scores table
	void menuCloseCallback(Ref* pSender);		// Exit the game

	// Screen size
	cocos2d::Size visibleSize;					// Visible screen size for the current platform
	cocos2d::Vec2 origin;						// Screen origin point

	// Sprites
	cocos2d::Sprite* backgroundSprite;			// Background image
	cocos2d::Sprite* titleSprite;				// Scene title image

	// Menu Images
	cocos2d::MenuItemImage* playItem;			// Start the game
	cocos2d::MenuItemImage*	scoreItem;			// Go to the high scores scene
	cocos2d::MenuItemImage* closeItem;			// Exit the game

	// Menu Buttons
	cocos2d::Menu* menu1;						// Button 1 start the game
	cocos2d::Menu* menu2;						// Button 2 go to high scores
	cocos2d::Menu* menu3;						// Button 3 exit the game

	// High Score
	cocos2d::UserDefault* def;					// UserDefault used to save and load variables
	int highScore;								// Load the first sorted score, which is the high score
	cocos2d::__String *tempScore;				// Score text to display
	cocos2d::LabelTTF* highScoreLbl;			// Label to display the score text
};

#endif // __MAIN_MENU_H__