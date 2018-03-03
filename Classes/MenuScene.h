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

#include "Defines.h"

class MenuScene : public cocos2d::Layer
{
public:
	static MenuScene* Layer() {
		if (m_pLayer == 0) {
			m_pLayer = new MenuScene();
			return m_pLayer;
		}
		return m_pLayer;
	}

	static cocos2d::Scene* createScene();							// Returns class instance pointer
		
	virtual bool init();											// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
	virtual void update(float dt);									// Update the menu
		
	CREATE_FUNC(MenuScene);											// Static create() method implemented manually
		
	// Callbacks
	void menuCloseCallback(Ref* pSender);							// Exit the game
	void GoToEnterName(cocos2d::Ref *sender);						// Go to the enter name scene
	void returnToMenu(cocos2d::Ref *sender);						// Return to the MainMenu

	// Keyboard button highlight
	void nextButton();												// Go to next button
	void prevButton();												// Go to previous button
	
protected:

	static MenuScene* m_pLayer;										// Statpic pointer to the Layer

	// Screen size
	cocos2d::Size m_visibleSize;									// Visible screen size for the current platform
	cocos2d::Vec2 m_origin;											// Screen origin point
	float m_scale = 0.67f;											// Scale images with screen resolution

	// Player Name
	cocos2d::UserDefault* m_def;									// UserDefault used to save and load variables
	cocos2d::Label* m_changeNameLbl;								// Player name label
	std::string m_currentPlayerName;								// Players name text

	// Sprites
	cocos2d::Sprite* m_pBackgroundSprite;							// Background image
	cocos2d::Sprite* m_pGameTitleSprite;							// Game title image
	cocos2d::Sprite* m_pTitleSprite;								// Scene title image

	cocos2d::MenuItemImage* m_pBtnBackImg;							// Set image for Back Button menu option
	cocos2d::MenuItemLabel* m_pCurrentPlayerLbl;					// Player name text changes scene to enter name scene when pressed
	cocos2d::MenuItemImage* m_pCloseItem;							// Close button, bottom right corner

	void setYPosAndScale(cocos2d::Sprite* sprite, float y);			// Set objects position and scale
	void setYPosAndScale(cocos2d::MenuItemImage* sprite, float y);	// Set position and scale for multiple objects

	unsigned int m_totalButtons;									// Total number of buttons in the scene
	unsigned int m_currentBtn = 1;									// Current selected button

	// Keyboard
	float m_nextBtnTime;											// Time to next button press (keyboard)
	float m_buttonRate = 300.0f;									// The time between button presses		

	// Menu Buttons
	cocos2d::Menu* menu;											// Button 1 start the game
};

#endif // __MENU_H__