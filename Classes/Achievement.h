/*
	Achievement.h

	Joe O'Regan
	K00203642

	Log achievements in the game
	And update leaderboards
*/

#ifndef __ACHIEVEMENTS_H__
#define __ACHIEVEMENTS_H__

class Achievement {
public:
	// Achievement singleton
	static Achievement* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Achievement();
			return s_pInstance;
		}
		return s_pInstance;
	}

	Achievement() {}
	~Achievement() {}		

	void updateHighScores();					// Send the score to the global high scores table
	void updateLoserBoard();					// Add fastest time player was killed to loser leaderboard

	void achievementKill();						// Achievement, analytics and Leaderboard updates for reaching a certain percentage of kills
	void achievementEndLevel();					// Show player has completed a level
	void achievementScore();					// Player has placed on the high scores table
	void achievementGameComplete();				// Player has completed the game

	void analyticLevelStart();					// Notify when player starts a new level
	void analyticLoser();						// Show the time the player was killed
	void analyticPowerUp();						// Indicate player has collected a power up
	void analyticWeaponGrade(int grade);		// Display the weapons upgrade level after collecting power up
	void analyticExitTime();					// Time Player exited the game
	void analyticNewHighScore();				// Player has achieved a new high score
	
private:
	static Achievement* s_pInstance;			// Single instance of Achievement used as singleton, so only one instance exists thoughout the game
};

#endif // __ACHIEVEMENTS_H__