// https://medium.com/@teratapgames/using-tinyxml-with-cocos2d-x-a-headache-time-saving-example-a5395ee6d34

// xml parsing test

#include "XML.h"

void XML::parseXML(){
	//parses XML containing hero definition
	cocos2d::FileUtils* fileUtils = cocos2d::FileUtils::getInstance();
	std::string path = fileUtils->fullPathForFilename(std::string("test.xml"));

	//reads contents of the file into a string
	std::string xmlData = fileUtils->getStringFromFile(path);

	//reads the xml document
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLError error = doc.Parse(xmlData.c_str());
	CCASSERT(error == XML_SUCCESS, "Cannot read test.xml");

	//gets the root element ==> <heroes>
	auto root = doc.RootElement();

	//reads the <hero element> underneath <heroes>
	auto heroEl = root->FirstChildElement("hero");

	//reads key and shape attribute values and stores them into a char array
	auto szKey = heroEl->Attribute("shape"); //char *
	auto szShape = heroEl->Attribute("shape"); //char *

	CCLOG("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	CCLOG("Test: %s", szKey);

	//now you can use the attribute values however you like,
	//for example, you could populate a Fish object with those values
}

void XML::parseAudioXML() {
//const char** XML::parseAudioXML() {
	//const char* audio[3];

	cocos2d::FileUtils* fileUtils = cocos2d::FileUtils::getInstance();				// parses XML containing hero definition
	std::string path = fileUtils->fullPathForFilename(std::string("audio.xml"));
		
	std::string xmlData = fileUtils->getStringFromFile(path);						// reads contents of the file into a string
		
	tinyxml2::XMLDocument doc;														// reads the xml document

	tinyxml2::XMLError error = doc.Parse(xmlData.c_str());
	CCASSERT(error == XML_SUCCESS, "Unable To Read audio.xml");
		
	tinyxml2::XMLElement* root = doc.RootElement();									// Get the root element ==> <heroes>	
	tinyxml2::XMLElement* tracks = root->FirstChildElement("music");				// Reads the <audio element> underneath <heroes>
	// tinyxml2::XMLElement* tracks = root->NextSiblingElement("audio");
	
	// reads key and shape attribute values and stores them into a char array
	track1 = tracks->Attribute("audio1");
	track2 = tracks->Attribute("audio2");
	track3 = tracks->Attribute("audio3");
	
	CCLOG("=============================================================");
	CCLOG("track1: %s, track2: %s, track3: %s", track1, track2, track3);			// Display the parsed file data
	CCLOG("=============================================================");

	audio[0] = track1;
	audio[1] = track2;
	audio[2] = track3;
	CCLOG("=============================================================");
	CCLOG("audio0: %s, audio1: %s, audio2: %s", audio[0], audio[1], audio[2]);			// Display the parsed file data
	CCLOG("=============================================================");

	//return audio;
}