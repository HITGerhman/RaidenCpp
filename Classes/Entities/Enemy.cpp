/****************************************************************************
 * Enemy.cpp
 ****************************************************************************/

#include "Enemy.h"
#include "EnemyBullet.h"
#include "Core/GameConfig.h"
#include "Utils/MathHelper.h"

USING_NS_CC;

Enemy* Enemy::createAndStart(Scene* scene) {
    auto enemy = new (std::nothrow) Enemy();
    if (enemy && enemy->initWithFile("enemy.png")) {
        enemy->autorelease();
        enemy->init();
        
        auto visibleSize = MathHelper::getVisibleSize();
        float randomX = RandomHelper::random_real(
            GameConfig::ENEMY_SPAWN_MARGIN, 
            visibleSize.width - GameConfig::ENEMY_SPAWN_MARGIN
        );
        
        enemy->setPosition(Vec2(randomX, visibleSize.height + GameConfig::ENEMY_SPAWN_MARGIN));
        enemy->startMovement(randomX);
        enemy->startFiring(scene);
        
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool Enemy::init() {
    // 注意：不要调用 Sprite::init()，因为 initWithFile() 已经完成了初始化
    
    setScale(GameConfig::ENEMY_SCALE);
    setName(GameConfig::Tags::ENEMY);
    
    m_collisionBox = CollisionBox(
        GameConfig::ENEMY_COLLISION_SIZE, 
        GameConfig::ENEMY_COLLISION_SIZE
    );
    
    return true;
}

void Enemy::startMovement(float startX) {
    auto moveAction = MoveTo::create(
        GameConfig::ENEMY_MOVE_DURATION, 
        Vec2(startX, -50)
    );
    auto removeAction = RemoveSelf::create();
    auto sequence = Sequence::create(moveAction, removeAction, nullptr);
    
    runAction(sequence);
}

void Enemy::startFiring(Scene* scene) {
    auto wait = DelayTime::create(GameConfig::ENEMY_FIRE_INTERVAL);
    auto fire = CallFunc::create([=]() {
        if (getParent() && scene) {
            auto bullet = EnemyBullet::create(getPosition());
            scene->addChild(bullet, -1);
        }
    });
    
    auto fireSequence = Sequence::create(wait, fire, nullptr);
    runAction(RepeatForever::create(fireSequence));
}

Rect Enemy::getCollisionRect() const {
    return MathHelper::createCollisionRect(
        const_cast<Enemy*>(this), 
        m_collisionBox
    );
}

