/*
	EnterName.cpp

	Joe O'Regan
	K00203642

	Enter the players name
*/

#include "Game.h"
#include "Settings.h"
#include "MainMenu.h"
#include "Audio.h"

#define TRANSITION_TIME 0.5

//std::string trackNames[] = { "Track 1: A song by Joe", "Track 2: Another song by Joe", "Track 3: Mostly Joe, and a guitar solo by Jimmy" };

Scene* Settings::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	Settings* layer = Settings::create();		// 'layer' is an autorelease object, JOR replaced auto specifier
		
	scene->addChild(layer);						// Add layer as a child to scene
		
	return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool Settings::init() {
	if (!Layer::init()) return false;																								// Super init first

	visibleSize = Director::getInstance()->getVisibleSize();																		// Get visible screen size
	origin = Director::getInstance()->getVisibleOrigin();																			// Get screen origin point

	// Set scale depending on screen resolution
	float scale = 0.67f;
	if (visibleSize.height == 1080)
		scale = 1.0f;

	// Initialise the background sprite image
	initBackground();																												// Decide which background image to use, based on the platforms screen resolution
	this->addChild(backgroundSprite);																								// Add to layer

	// Game Title
	gameTitleSprite = Sprite::create("GameTitle.png");																				// Game Title image
	gameTitleSprite->setPosition(Point(visibleSize.width * 0.1f, visibleSize.height / 2 + origin.y));								// Set position on screen
	gameTitleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.9f + origin.y));					// Set position on screen
	gameTitleSprite->setScale(1.75 * scale);
	this->addChild(gameTitleSprite);																								// Add to layer		

	// High Scores Title
	titleSprite = Sprite::create("Settings.png");																					// Title image
	//titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - titleSprite->getContentSize().height));	// Set position on screen
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75f + origin.y));						// Set position on screen
	titleSprite->setScale(scale);																									// Set the scale
	this->addChild(titleSprite);																									// Add to layer							
		
	// Instructions
	//MarkerFelt.ttf Super Mario Bros..ttf 
	//instructionLbl = LabelTTF::create("Adjust Audio Volumes", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);			// Label to display current high score	
	instructionLbl = cocos2d::Label::createWithTTF("Adjust Audio Volumes", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);// Label to display current high score

	instructionLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.65f + origin.y));					// Set its position on screen
	instructionLbl->setColor(Color3B::WHITE);																						// Set the text colour
	//instructionLbl->setScale(scale);																								// Set the scale
	this->addChild(instructionLbl);																									// Add it to the layer

	// Music Label
	musicLbl = Sprite::create("Music.png");																							// Music Vol Label
	musicLbl->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.55f + origin.y));							// Set position on screen
	musicLbl->setScale(0.8f * scale);																								// Set the scale
	this->addChild(musicLbl);																										// Add to layer	

	// Music volume down
	btnVolDownImg = MenuItemImage::create("btnMinus.png", "btnMinusSelect.png", CC_CALLBACK_1(Settings::musicVolDown, this));		// Set image for menu option
	btnVolDownImg->setPosition(Point(visibleSize.width * 0.3f + origin.x, visibleSize.height * 0.55f + origin.y));					// Set image position
	btnVolDownImg->setScale(scale);																									// Set the scale
	btnVolDown = Menu::create(btnVolDownImg, NULL);																					// Menu
	btnVolDown->setPosition(Point::ZERO);																							// Set position on screen
	this->addChild(btnVolDown);																										// Add to layer

	// Music volume up
	btnVolUpImg = MenuItemImage::create("btnPlus.png", "btnPlusSelect.png", CC_CALLBACK_1(Settings::musicVolUp, this));				// Set image for menu option
	btnVolUpImg->setPosition(Point(visibleSize.width * 0.7f + origin.x, visibleSize.height * 0.55f + origin.y));					// Set image position
	btnVolUpImg->setScale(scale);																									// Set the scale
	btnVolUp = Menu::create(btnVolUpImg, NULL);																						// Menu
	btnVolUp->setPosition(Point::ZERO);																								// Set position on screen
	this->addChild(btnVolUp);

	// FX Label
	sfxLbl = Sprite::create("SoundFX.png");																							// SFX Vol Label
	sfxLbl->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.45f + origin.y));							// Set position on screen
	sfxLbl->setScale(0.8f * scale);																									// Set the scale
	this->addChild(sfxLbl);																											// Add to layer	

	// FX Down
	btnVolDownImg = MenuItemImage::create("btnMinus.png", "btnMinusSelect.png", CC_CALLBACK_1(Settings::sfxVolDown, this));			// Set image for menu option
	btnVolDownImg->setPosition(Point(visibleSize.width * 0.3f + origin.x, visibleSize.height * 0.45f + origin.y));					// Set image position
	btnVolDownImg->setScale(scale);																									// Set the scale
	btnFXDown = Menu::create(btnVolDownImg, NULL);																					// Menu
	btnFXDown->setPosition(Point::ZERO);																							// Set position on screen
	this->addChild(btnFXDown);																										// Add to layer

	// FX Up
	btnFXUpImg = MenuItemImage::create("btnPlus.png", "btnPlusSelect.png", CC_CALLBACK_1(Settings::sfxVolUp, this));				// Set image for menu option
	btnFXUpImg->setPosition(Point(visibleSize.width * 0.7f + origin.x, visibleSize.height * 0.45f + origin.y));						// Set image positionscreen
	btnFXUpImg->setScale(scale);																									// Set the scale
	btnFXUp = Menu::create(btnFXUpImg, NULL);																						// Menu
	btnFXUp->setPosition(Point::ZERO);																								// Set position on 
	this->addChild(btnFXUp);
	
	//instructionLbl = LabelTTF::create("Select Music Track", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);			// Select track label
	instructionLbl = cocos2d::Label::createWithTTF("Select Music Track", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);// Select track label
	instructionLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.35f + origin.y));					// Set its position on screen
	instructionLbl->setColor(Color3B::WHITE);																						// Set the text colour
	//instructionLbl->setScale(scale);																								// Set the scale
	this->addChild(instructionLbl);																									// Add it to the layer

	// Track Label
	trackLbl = Sprite::create("Track.png");																							// SFX Vol Label
	trackLbl->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.25f + origin.y));							// Set position on screen
	trackLbl->setScale(0.8f * scale);																								// Set the scale
	this->addChild(trackLbl);																										// Add to layer	

	// Track Backwards
	btnTrackDownImg = MenuItemImage::create("btnMinus.png", "btnMinusSelect.png", CC_CALLBACK_1(Settings::skipTrackBackwards, this));// Set image for menu option
	btnTrackDownImg->setPosition(Point(visibleSize.width * 0.3f + origin.x, visibleSize.height * 0.25f + origin.y));				// Set image position
	btnTrackDownImg->setScale(scale);																								// Set the scale
	btnTrackDown = Menu::create(btnTrackDownImg, NULL);																				// Menu
	btnTrackDown->setPosition(Point::ZERO);																							// Set position on screen
	this->addChild(btnTrackDown);																									// Add to layer

	// Track Forwards
	btnTrackUpImg = MenuItemImage::create("btnPlus.png", "btnPlusSelect.png", CC_CALLBACK_1(Settings::skipTrackForwards, this));		// Set image for menu option
	btnTrackUpImg->setPosition(Point(visibleSize.width * 0.7f + origin.x, visibleSize.height * 0.25f + origin.y));						// Set image position
	btnTrackUpImg->setScale(scale);																										// Set the scale
	btnTrackUp = Menu::create(btnTrackUpImg, NULL);																						// Menu
	btnTrackUp->setPosition(Point::ZERO);																								// Set position on screen
	this->addChild(btnTrackUp);
	

	// Current Track
	//currentTrackLbl = LabelTTF::create(Audio::Instance()->getTrackName(), "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);	// Show the current tracks name
	currentTrackLbl = cocos2d::Label::createWithTTF(Audio::Instance()->getTrackName(), "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);// Show the current tracks name
	currentTrackLbl->setPosition(Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.15f + origin.y));						// Set its position on screen
	currentTrackLbl->setColor(Color3B::WHITE);																							// Set the text colour
	//currentTrackLbl->setScale(scale);																									// Set the scale
	this->addChild(currentTrackLbl);																									// Add it to the layer

	// Back Button
	btnBackImg = MenuItemImage::create("btnBack.png", "btnBackSelect.png", CC_CALLBACK_1(Settings::returnToMenu, this));				// Set image for menu option
	btnBackImg->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.05f + origin.y));							// Set image position
	btnBackImg->setScale(scale);																										// Set the scale
	btnBack = Menu::create(btnBackImg, NULL);																							// Menu
	btnBack->setPosition(Point::ZERO);																									// Set position on screen
	this->addChild(btnBack);																											// Add to layer																							// Add to layer
	
	return true;
}

// Callbacks
void Settings::returnToMenu(cocos2d::Ref *sender) {
	cocos2d::Scene* scene = MainMenu::createScene();																					// Return to the main menu

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));												// Switch scenes with animated transition
}
void Settings::musicVolUp(cocos2d::Ref *sender) {
	Audio::Instance()->musicVolIncrease();
}
void Settings::musicVolDown(cocos2d::Ref *sender) {
	Audio::Instance()->musicVolDecrease();
}
void Settings::sfxVolUp(cocos2d::Ref *sender) {
	Audio::Instance()->sfxVolIncrease();
}
void Settings::sfxVolDown(cocos2d::Ref *sender) {
	Audio::Instance()->sfxVolDecrease();
}
void Settings::skipTrackForwards(cocos2d::Ref *sender) {
	//trackSelect++;
	//if (trackSelect == 3) trackSelect = 0;
	//(trackSelect == 2) ? trackSelect = 0 : trackSelect++;
	//currentTrackLbl->setString(trackNames[trackSelect]);

	Audio::Instance()->skipTrackForwards();
	currentTrackLbl->setString(Audio::Instance()->getTrackName());
}
void Settings::skipTrackBackwards(cocos2d::Ref *sender) {
	//if (trackSelect > 0) trackSelect--;
	//else if (trackSelect == 0) trackSelect = 3;
	//(trackSelect == 0) ? trackSelect = 2 : trackSelect--;	

	Audio::Instance()->skipTrackBackwards();
	currentTrackLbl->setString(Audio::Instance()->getTrackName());
}

// Choose a background based on the screen resolution
void Settings::initBackground() {
	// Decide which background image to use, based on the platforms screen resolution
	//if (visibleSize.height == 1080)
	//	backgroundSprite = Sprite::create("BG1080p.png");
	//else
	//	backgroundSprite = Sprite::create("BG720p.png");																			// Background image

	backgroundSprite = Sprite::create((visibleSize.height == 1080) ? "BG1080p.png" : "BG720p.png");									// Decide which background image to use, based on the platforms screen resolution
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));						// Set backgound position
}