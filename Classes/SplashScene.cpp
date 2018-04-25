/*
	SplashScene.cpp

	Joe O'Regan
	K00203642

	Displays animated splash screen
	With scaling background image, and fading
*/

#include "SplashScene.h"
#include "MainMenu.h"
#include "Audio.h"
#include "Defines.h"
//#include "XML.h"

cocos2d::Scene* SplashScene::createScene() {
    cocos2d::Scene* scene = cocos2d::Scene::create();		// Create the scene. 'scene' is an autorelease object
        
    SplashScene* layer = SplashScene::create();				// Create the layer. 'layer' is an autorelease object
	    
    scene->addChild(layer);									// Add layer as a child to scene
	    
    return scene;											// Return the scene
}

/*
	Initialise the splash scene, animating and fading the background image
*/
bool SplashScene::init() {
	//XML blah;
	//blah.parseXML();

    // 1. super init first
    if ( !Layer::init() ) { return false; }
	
	Audio::Instance()->init();																								// Initialise the game audio
    
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();											// Get screen resolution
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();											// Get screen origin

	float scale = (visibleSize.height == 1080) ? 1.0f : (visibleSize.height == 720) ? 0.67f : visibleSize.height / 1080;	// Kindle resolution is different

    this->scheduleOnce( schedule_selector( SplashScene::GoToMainMenuScene ), DISPLAY_TIME_SPLASH_SCENE );

    cocos2d::Sprite* backgroundSprite = cocos2d::Sprite::create( "SplashScreenBG.png" );									// Create the background sprite
    backgroundSprite->setPosition(cocos2d::Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );	// Set the background position    
	backgroundSprite->setScale(scale);																						// Scale down the image size for lower resolution
    this->addChild( backgroundSprite );																						// Add background sprite as a child of the layer


	//backgroundSprite->runAction(cocos2d::ScaleTo::create(0.5F, (visibleSize.height == 1080) ? 1.5f : 1.0f));				// Not working Android
	backgroundSprite->runAction(cocos2d::ScaleTo::create(0.5f, scale * 1.5f));												// Set to grow from half to 1.5 times the size

	auto action = cocos2d::FadeOut::create(3.0f);
	backgroundSprite->runAction(action);


    return true;
}

// Callbacks

/*
	Go to main menu when time has passed
*/
void SplashScene::GoToMainMenuScene( float dt ) {
    cocos2d::Scene* scene = MainMenu::createScene();																		// Create the menu scene    
	cocos2d::Director::getInstance( )->replaceScene(cocos2d::TransitionFade::create( TRANSITION_TIME, scene ) );			// Change the scene with transition
}

