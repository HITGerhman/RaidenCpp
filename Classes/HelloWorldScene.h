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
    // [新增] 在指定位置生成一颗敌方子弹
    void createEnemyBullet(cocos2d::Vec2 pos);

    private:
    // 新增：保存主角的指针，方便在其他函数里访问它
    cocos2d::Node* _player;
    // [新增] 键盘按键状态标记
    bool _isPressingW = false;
    bool _isPressingS = false;
    bool _isPressingA = false;
    bool _isPressingD = false;

    // [新增] 专门处理键盘移动的函数
    void updatePlayerMovement(float dt);

    // [新增] 两张背景图
    cocos2d::Sprite* _bg1;
    cocos2d::Sprite* _bg2;
    // [新增] 游戏状态变量
    int _score;                  // 当前分数
    bool _isGameOver;            // 游戏是否结束
    cocos2d::Label* _scoreLabel; // 显示分数的文字标签

    // [新增] 背景滚动逻辑函数
    void updateBackground(float dt);
    // [新增] 统一处理游戏结束的函数
    void doGameOver();
};

#endif // __HELLOWORLD_SCENE_H__