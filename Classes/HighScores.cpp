/*
	HighScores.cpp

	Joe O'Regan
	K00203642

	Display a leaderboard of up to 10 scores
*/

#include "Game.h"
#include "HighScores.h"
#include "MainMenu.h"
//#include "HUD.h"

#define MAX_SCORES_DISPLAYED 10

// Arrays to store player names and scores
unsigned int arrScores[MAX_SCORES_DISPLAYED + 1];								// Array of scores +1 for sorting
std::string arrNames[MAX_SCORES_DISPLAYED + 1];									// Array of names +1 for sorting

Scene* HighScores::createScene() {
	cocos2d::Scene* scene = Scene::create();		// 'scene' is an autorelease object, JOR replaced auto specifier
	layer = HighScores::create();					// 'layer' is an autorelease object, JOR replaced auto specifier		
	scene->addChild(layer);							// Add layer as a child to scene
		
	return scene;									// Return the scene
}

// on "init" you need to initialize your instance
bool HighScores::init() {
	MenuScene::init();															// base class function

	//resetScores();															// Test resetting scores

	titleSprite->setTexture("HighScores.png");									// Change the title image text
		
	m_scoresStr = sortScores();													// set value for tempScore
		
	//m_allScoresLbl = cocos2d::Label::createWithTTF(tempScore->getCString(),	// Label to display current high score	
	m_allScoresLbl = cocos2d::Label::createWithTTF(m_scoresStr ,
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.06f);				// Label to display current high score (Label replaces LabelTTF causing warnings)
	m_allScoresLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, 
		visibleSize.height * 0.4f + origin.y));									// Set its position on screen
	m_allScoresLbl->setColor(Color3B::WHITE);									// Set the text colour
	this->addChild(m_allScoresLbl);												// Add it to the layer

	return true;
}

// Sort the scores, and display them using a Label
std::string HighScores::sortScores() {
	int swapScore = 0;
	std::string tempName = "";
	UserDefault* def = UserDefault::getInstance();
	std::string scoreLabelText;

	char scoreTxt[12] = "Score";												// Used to search for user default keys
	
	// Load the names and scores from User Defaults
	for (int i = 1; i <= MAX_SCORES_DISPLAYED; i++) {
		sprintf(scoreTxt, "Score%d", i);
		arrScores[i - 1] = def->getIntegerForKey(scoreTxt);						// search for Score1, Score2, Score3, etc.
		sprintf(scoreTxt, "Name%d", i);
		arrNames[i - 1] = def->getStringForKey(scoreTxt);						// search for Name1, Name2, Name3, etc.
	}

	// Set the current players name and score as the last element in each list
	arrScores[MAX_SCORES_DISPLAYED] = Game::Instance()->getScore();
	arrNames[MAX_SCORES_DISPLAYED] = Game::Instance()->getPlayerName();			// Get the players name

	// Set scores table heading, indicating no scores saved, or new high score
	if (Game::Instance()->getScore() > arrScores[0])							// If the current score is greater than the high score
		scoreLabelText = "New High Score:";
	else if (arrScores[0] == 0)
		scoreLabelText = "No High Score Yet";
	else
		scoreLabelText = "Leaderboard:";

	// Sort the scores highest to lowest
	for (int x = 1; x < (MAX_SCORES_DISPLAYED + 1); x++) {						// Check all the scores
		for (int y = 0; y < (MAX_SCORES_DISPLAYED + 1) - x; y++) {				// Against every other score
			if (arrScores[y] < arrScores[y + 1]) {								// Sorting the largest score first
				swapScore = arrScores[y];										// Do swapping
				tempName = arrNames[y];

				arrScores[y] = arrScores[y + 1];
				arrNames[y] = arrNames[y + 1];

				arrScores[y + 1] = swapScore;
				arrNames[y + 1] = tempName;
			}
		}
	}
	
	// Display updated scores
	for (int i = 0; i < MAX_SCORES_DISPLAYED; i++) {
		if (arrScores[i] == 0) continue;										// No need to display if score is 0

		//scoreLabelText += "\n" + StringUtils::toString(i + 1) + ".\t" +
		//	arrNames[i] + "\t " + StringUtils::toString(arrScores[i]);
		scoreLabelText += "\n" + StringUtils::toString(i + 1) + ".  " +
			arrNames[i] + "  " + StringUtils::toString(arrScores[i]);
	} 
	
	saveScores();																// Save the newly sorted socres

	CCLOG("%s", scoreLabelText.c_str());
	return scoreLabelText;
}

void HighScores::saveScores() {
	char scoreTxt[12] = "Score";												// Used to store user default keys

	// Set the new values in PlayerPrefs
	for (int i = 1; i <= MAX_SCORES_DISPLAYED; i++) {
		sprintf(scoreTxt, "Score%d", i);
		def->setIntegerForKey(scoreTxt, arrScores[i - 1]);						// Write the ordered score back to PlayerPrefs
		sprintf(scoreTxt, "Name%d", i);
		def->setStringForKey(scoreTxt, arrNames[i - 1]);						// and the player name
	}

	def->flush();
}

void HighScores::resetScores() {
	for (int i = 0; i < MAX_SCORES_DISPLAYED; i++) {
		arrScores[i] = 0;
		arrNames[i] = "";
	}

	saveScores();
}