/*
	Joe O'Regan
	K00203642

	Input.h
*/
#ifndef __INPUT__
#define __INPUT__

#include "cocos2d.h"

USING_NS_CC;


class Input {
public:
	static Input* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Input();
			return s_pInstance;
		}
		return s_pInstance;
	}
	
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode code);

	double keyPressedDuration(cocos2d::EventKeyboard::KeyCode code);

	void init(cocos2d::Layer *layer, cocos2d::EventDispatcher *eventDispatcher);

	void update() {	}	// will update in class later

private:
	Input() {};																					// Constructor is private for use as a Singleton. Initializes the variables
	~Input() {};																				// Private destructor shuts down and cleans up the mixer API

	static Input* s_pInstance;																	// Single instance of Input used as singleton so only one instance exists thoughout the game
	
	// Keyboard
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;									
};


#endif // __INPUT__