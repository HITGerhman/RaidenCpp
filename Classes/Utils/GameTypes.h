/****************************************************************************
 * GameTypes.h
 * 
 * 游戏类型定义和枚举
 ****************************************************************************/

#ifndef __GAME_TYPES_H__
#define __GAME_TYPES_H__

#include "cocos2d.h"

/**
 * @brief 游戏状态枚举
 */
enum class GameState {
    RUNNING,    // 游戏运行中
    PAUSED,     // 游戏暂停
    GAME_OVER   // 游戏结束
};

/**
 * @brief 输入状态结构体
 */
struct InputState {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    
    bool isAnyPressed() const {
        return up || down || left || right;
    }
    
    void reset() {
        up = down = left = right = false;
    }
};

/**
 * @brief 碰撞矩形辅助结构
 */
struct CollisionBox {
    cocos2d::Vec2 offset;  // 相对于精灵中心的偏移
    cocos2d::Size size;    // 碰撞体大小
    
    CollisionBox() : offset(0, 0), size(0, 0) {}
    CollisionBox(float w, float h) : offset(0, 0), size(w, h) {}
    CollisionBox(float offsetX, float offsetY, float w, float h) 
        : offset(offsetX, offsetY), size(w, h) {}
};

#endif // __GAME_TYPES_H__

