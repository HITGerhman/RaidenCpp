/****************************************************************************
 * PlayerBullet.cpp
 ****************************************************************************/

#include "PlayerBullet.h"
#include "Core/GameConfig.h"

USING_NS_CC;

PlayerBullet* PlayerBullet::create(const Vec2& startPos) {
    auto bullet = createAndFire(
        startPos, 
        GameConfig::BULLET_SPEED, 
        GameConfig::Tags::BULLET
    );
    
    if (bullet) {
        bullet->setScale(GameConfig::BULLET_SCALE);
        static_cast<PlayerBullet*>(bullet)->m_collisionBox = CollisionBox(
            GameConfig::BULLET_COLLISION_SIZE, 
            GameConfig::BULLET_COLLISION_SIZE
        );
    }
    
    return static_cast<PlayerBullet*>(bullet);
}

bool PlayerBullet::init() {
    return Sprite::init();
}

