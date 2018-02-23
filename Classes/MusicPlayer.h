/*
	MusicPlayer.h

	Joe O'Regan
	K00203642
	22/02/2018

	Controls for background music
*/

#ifndef __M_PLAYER_H
#define __M_PLAYER_H

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class MusicPlayer: public Node{

public:
	// MusicPlayer singleton
	static MusicPlayer* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new MusicPlayer();
			return s_pInstance;
		}
		return s_pInstance;
	}

	static MusicPlayer *create(Point position);

	MusicPlayer() {};							// Constructor	
	~MusicPlayer() {};							// Destructor

	bool init(cocos2d::Layer *layer);			// Initialise the music player
	void update();								// update the music player

	void playTrack();							// Play the music
	void pauseTrack();							// Pause the music
	void nextTrack();							// Skip to the next track
	void previousTrack();						// Skip to previous track

    MenuItemImage *getButton(int button);		// Get the active button
	
private:
    MenuItemImage *play;						// Play music button
    MenuItemImage *pause;						// Pause music button
    MenuItemImage *forward;						// Skip track forwards button
    MenuItemImage *back;						// Skip track backwards button
	cocos2d::LabelTTF* currentTrackLbl;			// Label to display current background music track
	
	static MusicPlayer* s_pInstance;			// Single instance of Game used as singleton, so only one instance exists thoughout the game
};
#endif /* __M_PLAYER_H */
