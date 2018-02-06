/*
	Joe O'Regan
	K00203642

	HUD.h

	Heads up display
	Shows level number, score, and time remaining each level
*/
#ifndef __HUD__
#define __HUD__

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

	void init(unsigned int &score, unsigned int &level, unsigned int &time) {
		score = 0;
		level = 1;
		time = 30;
	}

	void update() {
	
	}

private:
	HUD() {};																	// Constructor is private for use as a Singleton. Initializes the variables
	~HUD() {};																	// Private destructor

	static HUD* s_pInstance;													// Single instance of HUD used as singleton, so only one instance exists thoughout the game
};

#endif // __AUDIO__