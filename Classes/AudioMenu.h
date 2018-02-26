/*
	AudioMenu.h

	Joe O'Regan
	K00203642
	21/02/2018

	Audio settings menu

	This class inherits from MenuScene (Menu is already used by Cocos2d-x so could not be used as the name)
	MenuScene is a subclass of Layer.
*/

#ifndef __AUDIO_MENU_H__
#define __AUDIO_MENU_H__

#include "MenuScene.h"
//#include "cocos2d.h"

class AudioMenu : public MenuScene {
public:
	static cocos2d::Scene* createScene();			// Returns class instance pointer

	virtual bool init();							// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone

	CREATE_FUNC(AudioMenu);							// Static create() method implemented manually

private:
	// Callbacks
	void musicVolUp(cocos2d::Ref *sender);			// Music volume up
	void musicVolDown(cocos2d::Ref *sender);		// Music volume down
	void sfxVolUp(cocos2d::Ref *sender);			// FX volume up
	void sfxVolDown(cocos2d::Ref *sender);			// FX volume down
	void skipTrackForwards(cocos2d::Ref *sender);	// FX volume up
	void skipTrackBackwards(cocos2d::Ref *sender);	// FX volume down

	// Labels
	cocos2d::Sprite* musicLbl;						// Music volume setting label
	cocos2d::Sprite* sfxLbl;						// SFX volume setting label
	cocos2d::Sprite* trackLbl;						// Track select setting label
	cocos2d::Label* instructionLbl;					// Label to display current high score
	cocos2d::Label* currentTrackLbl;				// Label to display current background music track

	// Menu button images
	cocos2d::MenuItemImage* btnBackImg;				// Set image for Back Button menu option
	cocos2d::MenuItemImage* btnVolUpImg;			// Set image for volume up Button menu option
	cocos2d::MenuItemImage* btnVolDownImg;			// Set image for volume down Button menu option
	cocos2d::MenuItemImage* btnFXUpImg;				// Set image for FX volume up Button menu option
	cocos2d::MenuItemImage* btnFXDownImg;			// Set image for FX volume down Button menu option
	cocos2d::MenuItemImage* btnTrackUpImg;			// Select the next track Button menu option
	cocos2d::MenuItemImage* btnTrackDownImg;		// Select the previous track Button menu option

	// Menu
	cocos2d::Menu* menu;							// Add the menu options to a menu list		
};

#endif // __AUDIO_MENU_H__