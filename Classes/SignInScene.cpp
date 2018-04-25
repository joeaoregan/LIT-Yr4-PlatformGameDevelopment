/*
	SignInScene.cpp

	Joe O'Regan
	K00203642

	Added for assignment 3
	Sign into Google Play
*/

#include "Game.h"											// Function to check if signed in already
#include "SignInScene.h"
#include "SplashScene.h"
#include "Defines.h"										// Include definitions
//#include "XML.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"				// For leaderboard, and analytics
#endif

cocos2d::Scene* SignInScene::createScene() {
    cocos2d::Scene* scene = cocos2d::Scene::create();		// Create the scene. 'scene' is an autorelease object
        
	SignInScene* layer = SignInScene::create();				// Create the layer. 'layer' is an autorelease object
	    
    scene->addChild(layer);									// Add layer as a child to scene
	    
    return scene;											// Return the scene
}

/*
	Initialise the splash scene, animating and fading the background image
*/
bool SignInScene::init() {
    // 1. super init first
    if ( !Layer::init() ) { return false; }
	    
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();												// Get screen resolution
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();												// Get screen origin

	//float scale = (visibleSize.height == 1080) ? 1.0f : (visibleSize.height == 720) ? 0.67f : visibleSize.height / 1080;		// Kindle resolution is different
	float scale = (visibleSize.height == 1080) ? 1.5f : (visibleSize.height == 720) ? 1.0f : visibleSize.height / 1080 * 1.5f;	// Kindle resolution is different

    this->scheduleOnce( schedule_selector(SignInScene::GoToSplashScene ), DISPLAY_TIME_SPLASH_SCENE );

    cocos2d::Sprite* backgroundSprite = cocos2d::Sprite::create( "SignInBG.png" );												// Create the background sprite
    backgroundSprite->setPosition(cocos2d::Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );		// Set the background position    
	backgroundSprite->setScale(scale);																							// Scale down the image size for lower resolution
    this->addChild( backgroundSprite );																							// Add background sprite as a child of the layer
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (!Game::Instance()->getSignedIn()) {																						// If not already signed in
		sdkbox::PluginSdkboxPlay::signin();																						// Sign in for leaderboard
		Game::Instance()->setSignedIn(true);																					// Make sure it only does this once																
	}
#endif

    return true;
}

// Callbacks

/*
	Go to main menu when time has passed
*/
void SignInScene::GoToSplashScene( float dt ) {
    cocos2d::Scene* scene = SplashScene::createScene();																			// Create the SplashScene scene    
	cocos2d::Director::getInstance( )->replaceScene(cocos2d::TransitionFade::create( TRANSITION_TIME, scene ) );				// Change the scene with transition
}

