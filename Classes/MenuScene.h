/*
	Menu.h

	Joe O'Regan
	K00203642
	23/02/2018

	Menu base class
	
	Code for initialising the background image has been implemented in MenuScene
	This is inherited along with labels by menu sub-classes for the game and scene titles,
	a back button, and the exit button. The Current Player menu text object
	directs the player to the Enter Name scene.
*/

#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"

#define TRANSITION_TIME 0.5

class MenuScene : public cocos2d::Layer
{
public:
	static MenuScene* Layer() {
		if (layer == 0) {
			layer = new MenuScene();
			return layer;
		}
		return layer;
	}

	static cocos2d::Scene* createScene();		// Returns class instance pointer
		
	virtual bool init();						// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
	virtual void update(float dt);				// Update the menu
		
	CREATE_FUNC(MenuScene);						// Static create() method implemented manually
	
	// Screen size
	cocos2d::Size visibleSize;					// Visible screen size for the current platform
	cocos2d::Vec2 origin;						// Screen origin point
	float scale = 0.67f;						// Scale images with screen resolution

	// Menu Buttons
	cocos2d::Menu* menu;						// Button 1 start the game

	// Callbacks
	void menuCloseCallback(Ref* pSender);		// Exit the game
	void GoToEnterName(cocos2d::Ref *sender);	// Go to the enter name scene
	void returnToMenu(cocos2d::Ref *sender);	// Return to the MainMenu

	// Sprites
	cocos2d::Sprite* backgroundSprite;			// Background image
	cocos2d::Sprite* gameTitleSprite;			// Game title image
	cocos2d::Sprite* titleSprite;				// Scene title image

	static MenuScene* layer;					// Layer

	// Player Name
	cocos2d::UserDefault* def;					// UserDefault used to save and load variables
	cocos2d::Label* changeNameLbl; 
	std::string currentPlayerName;
	// Menu Buttons
	cocos2d::MenuItemImage* btnBackImg;			// Set image for Back Button menu option
	cocos2d::MenuItemLabel* currentPlayerLbl;	// Player name text changes scene to enter name scene when pressed
	cocos2d::MenuItemImage* closeItem;			// Close button, bottom right corner

	//cocos2d::DrawNode* createStatusBar(int x, int y, int w, int h, float percent, cocos2d::Color4F FG, cocos2d::Color4F BG);

	// Keyboard button highlight
	void nextButton();
	void prevButton();
	
protected:
	void setYPosAndScale(cocos2d::Sprite* sprite, float y);
	void setYPosAndScale(cocos2d::MenuItemImage* sprite, float y);

	unsigned int m_totalButtons;
	unsigned int m_currentBtn = 1;

	float nextBtnTime;
	float buttonRate = 300.0f;
};

#endif // __MENU_H__