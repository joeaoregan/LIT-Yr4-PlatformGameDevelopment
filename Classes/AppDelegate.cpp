/*
	Joe O'Regan
	AppDelegate.cpp
	02/02/2018

	Changed window name
*/

#include "AppDelegate.h"
#include "SplashScene.h"
#include "GameOverScene.h"
#include "Level2.h"
#include "CreditsScene.h"
#include "SignInScene.h"		// Sign into Google Play
#include "Level4.h"		// Sign into Google Play

AppDelegate::AppDelegate() {}	// Constructor
AppDelegate::~AppDelegate() {}	// Destructor

/*
	Include SDKBox and Google Analytics for Android only
*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"							// 20180307 Google Analytics
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"										// 20180307 SDKBox
#endif

bool AppDelegate::applicationDidFinishLaunching() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdkbox::PluginGoogleAnalytics::init();											// 20180307 Google Analytics
	sdkbox::PluginSdkboxPlay::init();												// 20180314 SDKBox Play
#endif

    // initialize director
	cocos2d::Director* director = cocos2d::Director::getInstance();					// JOR replaced auto specifier
	cocos2d::GLView* glview = director->getOpenGLView();							// JOR replaced auto specifier
    if(!glview) {
        glview = cocos2d::GLViewImpl::create("Space Game: Joe O'Regan K00203642");
        director->setOpenGLView(glview);
    }
	    
    director->setDisplayStats(false);												// Turn on/off display FPS
	    
    director->setAnimationInterval((float) (1.0 / 60));								// set FPS. the default value is 1.0/60 if you don't call this

	//cocos2d::Scene* scene = GameOverScene::createScene();							// Create the game over screen, JOR replaced auto specifier

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)										// If the platform is Android
	cocos2d::Scene* scene = SignInScene::createScene();								// Create the game sign in screen, JOR replaced auto specifier
#else
	//cocos2d::Scene* scene = SplashScene::createScene();								// Create the game splash screen, JOR replaced auto specifier
	cocos2d::Scene* scene = Level4::createScene();								// Create the game splash screen, JOR replaced auto specifier
#endif   
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
