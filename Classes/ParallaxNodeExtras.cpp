/*
	Joe O'Regan
	ParallaxNodeExtras.cpp
	02/02/2018
*/
#include "ParallaxNodeExtras.h"

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
		ret->autorelease();
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

bool ParallaxNodeExtras::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 2) Create the sprites will be added to the ParallaxNode
	_spaceDust1 = Sprite::create("bg_front_spacedust.png");
	_spaceDust2 = Sprite::create("bg_front_spacedust.png");
	_planetSunrise = Sprite::create("bg_planetsunrise.png");
	_galaxy = Sprite::create("bg_galaxy.png");
	_spatialAnomaly1 = Sprite::create("bg_spacialanomaly.png");
	_spatialAnomaly2 = Sprite::create("bg_spacialanomaly2.png");

	// 3) Determine relative movement speeds for space dust and background
	auto dustSpeed = Point(0.1F, 0.1F);
	auto bgSpeed = Point(0.05F, 0.05F);

	// 4) Add children to ParallaxNode
	addChild(_spaceDust1, 0, dustSpeed, Point(0, visibleSize.height / 2));
	addChild(_spaceDust2, 0, dustSpeed, Point(_spaceDust1->getContentSize().width, visibleSize.height / 2));
	addChild(_galaxy, -1, bgSpeed, Point(0, visibleSize.height * 0.7));
	addChild(_planetSunrise, -1, bgSpeed, Point(600, visibleSize.height * 0));
	addChild(_spatialAnomaly1, -1, bgSpeed, Point(900, visibleSize.height * 0.3));
	addChild(_spatialAnomaly2, -1, bgSpeed, Point(1500, visibleSize.height * 0.9));

	return true;
}

void ParallaxNodeExtras::update(float dt) {
	auto backgroundScrollVert = Point(-1000, 0);
	setPosition(getPosition() + (backgroundScrollVert * dt));

	// Parallax
	auto spaceDusts = new Vector<Sprite*>(2);
	spaceDusts->pushBack(_spaceDust1);
	spaceDusts->pushBack(_spaceDust2);
	for (auto spaceDust : *spaceDusts) {
		float xPosition = convertToWorldSpace(spaceDust->getPosition()).x;
		float size = spaceDust->getContentSize().width;
		if (xPosition < -size / 2) {
			incrementOffset(Point(spaceDust->getContentSize().width * 2, 0), spaceDust);
		}
	}
	
	auto backGrounds = new Vector<Sprite*>(4);
	backGrounds->pushBack(_galaxy);
	backGrounds->pushBack(_planetSunrise);
	backGrounds->pushBack(_spatialAnomaly1);
	backGrounds->pushBack(_spatialAnomaly2);

	for (auto background : *backGrounds) {
		float xPosition = convertToWorldSpace(background->getPosition()).x;
		float size = background->getContentSize().width;
		if (xPosition < -size) {
			incrementOffset(Point(2000, 0), background);
		}
	}
}