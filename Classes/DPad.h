//
//  DPad.h
//  RPGCollab
//
//  Easy Arrows by Felix Mo
//  Rewritten by Jordan Brown on 9/1/14.
//
//  Originally for Cocos2d, now for Cocos-2dx 3.0
//
// =====================================================================
// EasyArrows Test Version
// Quickly create a 4-arrows menu, saving some decent coding time.
// =====================================================================
// Required graphics:
//     > A base, which will be like the table for placing the arrows.
//     > One arrow facing up.
//     > One arrow facing up, pressed.
// =====================================================================
// Usage:
//   Call
//      DPad for beginning the system. This function will require
//                   you to pass a graphic name for the base, another for
//					 the arrow, and another for the arrow being pressed.
//					 Then the position of the system (CGPoint).
//
//      getButton	 will let you ask for one of the four arrow buttons.
//                   You pass on a integer, which can be:
//                   8 -> up   2 -> down   6 -> right   4 -> left
//                   You will receive the CCMenuItemImage representing
//					 said button. You can work with this.
// =====================================================================
// Extra Features:
//   Call
//		setCorner    will automatically place the whole system in one
//                   of the iPhone's screen corners, pretty useful.
//					 You need to pass a integer representign which corner:
//                   1 -> Top Left		 2 -> Top Right
//                   3 -> Bottom Left    4 -> Bottom Right
//                   <TEST VERSION: ONLY WORKS FOR LANDSCAPE MODE!!!>
// =====================================================================

#ifndef __RPGCollab__DPad__
#define __RPGCollab__DPad__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class DPad: public Node{

public:
	/*
	// DPad singleton
	static DPad* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new DPad();
			return s_pInstance;
		}
		return s_pInstance;
	}
	*/
    static DPad *create(std::string base, std::string buttonImage, std::string pressedButtonImage, Point position);

    DPad();
    ~DPad();

	bool init(cocos2d::Layer *layer);										// JOR 13/12/2018 initialise the directional pad
    MenuItemImage *getButton(int button);
    void setCorner(int corner);

private:
    MenuItemImage *up;
    MenuItemImage *down;
    MenuItemImage *left;
    MenuItemImage *right;
	MenuItemImage *aBtn;
	MenuItemImage *bBtn;
    Sprite *bg;    

	Size visibleSize;
	//static DPad* s_pInstance;												// Single instance of Game used as singleton, so only one instance exists thoughout the game
};
#endif /* defined(__RPGCollab__DPad__) */
