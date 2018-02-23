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
	void update(float dt);						// Update the menu
		
	CREATE_FUNC(MainMenu);						// Static create() method implemented manually

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
	cocos2d::LabelTTF* highScoreLbl;			// Label to display the score text
	cocos2d::LabelTTF* currentPlayer;			// Label to display the current Player name

	// Music Player
	MusicPlayer *mplayer;						// Music Player
};

#endif // __MAIN_MENU_H__