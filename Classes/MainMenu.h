/*
	MainMenu.h

	Joe O'Regan
	K00203642

	Game main menu, from here the player can start the game, view the highscores, or exit the game
*/

#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "MenuScene.h"
#include "MusicPlayer.h"

class MainMenu : public MenuScene {
public:
	static cocos2d::Scene* createScene();		// Returns class instance pointer
		
	virtual bool init();						// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
	virtual void update(float dt);				// Update the menu
		
	CREATE_FUNC(MainMenu);						// Static create() method implemented manually

	/*
	MenuItemImage* getButton(int button) {
		MenuItemImage *result;

		switch (button) {
		case 1: result = MainMenu::exitItem; break;		// Exit button
		case 2: result = MainMenu::optionsItem; break;	// Settings button
		case 3: result = MainMenu::scoreItem; break;	// High Scores button
		case 4: result = MainMenu::playItem; break;		// Play Game button
		default: break;
		}

		return result;
	};
	*/

	void highlightButton(unsigned int m_currentBtn);


private:
	// Callbacks
	void StartGame(cocos2d::Ref *sender);		// Start the game
	void GoToScores(cocos2d::Ref *sender);		// Go to the high scores table
	void GoToSettings(cocos2d::Ref *sender);	// Go to the options scene
	void GoToEnterName(cocos2d::Ref *sender);	// Go to the enter name scene
	
	// Menu Images
	cocos2d::MenuItemImage* playItem;			// Start the game
	cocos2d::MenuItemImage*	scoreItem;			// Go to the high scores scene
	cocos2d::MenuItemImage* optionsItem;		// Go to the options scene
	cocos2d::MenuItemImage* exitItem;			// Exit the game

	// Menu Buttons
	cocos2d::Menu* menu;						// Button 1 start the game

	// High Score
	int highScore;								// Load the first sorted score, which is the high score
	cocos2d::__String *tempScore;				// Score text to display
	cocos2d::Label* highScoreLbl;				// Label to display the score text

	// Music Player
	MusicPlayer *mplayer;						// Music Player

	// 20180223
	// Keyboard and gamepad
	//int currentBtn;								// Current selected menu button for keyboard / gamepad
	//int numButtons;								// The number of buttons in the menu/state
};

#endif // __MAIN_MENU_H__