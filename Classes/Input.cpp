#include "Input.h"

//Input* Input::s_pInstance;																// Singleton for Input

bool Input::isKeyPressed(cocos2d::EventKeyboard::KeyCode code) {	
	if (keys.find(code) != keys.end())														// Check if the key is currently pressed by seeing it it's in the std::map keys
		return true;

	return false;
}

double Input::keyPressedDuration(cocos2d::EventKeyboard::KeyCode code) {
	if (!isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_CTRL))
		return 0;																			// Not pressed, so no duration yet

	// Return the amount of time that has elapsed between now and when the user
	// first started holding down the key in milliseconds
	// Start time is the value we hold in our std::map keys
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - keys[code]).count();
}

void Input::init(cocos2d::Layer *layer, cocos2d::EventDispatcher *eventDispatcher) {
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) {
		cocos2d::EventListenerKeyboard* eventListener = EventListenerKeyboard::create();	// JOR replaced auto specifier
		Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
		eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
			// If a key already exists, do nothing as it will already have a time stamp
			// Otherwise, set's the timestamp to now
			if (keys.find(keyCode) == keys.end()) {
				keys[keyCode] = std::chrono::high_resolution_clock::now();
			}
		};
		eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {			
			keys.erase(keyCode);															// remove the key.  std::map.erase() doesn't care if the key doesnt exist
		};

		eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, layer);
	}
}