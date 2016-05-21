#include "NewScene.h"
#include "math.h"
#include <vector>
using namespace std;

USING_NS_CC;

Scene* New::createScene()
{
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );

	auto layer = New::create();

	layer->SetPhysicsWorld( scene->getPhysicsWorld() );

	scene->addChild(layer);

	return scene;
}

// polygon points creator

vector<Vec2> points(int i,int w,int x,int y)
{
    vector<Vec2> rect;

    for(int j=0;j<i;j++){
    	rect.push_back(Vec2(w * sin(atan(1)*4*(j*360/i)/180) + x,
    			    w * cos(atan(1)*4*(j*360/i)/180) + y));
    }

    return rect;
}

// polygon touch test

bool touchPoints(vector<Vec2> points,int x,int y)
{
	int j = points.size()-1;
	bool isTouch = false;

	for(int i=0;i<points.size();i++)
	{
		if((points[i].y< y && points[j].y>=y 
		 || points[j].y< y && points[i].y>=y)
    	&& (points[i].x<=x || points[j].x<=x)) {
      		isTouch ^= (points[i].x+(y-points[i].y)/
      				   (points[j].y-points[i].y)*(points[j].x-points[i].x)<x); 
		}
    
    	j=i; 

	}

	return isTouch;
}

// polygon creator

DrawNode* polygon(int i,int w,int * color) 
{
    auto polygonNode = DrawNode::create();

    auto _color = Color4F(color[0],color[1],color[2],1);

    Vec2 polygon[i];

    for(int j=0;j<i;j++)
    {
    	polygon[j] = Vec2(w * sin(atan(1)*4*(j*360/i)/180),
    			  w * cos(atan(1)*4*(j*360/i)/180));
    }

    polygonNode->drawPolygon(polygon, i, _color, 1, _color);

    return polygonNode;
}

bool New::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin      = Director::getInstance()->getVisibleOrigin();

    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT,3);

    auto edgeNode = Node::create();

    edgeNode->setPosition( Point(visibleSize.width/2+origin.x,visibleSize.height/2+origin.y) );

    edgeNode->setPhysicsBody( edgeBody );

    this->addChild(edgeNode);

    auto listener = EventListenerTouchOneByOne::create();
	
    auto dispatcher = Director::getInstance()->getEventDispatcher();

    listener->setSwallowTouches(true);

    listener->onTouchBegan = [&](Touch* touch,Event* event) {
        auto target = static_cast<DrawNode *>(event->getCurrentTarget());
    	
    	Point pt = touch->getLocation();

    	bool isTouch = false;

        for(auto polygon: this->polygonArray)
	{
		if(touchPoints(points(5,50,polygon->getPositionX(),polygon->getPositionY()),pt.x,pt.y)){
			polygon->removeFromParentAndCleanup(true);

			this->polygonArray.eraseObject(polygon, false);

			isTouch = true;

			break;
		}
	}

	if(!isTouch){
	    int color[3] = { RandomHelper::random_int(0,255),
    	    	  	 RandomHelper::random_int(0,255),
    	    	 	 RandomHelper::random_int(0,255) };

    	    int edgeNum = RandomHelper::random_int(3,8);
    	    int edgeLen = RandomHelper::random_int(20,50);
		
	    auto polygonNode = polygon(edgeNum, edgeLen, color);

	    this->polygonArray.pushBack(polygonNode);
	
	    int i = edgeNum;
    	    int w = edgeLen;

    	    Vec2 polygon[i];

	    for(int j=0;j<i;j++)
	    {
	    	polygon[j] = Vec2(w * sin(atan(1)*4*(j*360/i)/180),
	    				   w * cos(atan(1)*4*(j*360/i)/180));
	    }
		
	    auto physicsBody = PhysicsBody::createPolygon(polygon,
							  edgeNum,
							  PhysicsMaterial(0.1f, 0.5f, 0.0f));
    
	    physicsBody->setDynamic(true);

	    polygonNode->setPhysicsBody(physicsBody);
      				
	    polygonNode->setPosition(Vec2(pt.x,pt.y));

	    target->addChild(polygonNode);
	}

    	return false;
    };

    listener->onTouchMoved = [](Touch* touch,Event* event) {

    };

    listener->onTouchEnded = [=](Touch* touch,Event* event) {

    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

    return true;
}
