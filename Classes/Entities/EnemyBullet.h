/****************************************************************************
 * EnemyBullet.h
 * 
 * 敌方子弹类
 ****************************************************************************/

#ifndef __ENEMY_BULLET_H__
#define __ENEMY_BULLET_H__

#include "Bullet.h"

class EnemyBullet : public Bullet {
public:
    /**
     * @brief 创建敌方子弹
     * @param startPos 起始位置
     * @return 子弹实例
     */
    static EnemyBullet* create(const cocos2d::Vec2& startPos);
    
    virtual bool init() override;
};

#endif // __ENEMY_BULLET_H__

