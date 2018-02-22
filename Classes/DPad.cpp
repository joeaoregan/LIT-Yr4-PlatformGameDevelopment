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
	Size visibleSize = Director::getInstance()->getVisibleSize();

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
	

	//layer->addChild(create("Base300.png", "Arrow96.png", "Arrow96Pressed.png", Point(250, 250)));

	return true;
}

DPad *DPad::create(std::string base, std::string buttonImage, std::string pressedButtonImage, Point position){
    DPad *controller = new DPad();

    if (true) {
        // Set to autorelease
        controller->autorelease();

        controller->bg = Sprite::create(base);		// Set the background image
        controller->bg->setPosition(position);		// Set the background position
        controller->addChild(controller->bg,100);	// Add as child of bg
        controller->up = MenuItemImage::create(buttonImage,pressedButtonImage);		// Create up arrow menu item
        controller->down = MenuItemImage::create(buttonImage,pressedButtonImage);	// Create down arrow menu item
        controller->left = MenuItemImage::create(buttonImage,pressedButtonImage);	// Create left arrow menu item
        controller->right = MenuItemImage::create(buttonImage,pressedButtonImage);	// Create right arrow menu item
		// Set positions and rotations
        controller->up->setPosition(Point(controller->bg->getPosition().x, controller->bg->getPosition().y + controller->bg->getContentSize().height/2 - controller->up->getContentSize().height/2));
        controller->down->setPosition(Point(controller->bg->getPosition().x,controller->bg->getPosition().y - controller->bg->getContentSize().height/2 + controller->down->getContentSize().height/2));
        controller->right->setPosition(Point(controller->bg->getPosition().x + controller->bg->getContentSize().width/2 - controller->down->getContentSize().width/2, controller->bg->getPosition().y));
        controller->left->setPosition(Point(controller->bg->getPosition().x - controller->bg->getContentSize().width/2 + controller->down->getContentSize().width/2,controller->bg->getPosition().y));
        controller->right->setRotation(90);
        controller->down->setRotation(180);
        controller->left->setRotation(-90);

        Menu *menu = Menu::create(controller->up,controller->down,controller->left,controller->right,NULL);
        menu->setPosition(Point(0,0));
        controller->addChild(menu, 120);
        // assign color value
        
        return controller;
    }

    CC_SAFE_DELETE(controller);
    return NULL;
}

MenuItemImage *DPad::getButton(int button){
    MenuItemImage *result;
    switch (button) {
        case 8: result = DPad::up; break;		// Move up
        case 2: result = DPad::down; break;		// Move down
        case 6: result = DPad::right; break;	// Move left
        case 4: result = DPad::left; break;		// Move right
        default: break;
    }

    return result;
}

void DPad::setCorner(int corner){
    Point position;
	switch (corner) {
		case 1:
            position = Point(bg->getContentSize().width/2, Director::getInstance()->getWinSize().height - bg->getContentSize().height/2);
			break;
		case 2:
            position = Point(Director::getInstance()->getWinSize().width - bg->getContentSize().width/2,Director::getInstance()->getWinSize().height - bg->getContentSize().height/2);
			break;
		case 3:
			position = Point(bg->getContentSize().width/2,bg->getContentSize().height/2);
			break;
		case 4:
			position = Point(Director::getInstance()->getWinSize().width - bg->getContentSize().width/2,bg->getContentSize().height/2);
			break;
		default:
			break;
	}

	bg->setPosition(position);
	up->setPosition(Point(bg->getPosition().x,bg->getPosition().y + bg->getContentSize().height/2 - up->getContentSize().height/2));
	down->setPosition(Point(bg->getPosition().x,bg->getPosition().y - bg->getContentSize().height/2 + down->getContentSize().height/2));
	right->setPosition(Point(bg->getPosition().x + bg->getContentSize().width/2 - right->getContentSize().width/2,bg->getPosition().y));
	left->setPosition(Point(bg->getPosition().x - bg->getContentSize().width/2 + left->getContentSize().width/2,bg->getPosition().y));
}
