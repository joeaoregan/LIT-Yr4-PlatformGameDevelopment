/*
	Joe O'Regan
	AppDelegate.cpp
	02/02/2018
*/
#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Space Game: Joe O'Regan K00203642");
        director->setOpenGLView(glview);
    }
	    
    director->setDisplayStats(true);										// turn on display FPS
	    
    director->setAnimationInterval(1.0 / 60);								// set FPS. the default value is 1.0/60 if you don't call this
	    
    auto scene = HelloWorld::createScene();									// create a scene. it's an autorelease object
	    
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
