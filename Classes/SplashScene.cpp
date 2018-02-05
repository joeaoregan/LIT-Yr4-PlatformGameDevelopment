#include "SplashScene.h"
#include "GameScene.h"

USING_NS_CC;

#define DISPLAY_TIME_SPLASH_SCENE 2
#define TRANSITION_TIME 0.5

Scene* SplashScene::createScene() {    
    auto scene = Scene::create();		// 'scene' is an autorelease object
        
    auto layer = SplashScene::create();	// 'layer' is an autorelease object
	    
    scene->addChild(layer);				// add layer as a child to scene
	    
    return scene;						// return the scene
}

// on "init" you need to initialize your instance
bool SplashScene::init() {
    // 1. super init first
    if ( !Layer::init() ) { return false; }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->scheduleOnce( schedule_selector( SplashScene::GoToMainMenuScene ), DISPLAY_TIME_SPLASH_SCENE );

    auto backgroundSprite = Sprite::create( "SplashScreenBG.png" );													// Create the background sprite
    backgroundSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
    
    this->addChild( backgroundSprite );																				// Add background sprite as a child of the layer
    
    return true;
}

void SplashScene::GoToMainMenuScene( float dt ) {
    auto scene = GameScene::createScene();																			// Go to Hello World scene
    
    Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );						// Change the scene
}

