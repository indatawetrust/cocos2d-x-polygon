#ifndef __NEW_SCENE_H__
#define __NEW_SCENE_H__

#include "cocos2d.h"

class New : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	cocos2d::Vector<cocos2d::DrawNode *> polygonArray;

	CREATE_FUNC(New);
private:
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };
    
    cocos2d::PhysicsWorld *sceneWorld;
};

#endif