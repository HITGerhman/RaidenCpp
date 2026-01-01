/****************************************************************************
 * EnemyBullet.cpp
 ****************************************************************************/

#include "EnemyBullet.h"
#include "Core/GameConfig.h"

USING_NS_CC;

EnemyBullet* EnemyBullet::create(const Vec2& startPos) {
    auto bullet = createAndFire(
        startPos, 
        -GameConfig::ENEMY_BULLET_SPEED,  // 负数表示向下
        GameConfig::Tags::ENEMY_BULLET
    );
    
    if (bullet) {
        bullet->setScale(GameConfig::ENEMY_BULLET_SCALE);
        bullet->setColor(Color3B::RED);
        bullet->setRotation(180);  // 弹头朝下
        
        static_cast<EnemyBullet*>(bullet)->m_collisionBox = CollisionBox(
            GameConfig::ENEMY_BULLET_COLLISION_SIZE, 
            GameConfig::ENEMY_BULLET_COLLISION_SIZE
        );
    }
    
    return static_cast<EnemyBullet*>(bullet);
}

bool EnemyBullet::init() {
    return Sprite::init();
}

