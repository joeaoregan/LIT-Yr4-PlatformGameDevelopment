/*
	EnterName.h

	Joe O'Regan
	K00203642
	21/02/2018

	Change game Settings

	Previously implemented as a separate Layer sub-class
	The Settings class is now a sub-class of MenuScene

	MenuScene Sub-Class:
	Code for initialising the background image has been implemented in MenuScene
	This is inherited along with labels by menu sub-classes for the game and scene titles,
	a back button, and the exit button. The Current Player menu text object
	directs the player to the Enter Name scene.
*/

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "MenuScene.h"

class Settings : public MenuScene {
public:	
	static cocos2d::Scene* createScene();				// Returns class instance pointer
		
	virtual bool init();								// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
		
	CREATE_FUNC(Settings);								// Static create() method implemented manually

private:

	// Set label text
	std::string playerLabelText();						// Set the text on Enable/Disable music player button label
	std::string gameDifficulty();						// Set the game difficulty level

	void setLabelProperties(MenuItemLabel* l, float y);	// Set the properties for the specified label

	// Callbacks	
	void showHideMusicControls(cocos2d::Ref *sender);	// Set the music player visiblity
	void changeDifficulty(cocos2d::Ref *sender);		// Adjust the game difficulty
	void goToAudioMenu(cocos2d::Ref *sender);			// Go to the Audio settings menu

	// Menu
	cocos2d::MenuItemLabel* m_pShowMPlayerLbl;			// Show the music player
	cocos2d::MenuItemLabel* m_pSetDifficultyLbl;		// Set the game difficult
	cocos2d::MenuItemLabel* m_pGoToAudioLbl;			// Go to audio menu label

	// Labels
	cocos2d::Label* m_pInstructionLbl;					// Label to display current high score
	cocos2d::Label* m_pMusicLbl;						// Music volume setting label
	cocos2d::Label* m_pDifficultyLbl;					// Set the game difficult
	cocos2d::Label* m_pAudioMenuLbl;					// Go to audio menu label

	// Menu buttons
	cocos2d::Menu* m_menu;								// Create the menu options

};

#endif // __SETTINGS_H__