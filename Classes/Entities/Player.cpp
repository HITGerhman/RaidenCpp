/****************************************************************************
 * Player.cpp
 ****************************************************************************/

#include "Player.h"
#include "Core/GameConfig.h"
#include "Utils/MathHelper.h"

USING_NS_CC;

Player* Player::create() {
    auto player = new (std::nothrow) Player();
    if (player && player->initWithFile("player.png")) {
        player->autorelease();
        player->init();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool Player::init() {
    // 注意：不要调用 Sprite::init()，因为 initWithFile() 已经完成了初始化
    
    // 设置基本属性
    setScale(GameConfig::PLAYER_SCALE);
    setName(GameConfig::Tags::PLAYER);
    
    // 设置初始位置
    auto visibleSize = MathHelper::getVisibleSize();
    setPosition(Vec2(visibleSize.width / 2, GameConfig::PLAYER_START_Y));
    
    // 设置碰撞体
    m_collisionBox = CollisionBox(
        GameConfig::PLAYER_COLLISION_OFFSET_X,
        GameConfig::PLAYER_COLLISION_OFFSET_Y,
        GameConfig::PLAYER_COLLISION_WIDTH,
        GameConfig::PLAYER_COLLISION_HEIGHT
    );
    
    return true;
}

void Player::updateMovement(const InputState& inputState, float dt) {
    if (!inputState.isAnyPressed()) return;
    
    Vec2 currentPos = getPosition();
    float speed = GameConfig::PLAYER_SPEED * dt;
    
    if (inputState.up) currentPos.y += speed;
    if (inputState.down) currentPos.y -= speed;
    if (inputState.left) currentPos.x -= speed;
    if (inputState.right) currentPos.x += speed;
    
    setPosition(currentPos);
    clampPosition();
}

void Player::moveByDelta(const Vec2& delta) {
    Vec2 newPos = getPosition() + delta;
    setPosition(newPos);
    clampPosition();
}

void Player::clampPosition() {
    setPosition(MathHelper::clampToScreen(getPosition()));
}

Rect Player::getCollisionRect() const {
    return MathHelper::createCollisionRect(
        const_cast<Player*>(this), 
        m_collisionBox
    );
}

Vec2 Player::getFirePosition() const {
    return getPosition() + Vec2(0, GameConfig::BULLET_OFFSET_Y);
}

void Player::setGameOverState() {
    setColor(Color3B::RED);
}

