/*
	EnemyShip.h

	Joe O'Regan
	K00203642

	Enemy Ship base class is a sub class of Sprite
*/

#ifndef __ENEMY_SHIP_H__
#define __ENEMY_SHIP_H__

#include "cocos2d.h"
#include "HealthBar.h"
#include "Defines.h"

class EnemyShip : public cocos2d::Sprite {
public:
	EnemyShip() {}
	~EnemyShip() {}
	
	static EnemyShip* create(cocos2d::Size res);		// Create the enemy ship
	
	virtual void init(cocos2d::Size res);				// Initialise the enemy

	virtual void update(float dt);						// Virtual update function

	virtual void moveCanon() {}							// Used with third enemy type (EnemyShipWillKnot) to move its (ornamental) canon
		
	void takeLife();									// Decrement the number of lives
	
	unsigned int getLives() const { return m_lives; }	// Get the lives left for the Enemy Ship
	void setLives(unsigned int set) { m_lives = set; }

	float getNextFire() const { return m_nextFire; }	// Get the current fire rate	

	float getDuration() { return m_duration; }			// Random duration on screen, used in spawnObject() for applying action sequence
	
	virtual void spawnLaser() { }						// Spawn a laser

	cocos2d::Sprite* m_pCanon1;							// Ship canons (only used on Wilknot, and Derp Star enemies)
	cocos2d::Sprite* m_pCanon2;
	cocos2d::Sprite* m_pCanon3;

protected:
	cocos2d::Size m_winSize;							// Screen resolution

	// health
	unsigned int m_lives = MAX_ENEMY_SHIP1_LIVES;		// Number of lives (Defines.h)
	float m_totalLives = MAX_ENEMY_SHIP1_LIVES;			// Total lives for this enemy to set/rest lives (Defines.h)
	HealthBar* m_pBar;									// HealthBar

	// Fire rate
	float m_nextFire;									// Time next laser spawned
	unsigned int m_fireRate = ENEMY_FIRE_RATE;			// Time between lasers (Defines.h)

	float m_dx = 0;										// Offset for spawning the laser in front of the ship
	float m_dy = 0;
	
	float m_duration = 0;								// Duration the object is on screen (speed it travels to target point off screen)
	float m_speedMin = 2.0f;							// Set the min duration to travel across screen (Speed = faster)
	float m_speedMax = 10.0f;							// Set the max duration to travel across screen (Speed = slower)
	float m_randYPos;									// Spawn at random position on Y access
};


//void updateBar(float pc) {
//bar->setPercent(1);
//bar->update(0.0f);
//	pt->setPercentage(pc);
//}
//HealthBar* createHealthBar(cocos2d::Size res);	
//void myUpdate();
//void moveTo();
/*
//class EnemyShip : public cocos2d::Node {
class EnemyShip {
public:
	//EnemyShip(cocos2d::Layer *layer);				// Player constructor

	//void update();									// Update player movement etc.

	cocos2d::Sprite* getSprite() { return enemyShip; }
	void setSprite(cocos2d::Sprite* set) { enemyShip = set; }

	unsigned int getLives() { return lives; }
	void setLives(unsigned int set) { lives = set; }

private:
	cocos2d::Size visibleSize;						// Visible screen size
	cocos2d::Vec2 origin;							// Origin of the screen

	cocos2d::Sprite *enemyShip;						// Player sprite

	unsigned int lives = 3;
	//unsigned int ammo = 10;
};
*/
#endif // __ENEMY_SHIP_H__