/*
	StoryScene.cpp

	Joe O'Regan
	K00203642
	03/03/2018

	Display the story between levels, the scene appears for 10 seconds, 
	or can be skipped with the continue button
*/

#ifndef __STORY_SCENE_H__
#define __STORY_SCENE_H__

#include "cocos2d.h"

class StoryScene : public cocos2d::Layer {
public:    
    static cocos2d::Scene* createScene();		// there's no 'id' in SpaceQuest, so we recommend returning the class instance pointer
	    
    virtual bool init();						// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    CREATE_FUNC(StoryScene);					// implement the "static create()" method manually
    
protected:										// Game Over Scene is a child of Story Scene

	//void sceneActions();						// Scale objects, not noticeable, could be transition overrides

	// Callbacks timed
	void GoToLevel1(float dt);
	void GoToLevel2(float dt);
	void GoToLevel3(float dt);
	void GoToLevel4(float dt);

	// Callbacks continue Button, skip
	void continueToLevel1(Ref* pSender);
	void continueToLevel2(Ref* pSender);
	void continueToLevel3(Ref* pSender);
	void continueToLevel4(Ref* pSender);
	void backToMenu(Ref* pSender);

	cocos2d::Size m_pVisibleSize;				// Screen resolution

	cocos2d::Sprite* m_pGameTitleImg;			// Scene title image

	cocos2d::Label* m_pLevelLbl;				// Current level number
	cocos2d::Label* m_pStoryLbl;				// Display the story
	cocos2d::Label* m_pContinueLbl;				// Continue button text
	cocos2d::Label* m_pReturnToMenuLbl;			// REturn to menu button text

	cocos2d::MenuItemLabel* m_pContinueItem;	// Continue to next level
	cocos2d::MenuItemLabel* m_pReturnToMenuItem;// Return to main menu

	cocos2d::Menu* m_menu;						// Create the menu options
};

#endif // __STORY_SCENE_H__
