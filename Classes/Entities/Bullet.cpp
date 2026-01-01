/****************************************************************************
 * Bullet.cpp
 ****************************************************************************/

#include "Bullet.h"
#include "Core/GameConfig.h"
#include "Utils/MathHelper.h"

USING_NS_CC;

Bullet* Bullet::createAndFire(const Vec2& startPos, float velocity, const char* tag) {
    auto bullet = new (std::nothrow) Bullet();
    if (bullet && bullet->initWithFile("bullet.png")) {
        bullet->autorelease();
        bullet->setName(tag);
        bullet->setPosition(startPos);
        bullet->initWithVelocity(velocity);
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool Bullet::initWithVelocity(float velocity) {
    // 计算飞行目标位置和时间
    auto visibleSize = MathHelper::getVisibleSize();
    float targetY = (velocity > 0) ? visibleSize.height + 100 : -100;
    float distance = std::abs(getPositionY() - targetY);
    float duration = distance / std::abs(velocity);
    
    // 创建移动动作
    auto moveAction = MoveTo::create(duration, Vec2(getPositionX(), targetY));
    auto removeAction = RemoveSelf::create();
    auto sequence = Sequence::create(moveAction, removeAction, nullptr);
    
    runAction(sequence);
    return true;
}

Rect Bullet::getCollisionRect() const {
    return MathHelper::createCollisionRect(
        const_cast<Bullet*>(this), 
        m_collisionBox
    );
}

