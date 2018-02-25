/*
	AudioMenu.cpp

	Joe O'Regan
	K00203642

	Audio settings menu, inherits from MenuScene base class
*/

#include "AudioMenu.h"
#include "Audio.h"

cocos2d::Scene* AudioMenu::createScene() {
	cocos2d::Scene* scene = cocos2d::Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier
	layer = AudioMenu::create();						// 'layer' is an autorelease object, JOR replaced auto specifier
	scene->addChild(layer);								// Add layer as a child to scene

	return scene;										// Return the scene
}

// Initialise the menu base class, then add buttons and other scene content
bool AudioMenu::init() {
	MenuScene::init();																															// base class function

	titleSprite->setTexture("AudioMenu.png");																									// Change the title image text
	
	// Instructions (MarkerFelt.ttf Super Mario Bros..ttf)
	//instructionLbl = cocos2d::LabelTTF::create("Adjust Audio Volumes", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);				// Label to display current high score	
	instructionLbl = cocos2d::Label::createWithTTF("Adjust Audio Volumes", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);
	instructionLbl->setPosition(cocos2d::Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.65f + origin.y));						// Set its position on screen
	instructionLbl->setColor(cocos2d::Color3B::WHITE);																							// Set the text colour
	//instructionLbl->setScale(scale);																											// Set the scale
	this->addChild(instructionLbl);																												// Add it to the layer
	
	// Music Label
	musicLbl = cocos2d::Sprite::create("Music.png");																							// Music Vol Label
	musicLbl->setPosition(cocos2d::Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.55f + origin.y));								// Set position on screen
	musicLbl->setScale(0.8f * scale);																											// Set the scale
	this->addChild(musicLbl);																													// Add to layer	

	// Music volume down
	btnVolDownImg = cocos2d::MenuItemImage::create("btnMinus.png", "btnMinusSelect.png", CC_CALLBACK_1(AudioMenu::musicVolDown, this));			// Set image for menu option
	btnVolDownImg->setPosition(cocos2d::Point(visibleSize.width * 0.3f + origin.x, visibleSize.height * 0.55f + origin.y));						// Set image position
	btnVolDownImg->setScale(scale);																												// Set the scale

	// Music volume up
	btnVolUpImg = cocos2d::MenuItemImage::create("btnPlus.png", "btnPlusSelect.png", CC_CALLBACK_1(AudioMenu::musicVolUp, this));				// Set image for menu option
	btnVolUpImg->setPosition(cocos2d::Point(visibleSize.width * 0.7f + origin.x, visibleSize.height * 0.55f + origin.y));						// Set image position
	btnVolUpImg->setScale(scale);																												// Set the scale
	
	// FX Label
	sfxLbl = cocos2d::Sprite::create("SoundFX.png");																							// SFX Vol Label
	sfxLbl->setPosition(cocos2d::Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.45f + origin.y));								// Set position on screen
	sfxLbl->setScale(0.8f * scale);																												// Set the scale
	this->addChild(sfxLbl);																														// Add to layer	

	// FX Down
	btnFXDownImg = cocos2d::MenuItemImage::create("btnMinus.png", "btnMinusSelect.png", CC_CALLBACK_1(AudioMenu::sfxVolDown, this));			// Set image for menu option
	btnFXDownImg->setPosition(cocos2d::Point(visibleSize.width * 0.3f + origin.x, visibleSize.height * 0.45f + origin.y));						// Set image position
	btnFXDownImg->setScale(scale);																												// Set the scale

	// FX Up
	btnFXUpImg = cocos2d::MenuItemImage::create("btnPlus.png", "btnPlusSelect.png", CC_CALLBACK_1(AudioMenu::sfxVolUp, this));					// Set image for menu option
	btnFXUpImg->setPosition(cocos2d::Point(visibleSize.width * 0.7f + origin.x, visibleSize.height * 0.45f + origin.y));						// Set image positionscreen
	btnFXUpImg->setScale(scale);																												// Set the scale
	
	//instructionLbl = cocos2d::LabelTTF::create("Select Music Track", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);				// Label to display current high score	
	instructionLbl = cocos2d::Label::createWithTTF("Select Music Track", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);
	instructionLbl->setPosition(cocos2d::Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.35f + origin.y));						// Set its position on screen
	instructionLbl->setColor(cocos2d::Color3B::WHITE);																							// Set the text colour
	//instructionLbl->setScale(scale);																											// Set the scale
	this->addChild(instructionLbl);																												// Add it to the layer
	
	// Track Label
	trackLbl = cocos2d::Sprite::create("Track.png");																							// SFX Vol Label
	trackLbl->setPosition(cocos2d::Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.25f + origin.y));								// Set position on screen
	trackLbl->setScale(0.8f * scale);																											// Set the scale
	this->addChild(trackLbl);																													// Add to layer	

	// Track Backwards
	btnTrackDownImg = cocos2d::MenuItemImage::create("btnMinus.png", "btnMinusSelect.png", CC_CALLBACK_1(AudioMenu::skipTrackBackwards, this));	// Set image for menu option
	btnTrackDownImg->setPosition(cocos2d::Point(visibleSize.width * 0.3f + origin.x, visibleSize.height * 0.25f + origin.y));					// Set image position
	//btnTrackDownImg->setScale(scale);																											// Set the scale

	// Track Forwards
	btnTrackUpImg = cocos2d::MenuItemImage::create("btnPlus.png", "btnPlusSelect.png", CC_CALLBACK_1(AudioMenu::skipTrackForwards, this));		// Set image for menu option
	btnTrackUpImg->setPosition(cocos2d::Point(visibleSize.width * 0.7f + origin.x, visibleSize.height * 0.25f + origin.y));						// Set image position
	//btnTrackUpImg->setScale(scale);																											// Set the scale
	
	// Current Track
	//currentTrackLbl = cocos2d::LabelTTF::create(Audio::Instance()->getTrackName(), "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);	// Label to display current high score	
	currentTrackLbl = cocos2d::Label::createWithTTF(Audio::Instance()->getTrackName(), "fonts/Super Mario Bros..ttf", visibleSize.height * 0.045f);
	currentTrackLbl->setPosition(cocos2d::Point(visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.15f + origin.y));					// Set its position on screen
	currentTrackLbl->setColor(cocos2d::Color3B::WHITE);																							// Set the text colour
	//currentTrackLbl->setScale(scale);																											// Set the scale
	this->addChild(currentTrackLbl);																											// Add it to the layer

	menu = cocos2d::Menu::create(btnVolDownImg, btnVolUpImg, btnFXUpImg, btnFXDownImg, btnTrackDownImg, btnTrackUpImg, NULL);					// Menu
	menu->setPosition(cocos2d::Point::ZERO);																									// Set position on screen
	this->addChild(menu);

	return true;
}


// Callbacks
void AudioMenu::musicVolUp(cocos2d::Ref *sender) {
	Audio::Instance()->musicVolIncrease();
}
void AudioMenu::musicVolDown(cocos2d::Ref *sender) {
	Audio::Instance()->musicVolDecrease();
}
void AudioMenu::sfxVolUp(cocos2d::Ref *sender) {
	Audio::Instance()->sfxVolIncrease();
}
void AudioMenu::sfxVolDown(cocos2d::Ref *sender) {
	Audio::Instance()->sfxVolDecrease();
}
void AudioMenu::skipTrackForwards(cocos2d::Ref *sender) {
	Audio::Instance()->skipTrackForwards();
	currentTrackLbl->setString(Audio::Instance()->getTrackName());
}
void AudioMenu::skipTrackBackwards(cocos2d::Ref *sender) {
	Audio::Instance()->skipTrackBackwards();
	currentTrackLbl->setString(Audio::Instance()->getTrackName());
}