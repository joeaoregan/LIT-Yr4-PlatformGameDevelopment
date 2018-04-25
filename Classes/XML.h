// https://medium.com/@teratapgames/using-tinyxml-with-cocos2d-x-a-headache-time-saving-example-a5395ee6d34

#ifndef __XML_Parser_H__
#define __XML_Parser_H__

#include <stdio.h>
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "tinyxml2.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#include "external\tinyxml2\tinyxml2.h"
#endif

using namespace tinyxml2;
//using namespace cocos2d;

class XML {
public:
	XML() {
		//this->parseXML();
		this->parseAudioXML();
	}

	void parseXML();
	void parseAudioXML();
	//const char** parseAudioXML();

	const char* track1;
	const char* track2;
	const char* track3;

	const char* audio[3];
};

#endif