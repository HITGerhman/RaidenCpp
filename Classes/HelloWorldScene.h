#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // 这是 Cocos 的宏，用于实现 create() 方法
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__