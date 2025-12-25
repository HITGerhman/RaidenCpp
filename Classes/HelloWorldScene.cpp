#include "HelloWorldScene.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Init function
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. Draw the Player (Triangle)
    auto playerNode = DrawNode::create();
    
    Vec2 vertices[] = {
        Vec2(0, 50),
        Vec2(-30, -30),
        Vec2(30, -30)
    };
    
    playerNode->drawSolidPoly(vertices, 3, Color4F::YELLOW);
    playerNode->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    playerNode->setName("Player");
    
    this->addChild(playerNode);

    // 2. Log success
    CCLOG("Raiden Game Started!");

    return true;
}