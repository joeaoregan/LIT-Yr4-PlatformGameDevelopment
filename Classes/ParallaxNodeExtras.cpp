/*
	Joe O'Regan
	ParallaxNodeExtras.cpp
	02/02/2018
*/
#include "Game.h"
//#include "ParallaxNodeExtras.h"


//// Hack to access CCPointObject (which is not a public class)
//class PointObject : public Ref {
//	CC_SYNTHESIZE(Point, _ratio, Ratio)
//	CC_SYNTHESIZE(Point, _offset, Offset)
//	CC_SYNTHESIZE(Node*, _child, Child)	// weak ref
//};
class PointObject : public Ref {
public:
	static PointObject * create(Point ratio, Point offset) {
		PointObject *ret = new PointObject();
		ret->initWithPoint(ratio, offset);
		ret->autorelease();										// Autorelease memory allocated by new
		return ret;
	}

	bool initWithPoint(Point ratio, Point offset) {
		_ratio = ratio;
		_offset = offset;
		_child = nullptr;
		return true;
	}

	inline const Point& getRatio() const { return _ratio; };
	inline void setRatio(const Point& ratio) { _ratio = ratio; };

	inline const Point& getOffset() const { return _offset; };
	inline void setOffset(const Point& offset) { _offset = offset; };

	inline Node* getChild() const { return _child; };
	inline void setChild(Node* child) { _child = child; };

private:
	Point _ratio;
	Point _offset;
	Node *_child; // weak ref
};


// Need to provide a constructor
ParallaxNodeExtras::ParallaxNodeExtras() {
	ParallaxNode::create(); // call parent constructor
}

ParallaxNodeExtras * ParallaxNodeExtras::create() {
	return new ParallaxNodeExtras();
}

void ParallaxNodeExtras::incrementOffset(Point offset, Node* node){
	for (int i = 0; i < _parallaxArray->num; i++) {
		PointObject *point = (PointObject *)_parallaxArray->arr[i];
		Node * curNode = point->getChild();
		if (curNode==/*->isEqual*/(node)) {
			point->setOffset(point->getOffset() + offset);
			break;
		}
	}
}

/*
	Originally I was using separate background images
	But for efficiency it is better to use one image and scale down from a high resolution
*/
bool ParallaxNodeExtras::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();

	std::string bgFileName;

	// 2) Create the sprites will be added to the ParallaxNode
	// Choose the background image based on the screen resolution and current level
	if (Game::Instance()->getLevel() <= 1 || Game::Instance()->getLevel() == 4)
		bgFileName = "bg_front_spacedust1080.png";
	else if (Game::Instance()->getLevel() == 2)
		bgFileName = "bg_front_spacedust1080L2.png";
	else if (Game::Instance()->getLevel() == 3)
		bgFileName = "bg_front_spacedust1080L3.png";

	/*
	if (visibleSize.height == 1080) {
	} else {
		if (Game::Instance()->getLevel() == 1 || Game::Instance()->getLevel() == 4)
			bgFileName = "bg_front_spacedust.png";
		else if (Game::Instance()->getLevel() == 2)
			bgFileName = "bg_front_spacedustL2.png";
		else if (Game::Instance()->getLevel() == 3)
			bgFileName = "bg_front_spacedustL3.png";
	}
	*/
	float scaleDown = (visibleSize.height == 1080) ? 1.0f : 0.67f;											// Scale down high res image
	//float scaleUp = (visibleSize.height == 1080) ? 1.5f : 1.0f;												// Scale up lower res image

	_spaceDust1 = Sprite::create(bgFileName);
	_spaceDust2 = Sprite::create(bgFileName);
	//_spaceDust1->setScale(scaleDown);
	//_spaceDust2->setScale(scaleDown);

	_planetSunrise = Sprite::create("bg_planetsunrise.png");
	_galaxy = Sprite::create("bg_galaxy.png");
	_spatialAnomaly1 = Sprite::create("bg_spacialanomaly.png");
	_spatialAnomaly2 = Sprite::create("bg_spacialanomaly2.png");

	_planetSunrise->setScale(scaleDown);
	_galaxy->setScale(scaleDown);
	_spatialAnomaly1->setScale(scaleDown);
	_spatialAnomaly2->setScale(scaleDown);

	// 3) Determine relative movement speeds for space dust and background
	cocos2d::Point dustSpeed = Point(0.1F, 0.1F);															// JOR replaced auto specifier
	cocos2d::Point bgSpeed = Point(0.05F, 0.05F);															// JOR replaced auto specifier

	// 4) Add children to ParallaxNode
	addChild(_spaceDust1, 0, dustSpeed, Point(0, visibleSize.height / 2));
	addChild(_spaceDust2, 0, dustSpeed, Point(_spaceDust1->getContentSize().width, visibleSize.height / 2));
	addChild(_galaxy, -1, bgSpeed, Point(0, visibleSize.height * 0.7));
	addChild(_planetSunrise, -1, bgSpeed, Point(600, visibleSize.height * 0));
	addChild(_spatialAnomaly1, -1, bgSpeed, Point(900, visibleSize.height * 0.3));
	addChild(_spatialAnomaly2, -1, bgSpeed, Point(1500, visibleSize.height * 0.9));


	//setScale(scaleDown);

	return true;
}

void ParallaxNodeExtras::update(float dt) {
	cocos2d::Point backgroundScrollVert = Point(-1000, 0);									// JOR replaced auto specifier
	setPosition(getPosition() + (backgroundScrollVert * dt));

	// Parallax
	cocos2d::Vector<cocos2d::Sprite*>* spaceDusts = new Vector<Sprite*>(2);					// JOR replaced auto specifier
	spaceDusts->pushBack(_spaceDust1);
	spaceDusts->pushBack(_spaceDust2);

	for (cocos2d::Sprite* spaceDust : *spaceDusts) {
		float xPosition = convertToWorldSpace(spaceDust->getPosition()).x;
		float size = spaceDust->getContentSize().width;
		if (xPosition < -size / 2) {
			incrementOffset(Point(spaceDust->getContentSize().width * 2, 0), spaceDust);
		}
	}

	cocos2d::Vector<cocos2d::Sprite*>* backGrounds = new Vector<Sprite*>(4);				// JOR replaced auto specifier
	backGrounds->pushBack(_galaxy);
	backGrounds->pushBack(_planetSunrise);
	backGrounds->pushBack(_spatialAnomaly1);
	backGrounds->pushBack(_spatialAnomaly2);

	for (cocos2d::Sprite* background : *backGrounds) {										// JOR replaced auto specifier
		float xPosition = convertToWorldSpace(background->getPosition()).x;
		float size = background->getContentSize().width;
		if (xPosition < -size) {
			incrementOffset(Point(2000, 0), background);
		}
	}
}