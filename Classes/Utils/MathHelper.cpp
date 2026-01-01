/****************************************************************************
 * MathHelper.cpp
 ****************************************************************************/

#include "MathHelper.h"

USING_NS_CC;

Vec2 MathHelper::clampToScreen(const Vec2& position) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Vec2 clampedPos = position;
    
    if (clampedPos.x < 0) clampedPos.x = 0;
    if (clampedPos.x > visibleSize.width) clampedPos.x = visibleSize.width;
    if (clampedPos.y < 0) clampedPos.y = 0;
    if (clampedPos.y > visibleSize.height) clampedPos.y = visibleSize.height;
    
    return clampedPos;
}

bool MathHelper::checkCollision(const Rect& rect1, const Rect& rect2) {
    return rect1.intersectsRect(rect2);
}

Rect MathHelper::createCollisionRect(Node* node, const CollisionBox& box) {
    if (!node) return Rect::ZERO;
    
    Vec2 pos = node->getPosition();
    return Rect(
        pos.x - box.size.width / 2 + box.offset.x,
        pos.y - box.size.height / 2 + box.offset.y,
        box.size.width,
        box.size.height
    );
}

bool MathHelper::isOutOfScreen(Node* node, float margin) {
    if (!node) return true;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 pos = node->getPosition();
    
    return pos.x < -margin || 
           pos.x > visibleSize.width + margin ||
           pos.y < -margin || 
           pos.y > visibleSize.height + margin;
}

Size MathHelper::getVisibleSize() {
    return Director::getInstance()->getVisibleSize();
}

