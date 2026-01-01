/****************************************************************************
 * CollisionManager.h
 * 
 * 碰撞检测管理器 - 统一处理所有碰撞检测逻辑
 ****************************************************************************/

#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__

#include "cocos2d.h"

// 前向声明
class Player;

class CollisionManager {
public:
    /**
     * @brief 执行碰撞检测
     * @param scene 场景对象
     * @param player 玩家对象
     * @param score 当前分数（引用，碰撞后会修改）
     * @return 是否发生玩家死亡碰撞
     */
    static bool checkCollisions(cocos2d::Scene* scene, Player* player, int& score);
    
private:
    /**
     * @brief 检查玩家子弹与敌人的碰撞
     * @param scene 场景对象
     * @param score 当前分数
     */
    static void checkBulletEnemyCollision(cocos2d::Scene* scene, int& score);
    
    /**
     * @brief 检查敌人与玩家的碰撞
     * @param scene 场景对象
     * @param player 玩家对象
     * @return 是否发生碰撞
     */
    static bool checkEnemyPlayerCollision(cocos2d::Scene* scene, Player* player);
    
    /**
     * @brief 检查敌方子弹与玩家的碰撞
     * @param scene 场景对象
     * @param player 玩家对象
     * @return 是否发生碰撞
     */
    static bool checkEnemyBulletPlayerCollision(cocos2d::Scene* scene, Player* player);
    
    /**
     * @brief 创建爆炸特效
     * @param scene 场景对象
     * @param position 爆炸位置
     */
    static void createExplosion(cocos2d::Scene* scene, const cocos2d::Vec2& position);
};

#endif // __COLLISION_MANAGER_H__

