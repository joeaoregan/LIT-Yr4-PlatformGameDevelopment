/*
	Joe O'Regan
	K00203642

	Input.h
*/
#ifndef __INPUT__
#define __INPUT__

#include "cocos2d.h"

class Input {
public:
	static Input* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Input();
			return s_pInstance;
		}
		return s_pInstance;
	}
	
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode code) {
		// Check if the key is currently pressed by seeing it it's in the std::map keys
		// In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
		if (keys.find(code) != keys.end())
			return true;
		return false;
	}

	double keyPressedDuration(cocos2d::EventKeyboard::KeyCode code) {
		if (!isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_CTRL))
			return 0;  // Not pressed, so no duration obviously

					   // Return the amount of time that has elapsed between now and when the user
					   // first started holding down the key in milliseconds
					   // Obviously the start time is the value we hold in our std::map keys
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - keys[code]).count();
	}

	void init(cocos2d::Layer *layer, cocos2d::EventDispatcher *eventDispatcher) {
		auto eventListener = EventListenerKeyboard::create();
		Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
		eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
			// If a key already exists, do nothing as it will already have a time stamp
			// Otherwise, set's the timestamp to now
			if (keys.find(keyCode) == keys.end()) {
				keys[keyCode] = std::chrono::high_resolution_clock::now();
			}
		};
		eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
			// remove the key.  std::map.erase() doesn't care if the key doesnt exist
			keys.erase(keyCode);
		};

		eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, layer);
	}

private:
	Input() {};																			// Constructor is private for use as a Singleton. Initializes the variables
	~Input() {};																		// Private destructor shuts down and cleans up the mixer API

	static Input* s_pInstance;															// Single instance of Input used as singleton so only one instance exists thoughout the game
	
	// Keyboard
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
};


#endif // __INPUT__