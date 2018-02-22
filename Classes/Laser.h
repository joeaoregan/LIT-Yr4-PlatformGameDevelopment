#ifndef __LASER_H__
#define __LASER_H__

#include "cocos2d.h"

class Laser : public cocos2d::Sprite {
public:
	void spawnLasers(int amount, cocos2d::Layer *layer) {
		int i;
		for ((amount == 2) ? i = 1 : i = 0; i < amount; i++) {
			cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);										// Next laser in the list, JOR replaced auto specifier
			if (_nextShipLaser >= _shipLasers->size())
				_nextShipLaser = 0;																				// Reset laser list index to 0 (go back to start of list)

			if (i == 0) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 0));	// middle
			if (i == 1) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, -12));	// bottom
			if (i == 2) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 12));	// top

			shipLaser->setVisible(true);
			shipLaser->stopAllActions();

			shipLaser->runAction(
				Sequence::create(MoveBy::create(0.5, Point(winSize.width, 0)), // change to plus 100 for up - 100 for down
					CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, layer)), NULL));
		}
	}
};

#endif // __LASER_H__