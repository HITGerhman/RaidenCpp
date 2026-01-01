/****************************************************************************
 * Player.h
 * 
 * 玩家飞机类 - 封装玩家相关的所有行为
 ****************************************************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Utils/GameTypes.h"

class Player : public cocos2d::Sprite {
public:
    /**
     * @brief 创建玩家
     * @return 玩家实例
     */
    static Player* create();
    
    /**
     * @brief 初始化玩家
     * @return 是否成功
     */
    virtual bool init() override;
    
    /**
     * @brief 根据输入状态更新玩家位置
     * @param inputState 输入状态
     * @param dt 时间间隔
     */
    void updateMovement(const InputState& inputState, float dt);
    
    /**
     * @brief 移动玩家（触摸/鼠标控制）
     * @param delta 移动增量
     */
    void moveByDelta(const cocos2d::Vec2& delta);
    
    /**
     * @brief 获取碰撞矩形
     * @return 碰撞矩形
     */
    cocos2d::Rect getCollisionRect() const;
    
    /**
     * @brief 获取子弹发射位置
     * @return 发射位置
     */
    cocos2d::Vec2 getFirePosition() const;
    
    /**
     * @brief 设置游戏结束状态（变红）
     */
    void setGameOverState();
    
private:
    CollisionBox m_collisionBox;
    
    /**
     * @brief 限制位置在屏幕内
     */
    void clampPosition();
};

#endif // __PLAYER_H__

