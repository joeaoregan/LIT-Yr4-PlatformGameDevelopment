/*
	HighScores.h

	Joe O'Regan
	20/02/2018

	Display the stored high scores
*/

#ifndef __HIGH_SCORES_H__
#define __HIGH_SCORES_H__

#include "MenuScene.h"

class HighScores : public MenuScene {
public:	
	static cocos2d::Scene* createScene();		// Returns class instance pointer
		
	virtual bool init();						// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
		
	CREATE_FUNC(HighScores);					// Static create() method implemented manually
	
	void sortScores();							// Sort the high scores
	void resetScores();							// Reset the scores
	void saveScores();							// Save the names and scores to UserDefaults
private:

	// High Score
	__String *tempScore;						// Text to display	
	//cocos2d::LabelTTF* highScoreLbl;			// Label to display current high score
	cocos2d::Label* highScoreLbl;				// Label to display current high score
};

#endif // __HIGH_SCORES_H__