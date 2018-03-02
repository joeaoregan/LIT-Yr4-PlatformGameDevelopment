/*
	Defines.h
	
	Joe O'Regan
	K00203642
	02/03/2018

	Game Definitions - speed up testing
*/
#ifndef  _DEFINES_H_
#define  _DEFINES_H_

#include "cocos2d.h"

// Level length
#define LEVEL_TIME_EASY 25000.0f
#define LEVEL_TIME_MED 20000.0f
#define LEVEL_TIME_HARD 40000.0f

// Amount of each type of enemy to add to enemy vector list
#define L2_NUM_ENEMY_2 3

#define L3_NUM_ENEMY_1 1
#define L3_NUM_ENEMY_2 2
#define L3_NUM_ENEMY_3 2														// Two of the larger ship on screen at once


/* Images */
#define POWER_UP_LIFE_IMG "powerHeart.png"
#define POWER_UP_WEAPON_IMG "WeaponPowerUp.png"

// Dpad
#define DPAD_BASE "BaseCircle320b.png"
#define DPAD_ARROW "Arrow160b.png"
#define DPAD_ARROW_ACTIVE "Arrow160bPressed.png"

// Background
#define ASTEROID_IMG "asteroid.png"

// Weapoons
#define LASER_BLUE_IMG "laserbeam_blue.png"
#define LASER_ORANGE_IMG "LaserOrange.png"
#define LASER_GREEN_IMG "LaserGreen.png"


// Enemies and obstacles
#define NUM_ENEMIES 3
#define MAX_NUM_ASTEROIDS_L1 10								// Number of asteroids
#define NUM_ASTEROIDS_L2 15									// Number of asteroids
#define NUM_LASERS 8										// Number of lasers
#define ENEMY_FIRE_RATE 500
#define MAX_ENEMY_SHIP_LIVES 3.0f

// Player
#define MAX_PLAYER_LIVES 5									// The maximum number of lives a player can have
#define PLAYER_SPEED 3.0f

// Screen Resolution
#define RES_720P 720
#define RES_1080P 1080

// Menus
#define TRANSITION_TIME 0.5


enum difficulty { EASY, MEDIUM, HARD };
enum laserTypes { BLUE, ORANGE, GREEN1, GREEN2, GREEN3 };
enum asteroidType { STATIC_ROID, DYNAMIC_ROID };
enum powerUpTypes { NEW_LIFE, WEAPON_UPGRADE };

//static unsigned int NUM_LASERS_TO_FIRE = 2;	// Moved to player class

#endif // _DEFINES_H_

