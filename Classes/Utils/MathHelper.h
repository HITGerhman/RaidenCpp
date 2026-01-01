/****************************************************************************
 * MathHelper.h
 * 
 * 数学工具类 - 提供边界检测、碰撞计算等通用功能
 ****************************************************************************/

#ifndef __MATH_HELPER_H__
#define __MATH_HELPER_H__

#include "cocos2d.h"
#include "GameTypes.h"

class MathHelper {
public:
    /**
     * @brief 将位置限制在屏幕范围内
     * @param position 要限制的位置
     * @return 限制后的位置
     */
    static cocos2d::Vec2 clampToScreen(const cocos2d::Vec2& position);
    
    /**
     * @brief 检查两个矩形是否相交
     * @param rect1 矩形1
     * @param rect2 矩形2
     * @return 是否相交
     */
    static bool checkCollision(const cocos2d::Rect& rect1, const cocos2d::Rect& rect2);
    
    /**
     * @brief 从节点和碰撞体创建碰撞矩形
     * @param node 节点对象
     * @param box 碰撞体定义
     * @return 碰撞矩形
     */
    static cocos2d::Rect createCollisionRect(cocos2d::Node* node, const CollisionBox& box);
    
    /**
     * @brief 检查节点是否在屏幕外
     * @param node 要检查的节点
     * @param margin 边距容差
     * @return 是否在屏幕外
     */
    static bool isOutOfScreen(cocos2d::Node* node, float margin = 0.0f);
    
    /**
     * @brief 获取屏幕可见区域大小
     * @return 可见区域大小
     */
    static cocos2d::Size getVisibleSize();
};

#endif // __MATH_HELPER_H__

