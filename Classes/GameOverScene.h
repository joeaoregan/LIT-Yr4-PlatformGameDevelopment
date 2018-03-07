/*
GameOverScene.cpp

Joe O'Regan
K00203642
06/03/2018

Display the game over message for use with the game trailer
*/

#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "StoryScene.h"

class GameOverScene : public StoryScene {
public:    
    static cocos2d::Scene* createScene();	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();							// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    CREATE_FUNC(GameOverScene);						// implement the "static create()" method manually
    
private:
	
	void returnToMenu(float dt);

	void restartGame(cocos2d::Ref* pSender);
	void showCredits(cocos2d::Ref* pSender);
	void backToMenu(cocos2d::Ref* pSender);

	//bool m_victorious;
};

#endif // __GAME_OVER_SCENE_H__
