/*
	Joe O'Regan
	GameScene.h
	02/02/2018
*/
#ifndef __LEVEL_1_H__
#define __LEVEL_1_H__

#include "cocos2d.h"
//#include "ParallaxNodeExtras.h"
//#include <sstream>															// 20180204 score
#include "Player.h"
#include "Game.h"

USING_NS_CC;

enum EndReason1 {
	KENDREASONWIN,
	KENDREASONLOSE
};

class Level1 : public Layer {
public:
	/*
	// Game singleton
	static Level1* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Level1();
			return s_pInstance;
		}
		return s_pInstance;
	}
	*/
    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        
    void menuCloseCallback(cocos2d::Ref* pSender);							// a selector callback
        
    CREATE_FUNC(Level1);													// implement the "static create()" method manually

	//virtual void onAcceleration(Acceleration* acc, Event* event);
	float randomValueBetween(float low, float high);
	void setInvisible(Node * node);
	float getTimeTick();
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);

	void spawnLaser();														// 20180205
	void spawn2Lasers();													// 20180205

	void checkGameOver(float currenTime);									// 20180202 Check have game objects collided with each other
	void checkCollisions();													// 20180202 Check is the game over or not
	void moveShip(float dt);												// 20180202 Move the player ship
	void spawnAsteroids(float curTimeMillis);								// 20180202 Spawn asteroids
	void spawnEnemyShips(float curTimeMillis);								// 20180214 Spawn enemy ships
	void updateTimer(float curTimeMillis);									// 20180204 Update the countdown timer, 21/02/2018 Passing curTimeMillis solves Android timer issue
	void getInput();
	//unsigned int getScore() { return score; }								// 20180214 Return the current score

private:
	Player* player;

	SpriteBatchNode *_batchNode;
	Sprite *playerLife;														// Indicate lives left
	Sprite *EnemyShip;
	ParallaxNodeExtras *_backgroundNode;
	//float _shipPointsPerSecY;												// Accelerometer: How much distance to move the ship 
	Vector<Sprite*> *_asteroids;											// List of asteroids
	Vector<Sprite*> *EnemyShipList;											// List of enemy ships
	int _nextAsteroid = 0;
	int nextEnemyShip = 0;
	float _nextAsteroidSpawn = 0;											// time to spawn next asteroid
	float nextEnemyShipSpawnTime = 0;										// Time to spawn next enemy ship
	Vector<Sprite*> *_shipLasers;											// List of lasers
	int _nextShipLaser = 0;													// Ship laser list index
	Sprite* livesList[5];													// List of lives
	//int _lives=0;															// Player lives, Moved to Game.h

	double _gameOverTime;
	bool _gameOver=false;
		
	Size winSize;															// Size of the game window

	void update(float dt);
	void endScene(EndReason1 endReason);
	void restartTapped(Ref* pSender);
	void startLevel2(Ref* pSender);											// 20180218 Progress to the next level
	void returnToMenu(Ref* pSender);										// 20180218 Return to the main menu

	cocos2d::Label* scoreLabel;												// Display the current score
	cocos2d::Label * timeLabel;												// Display the time remaining

	//unsigned int score;													// Moved to Game.h
	unsigned int level;
	unsigned int time;

	float currentTime;														// 20180221 Change to float to fix Android timer issue

	//static Level1* s_pInstance;											// Single instance of GameScene used as singleton, so only one instance exists thoughout the game
};

#endif // __LEVEL_1_H__

/*
void initBG(cocos2d::Layer *layer);
void scrollBackground(float dt);											// 20180202 Scroll the background objects
void moveUp(cocos2d::Ref* pSender);
void moveDown(cocos2d::Ref* pSender);
void moveLeft(cocos2d::Ref* pSender);
void moveRight(cocos2d::Ref* pSender);
void showScore();
// Keyboard
bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
double keyPressedDuration(cocos2d::EventKeyboard::KeyCode);

	std::stringstream scoreText;
	Sprite *_ship;															// The main character
	Sprite *_spaceDust1, *_spaceDust2, *_planetSunrise;
	Sprite *_galaxy, *_spatialAnomaly1, *_spatialAnomaly2;
	// Keyboard
	//static std::map<cocos2d::EventKeyboard::KeyCode,
	//	std::chrono::high_resolution_clock::time_point> keys;
*/
