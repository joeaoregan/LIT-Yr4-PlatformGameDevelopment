/*
	Joe O'Regan
	K00203642
	22/02/2018

	Controls for background music
	Child class of cocos2d::Node
*/

#include "MusicPlayer.h"
#include "Audio.h"

MusicPlayer* MusicPlayer::s_pInstance;																// MusicPlayer Singleton

bool MusicPlayer::init(cocos2d::Layer *layer) {
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	// If the target platform is a mobile device (android in this case)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (visibleSize.height == 1080)
			layer->addChild(create(cocos2d::Point(250, 250)));
		else
			layer->addChild(create(cocos2d::Point(150, 150)));
	}
	
	return true;
}

MusicPlayer *MusicPlayer::create(cocos2d::Point position) {
	s_pInstance = new MusicPlayer();

    if (true) {        
		s_pInstance->autorelease();																	// Set to autorelease
		
		s_pInstance->play = cocos2d::MenuItemImage::create("btnPlay.png", "btnPlaySelect.png");		// Create play menu item
		s_pInstance->pause = cocos2d::MenuItemImage::create("btnPause.png", "btnPauseSelect.png");	// Create pause menu item
		s_pInstance->forward = cocos2d::MenuItemImage::create("btnSkip.png", "btnSkipSelect.png");	// Create track forwards menu item
		s_pInstance->back = cocos2d::MenuItemImage::create("btnSkip.png", "btnSkipSelect.png");		// Create track backwards menu item

		// Set positions and rotations
		s_pInstance->play->setPosition(cocos2d::Point(position.x, position.y));
		s_pInstance->pause->setPosition(cocos2d::Point(position.x, position.y));

		// Check to display pause or play
		if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
			s_pInstance->play->setVisible(false);													// Initially hide play, and reveal when pause button is pressed
		else
			s_pInstance->pause->setVisible(false);

		// Set button positions
		s_pInstance->forward->setPosition(cocos2d::Point(position.x + s_pInstance->pause->getContentSize().width + s_pInstance->forward->getContentSize().width/2, position.y));
		s_pInstance->back->setPosition(cocos2d::Point(position.x - s_pInstance->pause->getContentSize().width - s_pInstance->back->getContentSize().width/2, position.y));
		s_pInstance->forward->setScale(-1);

		// Display the current track information
		s_pInstance->currentTrackLbl = cocos2d::LabelTTF::create(Audio::Instance()->getTrackName(), "fonts/Super Mario Bros..ttf", s_pInstance->pause->getContentSize().height * 0.4f);
		s_pInstance->currentTrackLbl->setPosition(cocos2d::Point(position.x, position.y - (s_pInstance->pause->getContentSize().height) * 0.75f));
		s_pInstance->currentTrackLbl->setColor(cocos2d::Color3B::WHITE);
		s_pInstance->addChild(s_pInstance->currentTrackLbl);
		//s_pInstance->addChild(currentTrackLbl, 100);

		cocos2d::Menu *menu = cocos2d::Menu::create(s_pInstance->play, s_pInstance->pause, s_pInstance->forward, s_pInstance->back, NULL);
        menu->setPosition(cocos2d::Point(0,0));
		s_pInstance->addChild(menu, 120);
		s_pInstance->setScale(0.75);					// Make whole music player smaller, affects the screen positioning
        
        return s_pInstance;
    }

    CC_SAFE_DELETE(s_pInstance);
    return NULL;
}

void MusicPlayer::update() {
	if (getButton(1)->isSelected()) {
		playTrack();									// Swap play & pause button sprites
	}
	else if (getButton(2)->isSelected()) {
		pauseTrack();									// Swap play & pause button sprites
	}
	else if (getButton(3)->isSelected()) {
		nextTrack();									// Skip track forwards
	}
	else if (getButton(4)->isSelected()) {
		previousTrack();								// Skip track backwards
	}
}

cocos2d::MenuItemImage *MusicPlayer::getButton(int button){
	cocos2d::MenuItemImage *result;
    switch (button) {
		case 1: result = MusicPlayer::play; break;		// play music
		case 2: result = MusicPlayer::pause; break;		// pause music
        case 3: result = MusicPlayer::forward; break;	// next track
        case 4: result = MusicPlayer::back; break;		// previous track
        default: break;
    }

    return result;
}

// Callbacks
void MusicPlayer::playTrack() {
	Audio::Instance()->play();
	s_pInstance->pause->setVisible(true);
	s_pInstance->play->setVisible(false);
	s_pInstance->currentTrackLbl->setString(Audio::Instance()->getTrackName());
}
void MusicPlayer::pauseTrack() {
	Audio::Instance()->pause();
	s_pInstance->pause->setVisible(false);
	s_pInstance->play->setVisible(true);
	s_pInstance->currentTrackLbl->setString("Music Paused");
}
void MusicPlayer::nextTrack() {
	Audio::Instance()->skipTrackForwards();
	s_pInstance->currentTrackLbl->setString(Audio::Instance()->getTrackName());
}
void MusicPlayer::previousTrack() {
	Audio::Instance()->skipTrackBackwards();
	s_pInstance->currentTrackLbl->setString(Audio::Instance()->getTrackName());
}