/*
	Joe O'Regan
	HelloWorldScene.h
	02/02/2018
*/
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"

USING_NS_CC;

typedef enum {
	KENDREASONWIN,
	KENDREASONLOSE
} EndReason;

class HelloWorld : public Layer {
public:    
    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    void menuCloseCallback(cocos2d::Ref* pSender);							// a selector callback
        
    CREATE_FUNC(HelloWorld);												// implement the "static create()" method manually

	virtual void onAcceleration(Acceleration* acc, Event* event);
	float randomValueBetween(float low, float high);
	void setInvisible(Node * node);
	float getTimeTick();
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	void scrollBackground(float dt);										// 20180202 Scroll the background objects
	void checkGameOver(float currenTime);									// 20180202 Check have game objects collided with each other
	void checkCollisions();													// 20180202 Check is the game over or not
	void moveShip(float dt);												// 20180202 Move the player ship
	void spawnAsteroids(float curTimeMillis);								// 20180202 Spawn asteroids

private:
	SpriteBatchNode *_batchNode;
	Sprite *_ship;															// The main character
	ParallaxNodeExtras *_backgroundNode;
	Sprite *_spaceDust1, *_spaceDust2, *_planetSunrise;
	Sprite *_galaxy, *_spatialAnomaly1, *_spatialAnomaly2;
	float _shipPointsPerSecY;												// How much distance to move the ship 
	Vector<Sprite*> *_asteroids;											// List of asteroids
	int _nextAsteroid=0;
	float _nextAsteroidSpawn=0;												// time to spawn next asteroid
	Vector<Sprite*> *_shipLasers;											// List of lasers
	int _nextShipLaser=0;
	int _lives=0;															// Player lives
	double _gameOverTime;
	bool _gameOver=false;
		
	Size winSize;															// Size of the game window

	void update(float dt);
	void endScene(EndReason endReason);
	void restartTapped(Ref* pSender);
};

#endif // __HELLOWORLD_SCENE_H__
