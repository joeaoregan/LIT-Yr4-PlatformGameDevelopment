/*
	EnterName.h

	Joe O'Regan
	21/02/2018

	Change game Settings, musics and effects volumes etc
*/

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "cocos2d.h"

class Settings : public cocos2d::Layer {
public:	
	static cocos2d::Scene* createScene();			// Returns class instance pointer
		
	virtual bool init();							// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
		
	CREATE_FUNC(Settings);							// Static create() method implemented manually

	void initBackground();							// Initialise the background image

private:
	// Callbacks
	void returnToMenu(cocos2d::Ref *sender);		// Return to the MainMenu
	void musicVolUp(cocos2d::Ref *sender);			// Music volume up
	void musicVolDown(cocos2d::Ref *sender);		// Music volume down
	void sfxVolUp(cocos2d::Ref *sender);			// FX volume up
	void sfxVolDown(cocos2d::Ref *sender);			// FX volume down
	void skipTrackForwards(cocos2d::Ref *sender);	// FX volume up
	void skipTrackBackwards(cocos2d::Ref *sender);	// FX volume down
	
	// Screen size
	cocos2d::Size visibleSize;						// Visible screen size for the current platform
	cocos2d::Vec2 origin;							// Screen origin point

	cocos2d::Sprite* backgroundSprite;				// Sprite for background image

	__String *tempScore;							// Text to display

	// Labels
	cocos2d::Sprite* gameTitleSprite;				// Game name label
	cocos2d::Sprite* titleSprite;					// Scene name label
	cocos2d::Sprite* musicLbl;						// Music volume setting label
	cocos2d::Sprite* sfxLbl;						// SFX volume setting label
	cocos2d::Sprite* trackLbl;						// Track select setting label

	// Menu button images
	cocos2d::MenuItemImage* btnBackImg;				// Set image for Back Button menu option
	cocos2d::MenuItemImage* btnVolUpImg;			// Set image for volume up Button menu option
	cocos2d::MenuItemImage* btnVolDownImg;			// Set image for volume down Button menu option
	cocos2d::MenuItemImage* btnFXUpImg;				// Set image for FX volume up Button menu option
	cocos2d::MenuItemImage* btnFXDownImg;			// Set image for FX volume down Button menu option
	cocos2d::MenuItemImage* btnTrackUpImg;			// Select the next track Button menu option
	cocos2d::MenuItemImage* btnTrackDownImg;		// Select the previous track Button menu option

	// Menu buttons
	cocos2d::Menu* btnBack;
	cocos2d::Menu* btnVolUp;
	cocos2d::Menu* btnVolDown;
	cocos2d::Menu* btnFXUp;
	cocos2d::Menu* btnFXDown;
	cocos2d::Menu* btnTrackUp;
	cocos2d::Menu* btnTrackDown;

	// High Score
	cocos2d::LabelTTF* instructionLbl;			// Label to display current high score
	cocos2d::LabelTTF* currentTrackLbl;			// Label to display current background music track
	int trackSelect = 0;
	//std::string currentTrack[3];				// The name of the current track
};

#endif // __SETTINGS_H__