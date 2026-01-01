/****************************************************************************
 * Bullet.h
 * 
 * 子弹基类 - 提供通用的子弹行为
 ****************************************************************************/

#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "Utils/GameTypes.h"

class Bullet : public cocos2d::Sprite {
public:
    /**
     * @brief 创建并发射子弹
     * @param startPos 起始位置
     * @param velocity 速度 (正数向上，负数向下)
     * @param tag 子弹标签名
     * @return 子弹实例
     */
    static Bullet* createAndFire(const cocos2d::Vec2& startPos, float velocity, const char* tag);
    
    /**
     * @brief 获取碰撞矩形
     * @return 碰撞矩形
     */
    cocos2d::Rect getCollisionRect() const;
    
protected:
    CollisionBox m_collisionBox;
    
    /**
     * @brief 初始化子弹
     * @param velocity 速度
     * @return 是否成功
     */
    bool initWithVelocity(float velocity);
};

#endif // __BULLET_H__

