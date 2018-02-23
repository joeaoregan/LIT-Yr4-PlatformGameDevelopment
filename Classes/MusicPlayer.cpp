/*
	Joe O'Regan
	K00203642
	22/02/2018

	Controls for background music
*/

#include "MusicPlayer.h"
#include "Audio.h"

MusicPlayer* MusicPlayer::s_pInstance;	// MusicPlayer Singleton

bool MusicPlayer::init(cocos2d::Layer *layer) {
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// If the target platform is a mobile device (android in this case)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (visibleSize.height == 1080)
			layer->addChild(create(Point(250, 250)));
		else
			layer->addChild(create(Point(150, 150)));
	}
	
	return true;
}

MusicPlayer *MusicPlayer::create(Point position){
	s_pInstance = new MusicPlayer();

    if (true) {        
		s_pInstance->autorelease();															// Set to autorelease
		
		s_pInstance->play = MenuItemImage::create("btnPlay.png", "btnPlaySelect.png");		// Create play menu item
		s_pInstance->pause = MenuItemImage::create("btnPause.png", "btnPauseSelect.png");	// Create pause menu item
		s_pInstance->forward = MenuItemImage::create("btnSkip.png", "btnSkipSelect.png");	// Create track forwards menu item
		s_pInstance->back = MenuItemImage::create("btnSkip.png", "btnSkipSelect.png");		// Create track backwards menu item

		// Set positions and rotations
		s_pInstance->play->setPosition(Point(position.x, position.y));
		s_pInstance->pause->setPosition(Point(position.x, position.y));
		s_pInstance->play->setVisible(false);												// Initially hide play, and reveal when pause button is pressed

		// Set button positions
		s_pInstance->forward->setPosition(Point(position.x + s_pInstance->pause->getContentSize().width + s_pInstance->forward->getContentSize().width/2, position.y));
		s_pInstance->back->setPosition(Point(position.x - s_pInstance->pause->getContentSize().width - s_pInstance->back->getContentSize().width/2, position.y));
		s_pInstance->forward->setScale(-1);

		s_pInstance->currentTrackLbl = LabelTTF::create(Audio::Instance()->getTrackName(), "fonts/Super Mario Bros..ttf", s_pInstance->pause->getContentSize().height * 0.25f);
		s_pInstance->currentTrackLbl->setPosition(Point(position.x, position.y - (s_pInstance->pause->getContentSize().height) * 0.75f));
		s_pInstance->currentTrackLbl->setColor(Color3B::WHITE);
		s_pInstance->addChild(s_pInstance->currentTrackLbl);
		//s_pInstance->addChild(currentTrackLbl, 100);

        Menu *menu = Menu::create(s_pInstance->play, s_pInstance->pause, s_pInstance->forward, s_pInstance->back, NULL);
        menu->setPosition(Point(0,0));
		s_pInstance->addChild(menu, 120);
		s_pInstance->setScale(0.75);
        
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

MenuItemImage *MusicPlayer::getButton(int button){
    MenuItemImage *result;
    switch (button) {
		case 1: result = MusicPlayer::play; break;		// play music
		case 2: result = MusicPlayer::pause; break;		// pause music
        case 3: result = MusicPlayer::forward; break;	// next track
        case 4: result = MusicPlayer::back; break;		// previous track
        default: break;
    }

    return result;
}

void MusicPlayer::playTrack() {
	Audio::Instance()->play();
	s_pInstance->pause->setVisible(true);
	s_pInstance->play->setVisible(false);
}
void MusicPlayer::pauseTrack() {
	Audio::Instance()->pause();
	s_pInstance->pause->setVisible(false);
	s_pInstance->play->setVisible(true);
}
void MusicPlayer::nextTrack() {
	Audio::Instance()->skipTrackForwards();
	s_pInstance->currentTrackLbl->setString(Audio::Instance()->getTrackName());
}
void MusicPlayer::previousTrack() {
	Audio::Instance()->skipTrackBackwards();
	s_pInstance->currentTrackLbl->setString(Audio::Instance()->getTrackName());
}