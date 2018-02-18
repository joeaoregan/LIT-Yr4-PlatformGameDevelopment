/*
	Joe O'Regan
	AppDelegate.cpp
	02/02/2018
*/
#include "AppDelegate.h"
#include "GameScene.h"
#include "SplashScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	//auto director = Director::getInstance();
	//auto glview = director->getOpenGLView();
	cocos2d::Director* director = Director::getInstance();
	cocos2d::GLView* glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Space Game: Joe O'Regan K00203642");
        director->setOpenGLView(glview);
    }
	    
    director->setDisplayStats(false);										// Turn on/off display FPS
	    
    director->setAnimationInterval(1.0 / 60);								// set FPS. the default value is 1.0/60 if you don't call this

	//auto scene = GameScene::createScene();								// create a scene. it's an autorelease object
	cocos2d::Scene* scene = SplashScene::createScene();						// Create the game splash screen
	    
    director->runWithScene(scene);											// run

    return true;
}

// This function will be called when the app is inactive. 
// When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
