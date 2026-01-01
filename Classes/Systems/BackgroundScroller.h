/****************************************************************************
 * BackgroundScroller.h
 * 
 * 背景滚动管理器 - 处理无限滚动背景
 ****************************************************************************/

#ifndef __BACKGROUND_SCROLLER_H__
#define __BACKGROUND_SCROLLER_H__

#include "cocos2d.h"

class BackgroundScroller {
public:
    BackgroundScroller();
    ~BackgroundScroller();
    
    /**
     * @brief 初始化背景
     * @param scene 场景对象
     */
    void initialize(cocos2d::Scene* scene);
    
    /**
     * @brief 更新背景滚动
     * @param dt 时间间隔
     */
    void update(float dt);
    
private:
    cocos2d::Sprite* m_background1;
    cocos2d::Sprite* m_background2;
    float m_scrollSpeed;
    float m_realHeight;
};

#endif // __BACKGROUND_SCROLLER_H__

