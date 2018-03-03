/*
	Joe O'Regan
	K00203642
	22/02/2018

	Controls for background music
	Child class of cocos2d::Node
	Forwards, backwards, play, and pause
*/

#include "MusicPlayer.h"
#include "Audio.h"

MusicPlayer* MusicPlayer::s_pInstance;																// MusicPlayer Singleton

bool MusicPlayer::init(cocos2d::Layer *layer) {
	//visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	/*
	// If the target platform is a mobile device (android in this case)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (visibleSize.height == 1080)
			layer->addChild(create(cocos2d::Point(250, 250)));
		else
			layer->addChild(create(cocos2d::Point(150, 150)));
	}
	*/
	return true;
}

MusicPlayer *MusicPlayer::create(cocos2d::Point position) {
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	s_pInstance = new MusicPlayer();

    if (true) {        
		s_pInstance->autorelease();																					// Set to autorelease
		
		s_pInstance->m_playImg = cocos2d::MenuItemImage::create("btnPlay.png", "btnPlaySelect.png");				// Create play menu item
		s_pInstance->m_pauseImg = cocos2d::MenuItemImage::create("btnPause.png", "btnPauseSelect.png");				// Create pause menu item
		s_pInstance->m_forwardImg = cocos2d::MenuItemImage::create("btnSkip.png", "btnSkipSelect.png");				// Create track forwards menu item (Back image in reverse)
		s_pInstance->m_backImg = cocos2d::MenuItemImage::create("btnSkip.png", "btnSkipSelect.png");				// Create track backwards menu item
		
		// Check to display pause or play
		if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
			s_pInstance->m_playImg->setVisible(false);																// Initially hide play, and reveal when pause button is pressed
		else
			s_pInstance->m_pauseImg->setVisible(false);

		// Set button positions
		s_pInstance->m_forwardImg->setPosition(cocos2d::Point(s_pInstance->m_pauseImg->getPosition().x + 
			s_pInstance->m_pauseImg->getContentSize().width + s_pInstance->m_forwardImg->getContentSize().width/2, 
			s_pInstance->m_pauseImg->getPosition().y));

		s_pInstance->m_backImg->setPosition(cocos2d::Point(s_pInstance->m_pauseImg->getPosition().x - 
			s_pInstance->m_pauseImg->getContentSize().width - s_pInstance->m_forwardImg->getContentSize().width / 2, 
			s_pInstance->m_pauseImg->getPosition().y));
		s_pInstance->m_forwardImg->setScale(-1);																	// Reuse back arrow, flipping on vertical axis

		// Display the current track information
		s_pInstance->m_currentTrackLbl = cocos2d::Label::createWithTTF(Audio::Instance()->getTrackName(), 
			"fonts/Super Mario Bros..ttf", s_pInstance->m_pauseImg->getContentSize().height * 0.4f);
		s_pInstance->m_currentTrackLbl->setPosition(cocos2d::Point(s_pInstance->m_pauseImg->getPosition().x, 
			s_pInstance->m_pauseImg->getPosition().y - (s_pInstance->m_pauseImg->getContentSize().height) * 0.75f));
		s_pInstance->m_currentTrackLbl->setTextColor(cocos2d::Color4B::RED);
		s_pInstance->m_currentTrackLbl->enableOutline(cocos2d::Color4B::WHITE, 3);

		s_pInstance->addChild(s_pInstance->m_currentTrackLbl);

		// Add the menu items
		cocos2d::Menu *menu = cocos2d::Menu::create(s_pInstance->m_playImg, 
			s_pInstance->m_pauseImg, s_pInstance->m_forwardImg, s_pInstance->m_backImg, NULL);
        menu->setPosition(cocos2d::Point(0,0));
		s_pInstance->addChild(menu, 120);
		
		s_pInstance->setScale((visibleSize.height == 1080) ? 0.775f : 0.55f);										// Make whole music player smaller, affects the screen positioning

		s_pInstance->setPosition(cocos2d::Point(position.x, position.y));
        
        return s_pInstance;
    }

    CC_SAFE_DELETE(s_pInstance);
    return NULL;
}

void MusicPlayer::update() {
	if (getButton(1)->isSelected()) {
		playTrack();										// Swap play & pause button sprites
	}
	else if (getButton(2)->isSelected()) {
		pauseTrack();										// Swap play & pause button sprites
	}
	else if (getButton(3)->isSelected()) {
		nextTrack();										// Skip track forwards
	}
	else if (getButton(4)->isSelected()) {
		previousTrack();									// Skip track backwards
	}
}

cocos2d::MenuItemImage *MusicPlayer::getButton(int button) const {
	cocos2d::MenuItemImage *result;
    switch (button) {
		case 1: result = MusicPlayer::m_playImg; break;		// play music
		case 2: result = MusicPlayer::m_pauseImg; break;	// pause music
        case 3: result = MusicPlayer::m_forwardImg; break;	// next track
        case 4: result = MusicPlayer::m_backImg; break;		// previous track
        default: break;
    }

    return result;
}

// Audio Callbacks
void MusicPlayer::playTrack() {
	Audio::Instance()->play();
	s_pInstance->m_pauseImg->setVisible(true);
	s_pInstance->m_playImg->setVisible(false);
	s_pInstance->m_currentTrackLbl->setString(Audio::Instance()->getTrackName());
}
void MusicPlayer::pauseTrack() {
	Audio::Instance()->pause();
	s_pInstance->m_pauseImg->setVisible(false);
	s_pInstance->m_playImg->setVisible(true);
	s_pInstance->m_currentTrackLbl->setString("Music Paused");
}
void MusicPlayer::nextTrack() {
	Audio::Instance()->skipTrackForwards();
	s_pInstance->m_currentTrackLbl->setString(Audio::Instance()->getTrackName());
}
void MusicPlayer::previousTrack() {
	Audio::Instance()->skipTrackBackwards();
	s_pInstance->m_currentTrackLbl->setString(Audio::Instance()->getTrackName());
}