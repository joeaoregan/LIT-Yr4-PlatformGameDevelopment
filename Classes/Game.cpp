/*
	Game.cpp

	Joe O'Regan
	K00203642

	18/02/2018

	Stuff common to all levels of the game
*/

#include "Game.h"

Game* Game::s_pInstance;	// Game Singleton

bool Game::init() {
	score = 0;				// Initialise the score value
	level = 1;				// Initialise the level value
	
	return true;
}