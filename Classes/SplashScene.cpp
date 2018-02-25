#include "SplashScene.h"
//#include "GameScene.h"
#include "MainMenu.h"
#include "Audio.h"

USING_NS_CC;

#define DISPLAY_TIME_SPLASH_SCENE 2
#define TRANSITION_TIME 0.5

Scene* SplashScene::createScene() {    
    cocos2d::Scene* scene = Scene::create();		// Create the scene. 'scene' is an autorelease object
        
    SplashScene* layer = SplashScene::create();		// Create the layer. 'layer' is an autorelease object
	    
    scene->addChild(layer);							// Add layer as a child to scene
	    
    return scene;									// Return the scene
}

// on "init" you need to initialize your instance
bool SplashScene::init() {
    // 1. super init first
    if ( !Layer::init() ) { return false; }
	
	Audio::Instance()->init();																						// Initialise the game audio
    
    Size visibleSize = Director::getInstance()->getVisibleSize();													// Get screen resolution
    Vec2 origin = Director::getInstance()->getVisibleOrigin();														// Get screen origin
    
    this->scheduleOnce( schedule_selector( SplashScene::GoToMainMenuScene ), DISPLAY_TIME_SPLASH_SCENE );

    cocos2d::Sprite* backgroundSprite = Sprite::create( "SplashScreenBG.png" );										// Create the background sprite
    backgroundSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );	// Set the background position    
	backgroundSprite->setScale((visibleSize.height == 720) ? 0.67f : 1.0f);											// Scale down the image size for lower resolution
    this->addChild( backgroundSprite );																				// Add background sprite as a child of the layer

	backgroundSprite->runAction(ScaleTo::create(0.5F, 1.0f));														// Not working Android
    return true;
}

void SplashScene::GoToMainMenuScene( float dt ) {
    cocos2d::Scene* scene = MainMenu::createScene();																// Create the menu scene    
    Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );						// Change the scene with transition
}

