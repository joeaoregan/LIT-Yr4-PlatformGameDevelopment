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

#define MPLAYER_PRESS 250.0f								// Time between music player button presses

// Level length
#define MAX_LEVELS 4

#define LEVEL_TIME_EASY 25000.0f
#define LEVEL_TIME_MED 30000.0f
#define LEVEL_TIME_HARD 40000.0f

// Amount of each type of enemy to add to enemy vector list
#define L2_NUM_ENEMY_1 2
#define L2_NUM_ENEMY_2 3

#define L3_NUM_ENEMY_1 1
#define L3_NUM_ENEMY_2 2
#define L3_NUM_ENEMY_3 2									// Two of the larger ship on screen at once


/* Images */

#define PLAYER_OLD_IMG "SpaceFlier_sm_1.png"				// Old player sprite on sprite sheet using _drawNode
#define PLAYER_IMG "PlayerShip3.png"						// New Player sprite
#define PLAYER_CANON "PlayerShipGun.png"					// The weapon on the players ship

//#define POWER_UP_LIFE_IMG "powerHeart.png"					// Life power up image
//#define POWER_UP_WEAPON_IMG "WeaponPowerUp.png"				// Weapon power up image
#define POWER_UP_LIFE_IMG2 "PowerUpLife.png"				// Life power up image on sprite sheet
#define POWER_UP_WEAPON_IMG2 "PowerUpWeapon.png"			// Weapon power up image on sprite sheet

// Dpad
#define DPAD_BASE "BaseCircle320b.png"						// Base for Dpad
#define DPAD_ARROW "Arrow160b.png"							// Arrow for Dpad
#define DPAD_ARROW_ACTIVE "Arrow160Pressedb.png"			// Arrow is active

// Background
#define ASTEROID_IMG "asteroid.png"							// Asteroid image
#define ASTEROID2_IMG "Asteroid.png"						// Asteroid image

// Weapoons
#define LASER_BLUE_IMG "laserbeam_blue.png"					// Blue laser image
#define LASER_ORANGE_IMG "LaserOrange.png"					// Orange laser image
#define LASER_GREEN_IMG "LaserGreen.png"					// Green laser image

// Enemies and obstacles
#define NUM_ENEMIES 3										// Number of enemies to add to lists etc.
#define MAX_NUM_ASTEROIDS_L1 10								// Number of asteroids
#define NUM_ASTEROIDS_L2 15									// Number of asteroids
#define NUM_LASERS 8										// Number of lasers

// Max Enemy Lives - floats used to create percentage for health bars
#define MAX_ENEMY_SHIP1_LIVES 3.0f							// EnemyShip 
#define MAX_ENEMY_SHIP2_LIVES 4.0f							// EnemyShipKling
#define MAX_ENEMY_SHIP3_LIVES 5.0f							// EnemyShipWilknot
#define MAX_ENEMY_SHIP4_LIVES 80.0f							// EnemyShipDerpStar
#define ENEMY_FIRE_RATE 500.0f								// Time between laser spawns

// Player
#define MAX_PLAYER_LIVES 5									// The maximum number of lives a player can have
#define MAX_PLAYER_HEALTH 10.0f								// The maximum health for the player
#define PLAYER_SPEED 4.0f									// Players speed
#define MAX_WEAPON_LEVEL 4									// The current weapon the player has
#define CHECK_VISIBLE_TIME 500.0f							// Check the player is visible every half a second

// Screen Resolution
#define RES_720P 720
#define RES_1080P 1080

// Menus
#define TRANSITION_TIME 0.5									// Transition time between sceness

#define DISPLAY_TIME_SPLASH_SCENE 2							// Time to display the spash scene for
#define DISPLAY_TIME_SPLASH_STORY 10						// Time to display the story between levels

#define MAIN_MENU_BUTTONS 6									// Number of buttons displayed on main menu

// Types etc.
enum difficulty { EASY, MEDIUM, HARD };
enum laserTypes { BLUE, ORANGE, GREEN1, GREEN2, GREEN3 };
enum asteroidType { STATIC_ROID, DYNAMIC_ROID };
enum powerUpTypes { NEW_LIFE, WEAPON_UPGRADE };

//static unsigned int NUM_LASERS_TO_FIRE = 2;				// Moved to player class

#endif // _DEFINES_H_

