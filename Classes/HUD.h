/*
	Joe O'Regan
	K00203642

	HUD.h

	Heads up display
	Shows level number, score, and time remaining each level
*/
#ifndef __HUD__
#define __HUD__

//#include "Level1.h"

// Needed to use to_string method with android
template <typename T>
std::string to_string(T value) {
	std::ostringstream os;
	os << value;
	return os.str();
}

class HUD{
public:
	// HUD singleton
	static HUD* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new HUD();
			return s_pInstance;
		}
		return s_pInstance;
	}

	//void init(unsigned int &score, unsigned int &level, unsigned int &time, cocos2d::Layer *layer);
	void init(unsigned int &time, cocos2d::Layer *layer);

	void update() {
		//scoreLabel->setString("Score: " + to_string(GameScene::Instance()->getScore()));								// Update the displayed score
	}

private:
	HUD() {};																											// Constructor is private for use as a Singleton. Initializes the variables
	~HUD() {};																											// Private destructor

	static HUD* s_pInstance;																							// Single instance of HUD used as singleton, so only one instance exists thoughout the game
	
	cocos2d::Label* levelLabel;																							// Display the current level
	//cocos2d::Label* scoreLabel;																						// Display the current score
};

#endif // __AUDIO__