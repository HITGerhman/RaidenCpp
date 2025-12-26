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
    // 新增：发射子弹的函数 (参数 dt 是两帧之间的时间间隔，Cocos 调度器标准格式)
    void updateFire(float dt);
    // [新增] 生成敌人的函数
    void spawnEnemy(float dt);
    // [新增] 碰撞检测主循环
    void updateCollision(float dt);

    private:
    // 新增：保存主角的指针，方便在其他函数里访问它
    cocos2d::Node* _player;
};

#endif // __HELLOWORLD_SCENE_H__