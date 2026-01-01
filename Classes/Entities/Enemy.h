/****************************************************************************
 * Enemy.h
 * 
 * 敌机类 - 封装敌人的生成、移动和射击行为
 ****************************************************************************/

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Utils/GameTypes.h"

class Enemy : public cocos2d::Sprite {
public:
    /**
     * @brief 创建并启动敌机
     * @param scene 场景对象（用于生成子弹）
     * @return 敌机实例
     */
    static Enemy* createAndStart(cocos2d::Scene* scene);
    
    /**
     * @brief 初始化敌机
     * @return 是否成功
     */
    virtual bool init() override;
    
    /**
     * @brief 获取碰撞矩形
     * @return 碰撞矩形
     */
    cocos2d::Rect getCollisionRect() const;
    
private:
    CollisionBox m_collisionBox;
    
    /**
     * @brief 开始移动
     * @param startX 起始 X 坐标
     */
    void startMovement(float startX);
    
    /**
     * @brief 开始射击循环
     * @param scene 场景对象
     */
    void startFiring(cocos2d::Scene* scene);
};

#endif // __ENEMY_H__

