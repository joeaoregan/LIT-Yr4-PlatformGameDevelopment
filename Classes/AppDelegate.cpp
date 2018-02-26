/*
	Joe O'Regan
	AppDelegate.cpp
	02/02/2018
*/
#include "AppDelegate.h"
#include "SplashScene.h"
#include "AudioMenu.h"
#include "MenuScene.h"
//#include "Level.h"
//#include "Settings.h"

AppDelegate::AppDelegate() {}	// Constructor
AppDelegate::~AppDelegate() {}	// Destructor

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	cocos2d::Director* director = cocos2d::Director::getInstance();					// JOR replaced auto specifier
	cocos2d::GLView* glview = director->getOpenGLView();							// JOR replaced auto specifier
    if(!glview) {
        glview = cocos2d::GLViewImpl::create("Space Game: Joe O'Regan K00203642");
        director->setOpenGLView(glview);
    }
	    
    director->setDisplayStats(false);												// Turn on/off display FPS
	    
    director->setAnimationInterval((float) (1.0 / 60));								// set FPS. the default value is 1.0/60 if you don't call this

	cocos2d::Scene* scene = SplashScene::createScene();								// Create the game splash screen, JOR replaced auto specifier
	//cocos2d::Scene* scene = AudioMenu::createScene();								// 20180223 Test Audio Menu with Menu base class
	//cocos2d::Scene* scene = MenuScene::createScene();								// 20180224 Levels with Level base class
	    
    director->runWithScene(scene);													// run

    return true;
}

// This function will be called when the app is inactive. 
// When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	cocos2d::Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	cocos2d::Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
