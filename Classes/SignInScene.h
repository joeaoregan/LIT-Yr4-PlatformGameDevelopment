#ifndef __SIGN_IN_SCENE_H__
#define __SIGN_IN_SCENE_H__

#include "cocos2d.h"

class SignInScene : public cocos2d::Layer {
public:    
    static cocos2d::Scene* createScene();	// there's no 'id' in SpaceQuest, so we recommend returning the class instance pointer
	    
    virtual bool init();					// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    CREATE_FUNC(SignInScene);				// implement the "static create()" method manually
    
private:
    void GoToSplashScene( float dt );		// Go to main menu call back
};

#endif // __SPLASH_SCENE_H__
