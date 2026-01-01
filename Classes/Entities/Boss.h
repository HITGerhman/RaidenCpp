/****************************************************************************
 * Boss.h
 * 
 * Boss类 - 东方风格的弹幕Boss
 * 拥有护盾、血量系统和多种弹幕攻击模式
 ****************************************************************************/

#ifndef __BOSS_H__
#define __BOSS_H__

#include "cocos2d.h"
#include "Utils/GameTypes.h"

class Boss : public cocos2d::Sprite {
public:
    /**
     * @brief 创建并启动Boss
     * @param scene 场景对象
     * @return Boss实例
     */
    static Boss* createAndStart(cocos2d::Scene* scene);
    
    /**
     * @brief 初始化Boss
     * @return 是否成功
     */
    virtual bool init() override;
    
    /**
     * @brief 获取碰撞矩形
     * @return 碰撞矩形
     */
    cocos2d::Rect getCollisionRect() const;
    
    /**
     * @brief 受到伤害
     * @param damage 伤害值
     * @return 是否被击败
     */
    bool takeDamage(int damage);
    
    /**
     * @brief 是否有护盾
     * @return 护盾状态
     */
    bool hasShield() const { return m_shieldActive; }
    
    /**
     * @brief 获取当前血量
     * @return 血量值
     */
    int getHealth() const { return m_health; }
    
    /**
     * @brief 获取最大血量
     * @return 最大血量值
     */
    int getMaxHealth() const { return m_maxHealth; }
    
private:
    cocos2d::Scene* m_scene;
    CollisionBox m_collisionBox;
    
    // 血量系统
    int m_health;
    int m_maxHealth;
    
    // 护盾系统
    bool m_shieldActive;
    cocos2d::Node* m_shieldSprite;
    
    // 血条
    cocos2d::Node* m_healthBarBg;
    cocos2d::Node* m_healthBarFill;
    
    // 弹幕模式
    int m_currentPattern;
    
    /**
     * @brief 登场动画
     */
    void playEntranceAnimation();
    
    /**
     * @brief 移动模式
     */
    void startMovementPattern();
    
    /**
     * @brief 初始化护盾
     */
    void initShield();
    
    /**
     * @brief 移除护盾
     */
    void removeShield();
    
    /**
     * @brief 初始化血条
     */
    void initHealthBar();
    
    /**
     * @brief 更新血条显示
     */
    void updateHealthBar();
    
    /**
     * @brief 开始攻击循环
     */
    void startAttackPattern();
    
    /**
     * @brief 弹幕模式1：圆形扩散
     */
    void attackPattern_CircularSpread();
    
    /**
     * @brief 弹幕模式2：螺旋弹幕
     */
    void attackPattern_Spiral();
    
    /**
     * @brief 弹幕模式3：扇形密集弹幕
     */
    void attackPattern_FanSpray();
    
    /**
     * @brief 弹幕模式4：追踪弹
     */
    void attackPattern_Homing();
    
    /**
     * @brief 切换到下一个攻击模式
     */
    void switchToNextPattern();
};

#endif // __BOSS_H__

