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
	static cocos2d::Scene* createScene();			// Returns class instance pointer
		
	virtual bool init();							// init() returns bool in cocos2d-x, instead of returning 'id' in cocos2d-iphone
		
	CREATE_FUNC(HighScores);						// Static create() method implemented manually
	
	std::string sortScores();						// Sort the high scores
	void resetScores();								// Reset the scores
	void saveScores();								// Save the names and scores to UserDefaults

	
	// Social Media & Analytics
	cocos2d::MenuItemImage* m_pAchievmentItem2;		// Show Achievements
	cocos2d::MenuItemImage* m_pLeaderboardsItem2;	// Show Leaderboards

private:

	// High Score
	std::string m_scoresStr;						// Text to display	
	cocos2d::Label* m_allScoresLbl;					// Label to display current high score

	//void scoreAchievement();						// Player has achieved a place on the high scores table
};

#endif // __HIGH_SCORES_H__