/*
	Joe O'Regan
	K00203642

	HUD.h

	Heads up display
	Shows level number, score, and time remaining each level
*/

#ifndef __HUD__
#define __HUD__

#define MAX_PLAYER_LIVES 5											// The maximum number of lives a player can have

/*
// Needed to use to_string method with android
template <typename T>
std::string to_string(T value) {
	std::ostringstream os;
	os << value;
	return os.str();
}
*/

class HUD : public cocos2d::Node {
public:
	// HUD singleton
	static HUD* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new HUD();
			return s_pInstance;
		}
		return s_pInstance;
	}

	HUD() {};														// Constructor is private for use as a Singleton. Initializes the variables
	~HUD() {};														// Private destructor

	static HUD *create(cocos2d::Point position, cocos2d::Size res);	// Create the HUD at the position specified
	void init(cocos2d::Layer *layer);								// Initialise the HUD
	void update(float curTimeMillis);								// Update the HUD, passing the time in milliseconds for the timer

	// Update variables
	//void updateLives();											// Update the displayed number of lives sprites
	void updateTimer(float curTimeMillis);							// 20180204 Update the countdown timer, 21/02/2018 Passing curTimeMillis solves Android timer issue
	void setLevelLabel();											// Update the level number for levels after 1

private:
	cocos2d::Size m_visibleSize;									// Screen resolution changes depending on the platform

	static HUD* s_pInstance;										// Static pointer for single instance of HUD used as singleton, so only one instance exists thoughout the game
	
	cocos2d::Label *m_pLevelLabel;									// Display the current level
	cocos2d::Label *m_pScoreLabel;									// Display the current score
	cocos2d::Label *m_pTimeLabel;									// Display the time remaining

	cocos2d::MenuItemImage* m_pCloseItem;							// Exit button in bottom right corner
	cocos2d::MenuItemImage* m_pMenuItem;							// Menu sprite
	cocos2d::Menu* m_pMenuClose;									// Menu items

	// Lives
	cocos2d::Sprite *m_pPlayerLife;									// Indicate lives left
	cocos2d::Sprite* m_pLivesList[MAX_PLAYER_LIVES];				// List of lives

	// Time
	float m_currentTime;											// 20180221 Change to float to fix Android timer issue
	unsigned int m_time;											// Countdown timer displayed time
};

#endif // __AUDIO__