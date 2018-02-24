/*
	Level1.h

	Joe O'Regan
	K00203642
	02/02/2018

	Level1 header file
*/
#ifndef __LEVEL_1_H__
#define __LEVEL_1_H__

//#include "cocos2d.h"
//#include "ParallaxNodeExtras.h"
//#include <sstream>															// 20180204 score
//#include "Player.h"
//#include "Game.h"
#include "Level.h"

USING_NS_CC;
/*
enum EndReason1 {
	KENDREASONWIN,
	KENDREASONLOSE
};
*/

class Level1 : public Level {
public:
    static cocos2d::Scene* createScene();									// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    
    virtual bool init();													// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

	virtual void endScene(EndReason endReason);
                
    CREATE_FUNC(Level1);													// implement the "static create()" method manually

	//void createStatusBar(int x, int y, int w, int h, float Percent, Color4F FGColor, Color4F BGColor, int orientation, int startFrom);

	void checkCollisions();													// Check collisions with different objects in different levels

private:

	void update(float dt);
};

#endif // __LEVEL_1_H__