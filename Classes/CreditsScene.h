/*
	CreditsScene.h

	Joe O'Regan
	K00203642
	06/03/2018

	Display game over credits
*/

#ifndef __CREDITS_SCENE_H__
#define __CREDITS_SCENE_H__

#include "StoryScene.h"

class CreditsScene : public StoryScene {
public:    
    static cocos2d::Scene* createScene();	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();							// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    CREATE_FUNC(CreditsScene);						// implement the "static create()" method manually
    
private:
	cocos2d::Label * creditLbl;
	std::string message;

	void returnToMenu(float dt);

	void restartGame(cocos2d::Ref* pSender);
	void backToMenu(cocos2d::Ref* pSender);

	void credit1();
	void credit2();
	void credit3();
	void credit4();

	//bool m_victorious;
};

#endif // __CREDITS_SCENE_H__
