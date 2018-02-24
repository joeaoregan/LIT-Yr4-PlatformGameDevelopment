/*
	Level1.cpp

	Joe O'Regan
	K00203642
	02/02/2018

	The first level of the game

	20180202	Added Audio class with singleton access
*/
#include "Level1.h"
#include "Level2.h"
#include "MainMenu.h"
#include "DPad.h"
#include <string>
#include <sstream>
#include "Input.h"


Scene* Level1::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier   
	layerInstance = Level1::create();			// 'layer' is an autorelease object, JOR replaced auto specifier  
	//Level1->setName("Level1");				// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(layerInstance);				// Add layer as a child to scene	    
    return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool Level1::init() {
	Level::init();																						// 20180221 Added Level base class

	if ( !Layer::init() ) { return false; }																// super init first

	Game::Instance()->setLevel(1);																		// for parallax node init

	Game::Instance()->setLives(3);																		// JOR set the number of lives for the player
	
	// 1) Create the ParallaxNode
	//_backgroundNode = ParallaxNodeExtras::create();
	this->addChild(_backgroundNode, -1);
	_backgroundNode->init();																			// ParallaxNodeExtras.cpp: Initialise the parallax scrolling background
	
	/*
	//controller->init(this);																									// NOT WORKING ANDROID: D-pad (Display on mobile device)
	//DPad::Instance()->init(this);																								// NOT WORKING ANDROID: D-pad (Display on mobile device)
	controller = DPad::Instance()->create("DPad/Base300.png", "DPad/Arrow96.png", "DPad/Arrow96Pressed.png", Point(250, 250));
	this->addChild(controller);
	*/
	
	this->scheduleUpdate();
	
    return true;
}

void Level1::update(float dt) {
	Level::update(dt);																						// Call base class update function
	
	//HUD::Instance()->update();																			// Update the score (Not working)
	
	//time -= ((int) dt % 10);
	//time += dt;
	//this->schedule(schedule_selector(Level1::updateTimer), 1.0f);											// Call the function every 1 second
	//if (curTimeMillis > timerTime) {
	//	timerTime = curTimeMillis + 1000;
	//	time--;
	//}
}

void Level1::checkCollisions() {
	Level::checkCollisions();																				// Call base class function
																											// Check collisions with different objects in different levels
}

/*
void Level1::createStatusBar(int x, int y, int w, int h, float percent, Color4F FGColor, Color4F BGColor, int orientation, int startFrom) {	// Horizontal healthbar 2017/01/20 added if statement to decide orientation of health bar

	percent = (percent > 1.0f) ? 1.0f : (percent < 0.f) ? 0.f : percent;		// If greater than 1 set to 1, if minus set to 0

	cocos2d::DrawNode* rectNode = DrawNode::create();
	Color4F white(1, 1, 1, 1);
	Vec2 foregroundRect[4];
	Vec2 backgroundRect[4];

	foregroundRect[0] = Vec2(-w/2, -h/2);
	foregroundRect[1] = Vec2(w/2, -h/2);
	foregroundRect[2] = Vec2(w/2, h/2);
	foregroundRect[3] = Vec2(-w/2, h/2);

	backgroundRect[0] = Vec2(-w / 2, -h / 2);
	backgroundRect[1] = Vec2(w / 2 * percent, -h / 2);
	backgroundRect[2] = Vec2(w / 2 * percent, h / 2);
	backgroundRect[3] = Vec2(-w, h / 2);

	rectNode->drawPolygon(backgroundRect, 4, BGColor, 2, white);
	rectNode->drawPolygon(foregroundRect, 4, BGColor, 2, white);
	rectNode->setPosition(Point(x, y));


	//SDL_Rect foregroundRect;													// The rectangle to
	//Color4F old;

//	SDL_GetRenderDrawColor(Game::Instance()->getRenderer(), &old.r, &old.g, &old.g, &old.a);
//	SDL_Rect backgroundRect = { x, y, w, h };									// Background rectangle with coordinates and dimensions entered
//	SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a);	// Draw a Solid Rectangle with the RGB colour values assigned
//	SDL_RenderFillRect(Game::Instance()->getRenderer(), &backgroundRect);
//	SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);
	
	// Decide if Healthbar is horizontal or vertical
	if (orientation == HORIZONTAL) {
		int newW = (int)((float)w * Percent);									// new width
		int newX = x + (w - newW);												// new x coord
		if (startFrom == START_LEFT) foregroundRect = { newX, y, newW, h };		// Decrease from left
		else if (startFrom == START_RIGHT) foregroundRect = { x, y, newW, h };	// 2017/02/17 Decrease from right
	}
	else if (orientation == VERTICAL) {
		int ph = (int)((float)h * Percent);
		int py = y + (h - ph);
		foregroundRect = { x, py, w, ph };
	}
	
	//SDL_RenderFillRect(Game::Instance()->getRenderer(), &foregroundRect);
	//SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), old.r, old.g, old.b, old.a);
	
//}
*/
