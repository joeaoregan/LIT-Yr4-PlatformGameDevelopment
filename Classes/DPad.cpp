/*
	Joe O'Regan
	K00203642

	Original Version:
	DPad.cpp 
	RPGCollab
	Created by Jordan Brown on 9/1/14.
*/

#include "DPad.h"

//DPad* DPad::s_pInstance;	// DPad Singleton

DPad::DPad(){}				// Constructor	
DPad::~DPad() {}			// Destructor

bool DPad::init(cocos2d::Layer *layer) {	
	m_visibleSize = Director::getInstance()->getVisibleSize();
	/*
	// If the target platform is a mobile device (android in this case)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		//controller = DPad::create("Base01.png", "Button01.png", "ButtonPressed01.png", Point(150, 150));
		if (visibleSize.height == 1080)
			//layer->addChild(DPad::create("Base225.png", "Arrow72.png", "Arrow72Pressed.png", Point(200, 200)));
			layer->addChild(create("Base300.png", "Arrow96.png", "Arrow96Pressed.png", Point(250, 250)));
		else
			//layer->addChild(DPad::create("Base01.png", "Button01.png", "ButtonPressed01.png", Point(150, 150)));
			layer->addChild(create("Base150.png", "Arrow.png", "ArrowPressed.png", Point(150, 150)));
	}
	*/

	//layer->addChild(create("Base300.png", "Arrow96.png", "Arrow96Pressed.png", Point(250, 250)));

	return true;
}

DPad *DPad::create(std::string base, std::string buttonImage, std::string pressedButtonImage, Point position){

    DPad *controller = new DPad();

    if (true) {
		cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();

		//int scaleDown = (controller->visibleSize.height == 1080) ? 1.0f : 0.67f;
        controller->autorelease();					// Set to autorelease

        controller->m_pBGSprite = Sprite::create(base);		// Set the background image
        controller->m_pBGSprite->setPosition(position);		// Set the background position
        controller->addChild(controller->m_pBGSprite,100);	// Add as child of bg
        controller->m_pUpItem = MenuItemImage::create(buttonImage,pressedButtonImage);		// Create up arrow menu item
        controller->m_pDownItem = MenuItemImage::create(buttonImage,pressedButtonImage);	// Create down arrow menu item
		controller->m_pLeftItem = MenuItemImage::create(buttonImage, pressedButtonImage);	// Create left arrow menu item
		controller->m_pRightItem = MenuItemImage::create(buttonImage, pressedButtonImage);	// Create right arrow menu item

		controller->m_pABtnItem = MenuItemImage::create("btnA.png", "btnASelect.png");		// Create A button menu item
		controller->m_pBBtnItem = MenuItemImage::create("btnB.png", "btnBSelect.png");		// Create B button menu item

		// Set positions and rotations
        controller->m_pUpItem->setPosition(Point(controller->m_pBGSprite->getPosition().x, controller->m_pBGSprite->getPosition().y + controller->m_pBGSprite->getContentSize().height * 0.6f - controller->m_pUpItem->getContentSize().height/2));
        controller->m_pDownItem->setPosition(Point(controller->m_pBGSprite->getPosition().x,controller->m_pBGSprite->getPosition().y - controller->m_pBGSprite->getContentSize().height * 0.6f + controller->m_pDownItem->getContentSize().height/2));
		controller->m_pRightItem->setPosition(Point(controller->m_pBGSprite->getPosition().x + controller->m_pBGSprite->getContentSize().width * 0.6f - controller->m_pDownItem->getContentSize().height / 2, controller->m_pBGSprite->getPosition().y));
		controller->m_pLeftItem->setPosition(Point(controller->m_pBGSprite->getPosition().x - controller->m_pBGSprite->getContentSize().width * 0.6f + controller->m_pDownItem->getContentSize().height / 2, controller->m_pBGSprite->getPosition().y));

		controller->m_pABtnItem->setPosition(Point(visibleSize.width * 0.8f, position.y - (controller->m_pBBtnItem->getContentSize().height / 2)));
		controller->m_pBBtnItem->setPosition(Point(visibleSize.width * 0.9f, position.y + (controller->m_pBBtnItem->getContentSize().height / 2)));

        controller->m_pRightItem->setRotation(90);
        controller->m_pDownItem->setRotation(180);
        controller->m_pLeftItem->setRotation(-90);
		//controller->setScale(scaleDown);

        Menu *menu = Menu::create(controller->m_pUpItem,controller->m_pDownItem,controller->m_pLeftItem, controller->m_pRightItem, controller->m_pABtnItem, controller->m_pBBtnItem, NULL);
        menu->setPosition(Point(0,0));
        controller->addChild(menu, 120);
        // assign color value

		CCLOG("Controller Created");
        
        return controller;
    }

    CC_SAFE_DELETE(controller);
    return NULL;
}

MenuItemImage *DPad::getButton(int button){
    MenuItemImage *result;
    switch (button) {
        case 8: result = DPad::m_pUpItem; break;		// Move up
        case 2: result = DPad::m_pDownItem; break;		// Move down
        case 6: result = DPad::m_pRightItem; break;	// Move left
		case 4: result = DPad::m_pLeftItem; break;		// Move right
		case 10: result = DPad::m_pABtnItem; break;	// Move button A
		case 11: result = DPad::m_pBBtnItem; break;	// Move button B
        default: break;
    }

    return result;
}

void DPad::setCorner(int corner){
    Point position;
	switch (corner) {
		case 1:
            position = Point(m_pBGSprite->getContentSize().width/2, Director::getInstance()->getWinSize().height - m_pBGSprite->getContentSize().height/2);
			break;
		case 2:
            position = Point(Director::getInstance()->getWinSize().width - m_pBGSprite->getContentSize().width/2,Director::getInstance()->getWinSize().height - m_pBGSprite->getContentSize().height/2);
			break;
		case 3:
			position = Point(m_pBGSprite->getContentSize().width/2,m_pBGSprite->getContentSize().height/2);
			break;
		case 4:
			position = Point(Director::getInstance()->getWinSize().width - m_pBGSprite->getContentSize().width/2,m_pBGSprite->getContentSize().height/2);
			break;
		default:
			break;
	}

	m_pBGSprite->setPosition(position);
	m_pUpItem->setPosition(Point(m_pBGSprite->getPosition().x,m_pBGSprite->getPosition().y + m_pBGSprite->getContentSize().height/2 - m_pUpItem->getContentSize().height/2));
	m_pDownItem->setPosition(Point(m_pBGSprite->getPosition().x,m_pBGSprite->getPosition().y - m_pBGSprite->getContentSize().height/2 + m_pDownItem->getContentSize().height/2));
	m_pRightItem->setPosition(Point(m_pBGSprite->getPosition().x + m_pBGSprite->getContentSize().width/2 - m_pRightItem->getContentSize().width/2,m_pBGSprite->getPosition().y));
	m_pLeftItem->setPosition(Point(m_pBGSprite->getPosition().x - m_pBGSprite->getContentSize().width/2 + m_pLeftItem->getContentSize().width/2,m_pBGSprite->getPosition().y));
}
