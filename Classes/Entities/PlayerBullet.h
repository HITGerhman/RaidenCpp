/****************************************************************************
 * PlayerBullet.h
 * 
 * 玩家子弹类
 ****************************************************************************/

#ifndef __PLAYER_BULLET_H__
#define __PLAYER_BULLET_H__

#include "Bullet.h"

class PlayerBullet : public Bullet {
public:
    /**
     * @brief 创建玩家子弹
     * @param startPos 起始位置
     * @return 子弹实例
     */
    static PlayerBullet* create(const cocos2d::Vec2& startPos);
    
    virtual bool init() override;
};

#endif // __PLAYER_BULLET_H__

