/*
	Achievement.cpp

	Joe O'Regan
	K00203642

	Log achievements in the game with analytics
	Send updates to leaderboards
	Unlock Google Play Services Achievements
*/

#include "Level.h"
#include "Achievement.h"
#include "Game.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"																		// For leaderboard
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"															// 20180422 Google Analytics
#endif
	
Achievement* Achievement::s_pInstance;																				// Game Singleton

/*
	Calculate the time the player has played the game for
*/
int timeInGame() { 
	int levelDuration = (int) Game::Instance()->getLevelDuration() / 1000;											// Convert level duration from millis to secs
	int timeOnClock = (int)Game::Instance()->getTimer();															// Time on countdown timer						
	int timePreviousLevel = (int)(Game::Instance()->getLevel() - 1) * Game::Instance()->getLevelDuration();			// Time elapsed in level added to time for previous levels

	return levelDuration - timeOnClock + timePreviousLevel;													
}

/*
	Leaderboard: Register the lowest time when the player died in the game
*/
void Achievement::updateLoserBoard() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginSdkboxPlay::submitScore("Loser Board (Quckest Deaths)", timeInGame());							// Submit the time the player lasted in the game
	analyticLoser();
#endif
}

/*
	Analytics: Display statistic for time player died
*/
void Achievement::analyticLoser() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string deathTime = "Player Died after " + cocos2d::StringUtils::toString(timeInGame()) + " seconds";
	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Loser", deathTime, 5);									// Google Analytics
#endif
}

/*
	Achievement: Completed each level for console and analytics
*/
void Achievement::achievementEndLevel() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string achievementStr = "Level " + 
		cocos2d::StringUtils::toString(Game::Instance()->getLevel()) + " Complete";									// Generic Level achievement

		sdkbox::PluginSdkboxPlay::unlockAchievement(achievementStr);												// Console Achievement

		sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Level " +
			cocos2d::StringUtils::toString(Game::Instance()->getLevel()) + " Finished", 5);							// Google Analytics
#endif
}

/*
	Analytics: Display power up collected
*/
void Achievement::analyticPowerUp() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Collect Power Up", 5);						// Google Analytics
#endif
}

/*
	Analytics: Display level of weapon upgrade after collecting power up
*/
void Achievement::analyticWeaponGrade(int grade) {
	//CCLOG("***************************** WEAPON UPGRADED ***************************");
	//CCLOG("Weapon Upgrade Level: %d", grade);																		// Show the upgrade level in the Output pane
	std::string upgradeStr = "Laser Upgrade Level: " + cocos2d::StringUtils::toString(grade);						// Create string to sent to Analytics

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Upgrade", upgradeStr, 5);
		//"Upgrade", upgrade.str(), 5);																				// const char 17
#endif
}


/*
	Social Media and Analytics
	Unlock asteroid / enemy kills achievement
	If the kill rate is better than 50% for either Enemy Ships or Asteroids
	Or it the player completes the level / dies and destroys no objects with lasers
*/
void Achievement::achievementKill() {
	//CCLOG("Kill Rate Achievement");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginSdkboxPlay::submitScore("Asteroid Count", Game::Instance()->getAsteroidKills());					// Add the asteroid kill count to the leaderboard

	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Statistic", "Asteroids Destroyed Level " +				// Asteroid kill statistic
		cocos2d::StringUtils::toString(Game::Instance()->getLevel()) + ": " +										// Add the level number
		cocos2d::StringUtils::toString(Game::Instance()->getAsteroidKills()), 5);									// Google Analytics: Register the number of asteroids destroyed

	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Statistic", "Enemies Killed Level " +					// Enemy kill statistic
		cocos2d::StringUtils::toString(Game::Instance()->getLevel()) + ": " +										// Add the level number
		cocos2d::StringUtils::toString(Game::Instance()->getEnemyShipKills()), 5);									// Google Analytics: Register the number of enemies destroyed

	sdkbox::PluginSdkboxPlay::submitScore("Enemies Destroyed", Game::Instance()->getEnemyShipKills());				// Add the enemy kill count to the leaderboard
//#endif

	// Player kill 50% of enemies or asteroids
	if (!Game::Instance()->getAchievedKills()) {																	// If the achievement isn't complete
		if (Game::Instance()->getAsteroidKills() / 
			(float) Game::Instance()->getAsteroidCount() >= 0.5f ||													// Player gets a kill percentage of 50% or more for
			Game::Instance()->getEnemyShipKills() / 
			(float) Game::Instance()->getEnemyShipCount() >= 0.5f) {												// Asteroids or enemy ships
			CCLOG("Destroyed 50 Percent Or More Asteroids Or Enemy Ships");
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Target Practice", 5);				// Google Analytics: Register the Expert Marksman achievement
			sdkbox::PluginSdkboxPlay::unlockAchievement("Target Practice");											// Console Achievement killed 75% of enemies
//#endif
			Game::Instance()->setAchievedKills(true);																// Mark the achievement as true
		}
	}

	// Player kill over 75% of enemies
	if (Game::Instance()->getEnemyShipKills() /
		(float)Game::Instance()->getEnemyShipCount() >= 0.75f) {
		sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Expert Shooter", 5);					// Google Analytics: Register killed 75% of enemies
		sdkbox::PluginSdkboxPlay::unlockAchievement("Expert Shooter");												// Console Achievement killed 75% of enemies
	}
	// Player kill over 90% of enemies
	if (Game::Instance()->getEnemyShipKills() /
		(float)Game::Instance()->getEnemyShipCount() >= 0.9f) {
		sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Kill Em All", 5);						// Google Analytics: Register killed 90% of enemies
		sdkbox::PluginSdkboxPlay::unlockAchievement("Kill Em All");													// Console Achievement killed 90% of enemies
	}
#endif
/*
// Test win
	if (!Game::Instance()->getAchievedKills()) {																// If the achievement isn't complete
		if (Game::Instance()->getAsteroidKills() / (float) Game::Instance()->getAsteroidCount() >= 0.5f) {		// Player gets a kill percentage of 50% or more
			CCLOG("Destroyed 50 Or More Asteroids %.2f", Game::Instance()->getAsteroidKills() / (float) Game::Instance()->getAsteroidCount());
			Game::Instance()->setAchievedKills(true);															// Mark the achievement as true
		}
	}

	if (!Game::Instance()->getAchievedKills()) {																// If the achievement isn't complete
		if (Game::Instance()->getEnemyShipKills() / (float) Game::Instance()->getEnemyShipCount() >= 0.5f) {
			CCLOG("Destroyed 50 Or More Enemy Ships %.2f", Game::Instance()->getEnemyShipKills() / (float) Game::Instance()->getEnemyShipCount());
			Game::Instance()->setAchievedKills(true);															// Mark the achievement as true
		}
	}
*/
	/*
		Kamikaze Achievement

		Player has completed the level / died
		without getting a laser on target
	*/
	if (!Game::Instance()->getAchievedKamikaze()) {																	// If the achievement isn't complete
		if (Game::Instance()->getAsteroidKills() / (
			float) Game::Instance()->getAsteroidCount() == 0.0f &&													// Player didn't destroy any asteroids
			Game::Instance()->getEnemyShipKills() /																	// and didn't destroy any enemy ships 
			(float) Game::Instance()->getEnemyShipCount() == 0.0f) {
			CCLOG("Destroyed Nothing %.2f %.2f", (float) (Game::Instance()->getAsteroidKills() /					// Display the percentages in output
				Game::Instance()->getAsteroidCount()),
				(float) (Game::Instance()->getEnemyShipKills() / 
				Game::Instance()->getEnemyShipCount() == 0.0f));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Kamikaze", 5);						// Google Analytics: Register the Kamikaze achievement
#endif
			Game::Instance()->setAchievedKamikaze(true);															// Mark the achievement as achieved
		}
	}
}

/*
	Time Player exited the game
*/
void Achievement::analyticExitTime() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//	sdkbox::PluginGoogleAnalytics::logTiming("StartGame", 0, "Test Time", "Level init");						// Google Analytics: Register game exit time for menu button 
	std::string exitTimeStr = "Exit Time: " + cocos2d::StringUtils::toString(Game::Instance()->getTimer());			// Create exit time string to send to Analytics
	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Exit Time", exitTimeStr, 5);							// Google Analytics: Register the Kamikaze achievement
#endif
}

/*
	Achievement: Player has achieved a place on the high scores table
*/
void Achievement::achievementScore() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Scores", "Score Table Position", 5);				// Google Analytics
#endif
}

/*
	Analytics: New High Score (GameOverScene::scoreAchievement())
*/
void Achievement::analyticNewHighScore() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Scores", "New High Score", 5);							// Google Analytics
#endif
}

/*
	Analytics: Notify when player starts a new level
*/
void Achievement::analyticLevelStart() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string levelStartStr = "Level " + cocos2d::StringUtils::toString(Game::Instance()->getLevel())  +" Started";
	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", levelStartStr, 5);							// Google Analytics
#endif
}

/*
	Leaderboard: Submit score to Global High Scores table
*/
void Achievement::updateHighScores() {
	CCLOG("Update Leaderboard");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginSdkboxPlay::submitScore("Global High Scores", Game::Instance()->getScore());						// Add the score to the leaderboard at the end of each level win or lose
#endif
}

/*
	Achievement: Player has completed the game
*/
void Achievement::achievementGameComplete() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", "Unlocked", "Game Finished", 5);							// Google Analytics
	sdkbox::PluginSdkboxPlay::unlockAchievement("Game Complete");													// Achievement
#endif
}
