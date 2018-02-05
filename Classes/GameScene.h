/*
	Joe O'Regan
	GameScene.h
	02/02/2018
*/
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"
//#include <sstream>															// 20180204 score
#include "Player.h"

USING_NS_CC;

typedef enum {
	KENDREASONWIN,
	KENDREASONLOSE
} EndReason;

class GameScene : public Layer {
public:    
    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    void menuCloseCallback(cocos2d::Ref* pSender);							// a selector callback
        
    CREATE_FUNC(GameScene);													// implement the "static create()" method manually

	virtual void onAcceleration(Acceleration* acc, Event* event);
	float randomValueBetween(float low, float high);
	void setInvisible(Node * node);
	float getTimeTick();
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	void spawnLaser();														// 20180205
	void spawn2Lasers();													// 20180205

	void scrollBackground(float dt);										// 20180202 Scroll the background objects
	void checkGameOver(float currenTime);									// 20180202 Check have game objects collided with each other
	void checkCollisions();													// 20180202 Check is the game over or not
	void moveShip(float dt);												// 20180202 Move the player ship
	void spawnAsteroids(float curTimeMillis);								// 20180202 Spawn asteroids
	void updateTimer();														// 20180204 Update the countdown timer
	void getInput();
	/*
	void moveUp(cocos2d::Ref* pSender);
	void moveDown(cocos2d::Ref* pSender);
	void moveLeft(cocos2d::Ref* pSender);
	void moveRight(cocos2d::Ref* pSender);
	*/
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	//void showScore();

	// Keyboard
	//bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	//double keyPressedDuration(cocos2d::EventKeyboard::KeyCode);

private:
	Player* player;

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
	int _nextShipLaser=0;													// Ship laser list index
	int _lives=0;															// Player lives
	double _gameOverTime;
	bool _gameOver=false;
		
	Size winSize;															// Size of the game window

	void update(float dt);
	void endScene(EndReason endReason);
	void restartTapped(Ref* pSender);

	cocos2d::Label* scoreLabel;												// Display the current score
	cocos2d::Label* levelLabel;												// Display the current level
	cocos2d::Label * timeLabel;												// Display the time remaining
	//std::stringstream scoreText;
	unsigned int score;
	unsigned int level;
	unsigned int time;

	int currentTime;

	// Keyboard
	//static std::map<cocos2d::EventKeyboard::KeyCode,
	//	std::chrono::high_resolution_clock::time_point> keys;
};

#endif // __GAME_SCENE_H__
