/****************************************************************************
 * Boss.cpp
 ****************************************************************************/

#include "Boss.h"
#include "EnemyBullet.h"
#include "Core/GameConfig.h"
#include "Utils/MathHelper.h"

USING_NS_CC;

Boss* Boss::createAndStart(Scene* scene) {
    auto boss = new (std::nothrow) Boss();
    if (boss && boss->initWithFile("enemy.png")) {  // 使用 enemy.png 作为占位符
        boss->autorelease();
        boss->m_scene = scene;
        boss->init();
        
        auto visibleSize = MathHelper::getVisibleSize();
        boss->setPosition(Vec2(visibleSize.width / 2, visibleSize.height + 100));
        
        boss->playEntranceAnimation();
        
        return boss;
    }
    CC_SAFE_DELETE(boss);
    return nullptr;
}

bool Boss::init() {
    // 设置Boss属性
    setScale(GameConfig::BOSS_SCALE);
    setName(GameConfig::Tags::BOSS);
    
    m_collisionBox = CollisionBox(
        GameConfig::BOSS_COLLISION_SIZE, 
        GameConfig::BOSS_COLLISION_SIZE
    );
    
    // 初始化血量
    m_maxHealth = GameConfig::BOSS_MAX_HEALTH;
    m_health = m_maxHealth;
    
    // 初始化护盾
    m_shieldActive = true;
    initShield();
    
    // 初始化血条
    initHealthBar();
    
    // 初始化弹幕模式
    m_currentPattern = 0;
    
    return true;
}

void Boss::playEntranceAnimation() {
    auto visibleSize = MathHelper::getVisibleSize();
    float targetY = visibleSize.height - 150;
    
    // 降落动画
    auto moveDown = MoveTo::create(2.0f, Vec2(visibleSize.width / 2, targetY));
    auto easeMove = EaseBackOut::create(moveDown);
    
    auto startAttack = CallFunc::create([this]() {
        // 护盾持续时间
        auto wait = DelayTime::create(GameConfig::BOSS_SHIELD_DURATION);
        auto removeShieldAction = CallFunc::create([this]() {
            removeShield();
        });
        runAction(Sequence::create(wait, removeShieldAction, nullptr));
        
        // 开始移动和攻击
        startMovementPattern();
        startAttackPattern();
    });
    
    runAction(Sequence::create(easeMove, startAttack, nullptr));
}

void Boss::startMovementPattern() {
    auto visibleSize = MathHelper::getVisibleSize();
    float centerX = visibleSize.width / 2;
    float currentY = getPositionY();
    float moveRange = 150.0f;
    
    // 左右往返移动
    auto moveLeft = MoveTo::create(3.0f, Vec2(centerX - moveRange, currentY));
    auto moveRight = MoveTo::create(3.0f, Vec2(centerX + moveRange, currentY));
    auto easeLeft = EaseSineInOut::create(moveLeft);
    auto easeRight = EaseSineInOut::create(moveRight);
    
    auto moveSequence = Sequence::create(easeLeft, easeRight, nullptr);
    runAction(RepeatForever::create(moveSequence));
}

void Boss::initShield() {
    if (!m_shieldActive) return;
    
    // 创建一个简单的护盾（使用DrawNode直接添加）
    m_shieldSprite = DrawNode::create();
    if (m_shieldSprite) {
        DrawNode* drawNode = static_cast<DrawNode*>(m_shieldSprite);
        drawNode->drawSolidCircle(Vec2::ZERO, 80, 0, 50, Color4F(0, 0.5f, 1.0f, 0.3f));
        drawNode->drawCircle(Vec2::ZERO, 80, 0, 50, false, Color4F(0, 0.7f, 1.0f, 0.8f));
        
        m_shieldSprite->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
        this->addChild(m_shieldSprite, -1);
        
        // 护盾旋转动画
        auto rotate = RotateBy::create(3.0f, 360);
        m_shieldSprite->runAction(RepeatForever::create(rotate));
        
        // 护盾呼吸效果
        auto scaleUp = ScaleTo::create(1.0f, 1.1f);
        auto scaleDown = ScaleTo::create(1.0f, 0.9f);
        auto breathe = Sequence::create(scaleUp, scaleDown, nullptr);
        m_shieldSprite->runAction(RepeatForever::create(breathe));
    }
}

void Boss::removeShield() {
    if (m_shieldSprite) {
        // 护盾破碎效果
        auto fadeOut = FadeOut::create(0.5f);
        auto scaleUp = ScaleTo::create(0.5f, 1.5f);
        auto spawn = Spawn::create(fadeOut, scaleUp, nullptr);
        auto remove = RemoveSelf::create();
        m_shieldSprite->runAction(Sequence::create(spawn, remove, nullptr));
        m_shieldSprite = nullptr;
    }
    m_shieldActive = false;
}

void Boss::initHealthBar() {
    auto visibleSize = MathHelper::getVisibleSize();
    
    // 血条背景（使用DrawNode）
    auto bgDrawNode = DrawNode::create();
    bgDrawNode->drawSolidRect(Vec2(-150, -10), Vec2(150, 10), Color4F(0.3f, 0.3f, 0.3f, 0.8f));
    bgDrawNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
    m_scene->addChild(bgDrawNode, 100);
    m_healthBarBg = bgDrawNode;
    
    // 血条填充（使用DrawNode）
    auto fillDrawNode = DrawNode::create();
    fillDrawNode->drawSolidRect(Vec2(-148, -8), Vec2(148, 8), Color4F(1.0f, 0.2f, 0.2f, 1.0f));
    fillDrawNode->setAnchorPoint(Vec2(0, 0.5f));
    fillDrawNode->setPosition(Vec2(-148, 0));
    m_healthBarBg->addChild(fillDrawNode);
    m_healthBarFill = fillDrawNode;
}

void Boss::updateHealthBar() {
    if (m_healthBarFill) {
        float healthPercent = (float)m_health / (float)m_maxHealth;
        m_healthBarFill->setScaleX(healthPercent);
        
        // 重绘血条以改变颜色
        DrawNode* fillNode = static_cast<DrawNode*>(m_healthBarFill);
        fillNode->clear();
        
        Color4F fillColor;
        if (healthPercent > 0.5f) {
            fillColor = Color4F(1.0f, 0.2f, 0.2f, 1.0f);
        } else if (healthPercent > 0.25f) {
            fillColor = Color4F(1.0f, 0.6f, 0.0f, 1.0f);
        } else {
            fillColor = Color4F(1.0f, 1.0f, 0.0f, 1.0f);
        }
        
        fillNode->drawSolidRect(Vec2(-148, -8), Vec2(148, 8), fillColor);
    }
}

void Boss::startAttackPattern() {
    switchToNextPattern();
}

void Boss::switchToNextPattern() {
    if (!getParent()) return;
    
    // 随机选择攻击模式
    m_currentPattern = RandomHelper::random_int(0, 3);
    
    auto wait = DelayTime::create(GameConfig::BOSS_PATTERN_SWITCH_INTERVAL);
    auto attack = CallFunc::create([this]() {
        switch(m_currentPattern) {
            case 0: attackPattern_CircularSpread(); break;
            case 1: attackPattern_Spiral(); break;
            case 2: attackPattern_FanSpray(); break;
            case 3: attackPattern_Homing(); break;
        }
    });
    auto next = CallFunc::create([this]() {
        switchToNextPattern();
    });
    
    runAction(Sequence::create(wait, attack, next, nullptr));
}

void Boss::attackPattern_CircularSpread() {
    // 圆形扩散弹幕
    int bulletCount = 16;
    float angleStep = 360.0f / bulletCount;
    
    for (int i = 0; i < bulletCount; i++) {
        float angle = angleStep * i;
        float radian = CC_DEGREES_TO_RADIANS(angle);
        
        auto bullet = EnemyBullet::create(getPosition());
        if (bullet && m_scene) {
            m_scene->addChild(bullet, -1);
            
            // 设置子弹方向
            Vec2 direction(cos(radian), sin(radian));
            Vec2 velocity = direction * GameConfig::BOSS_BULLET_SPEED;
            
            auto move = MoveBy::create(5.0f, velocity * 5.0f);
            bullet->runAction(move);
        }
    }
}

void Boss::attackPattern_Spiral() {
    // 螺旋弹幕
    float currentAngle = 0;
    
    for (int wave = 0; wave < 8; wave++) {
        auto delay = DelayTime::create(wave * 0.1f);
        auto fire = CallFunc::create([this, wave]() {
            for (int i = 0; i < 6; i++) {
                float angle = (wave * 45 + i * 60) % 360;
                float radian = CC_DEGREES_TO_RADIANS(angle);
                
                auto bullet = EnemyBullet::create(getPosition());
                if (bullet && m_scene) {
                    m_scene->addChild(bullet, -1);
                    
                    Vec2 direction(cos(radian), sin(radian));
                    Vec2 velocity = direction * GameConfig::BOSS_BULLET_SPEED;
                    
                    auto move = MoveBy::create(5.0f, velocity * 5.0f);
                    bullet->runAction(move);
                }
            }
        });
        
        runAction(Sequence::create(delay, fire, nullptr));
    }
}

void Boss::attackPattern_FanSpray() {
    // 扇形密集弹幕
    for (int wave = 0; wave < 5; wave++) {
        auto delay = DelayTime::create(wave * 0.2f);
        auto fire = CallFunc::create([this]() {
            // 向下方扇形发射
            for (int i = 0; i < 12; i++) {
                float angle = 180 + 60 - (i * 10);  // 120度到240度的扇形
                float radian = CC_DEGREES_TO_RADIANS(angle);
                
                auto bullet = EnemyBullet::create(getPosition());
                if (bullet && m_scene) {
                    m_scene->addChild(bullet, -1);
                    
                    Vec2 direction(cos(radian), sin(radian));
                    Vec2 velocity = direction * GameConfig::BOSS_BULLET_SPEED * 0.8f;
                    
                    auto move = MoveBy::create(5.0f, velocity * 5.0f);
                    bullet->runAction(move);
                }
            }
        });
        
        runAction(Sequence::create(delay, fire, nullptr));
    }
}

void Boss::attackPattern_Homing() {
    // 追踪弹（简化版，向玩家方向发射）
    auto player = m_scene->getChildByName(GameConfig::Tags::PLAYER);
    if (!player) return;
    
    for (int i = 0; i < 8; i++) {
        auto delay = DelayTime::create(i * 0.3f);
        auto fire = CallFunc::create([this, player]() {
            Vec2 direction = player->getPosition() - getPosition();
            direction.normalize();
            
            // 添加一些随机偏移
            float randomAngle = RandomHelper::random_real(-15.0f, 15.0f);
            float radian = CC_DEGREES_TO_RADIANS(randomAngle);
            float cos_val = cos(radian);
            float sin_val = sin(radian);
            Vec2 rotated(
                direction.x * cos_val - direction.y * sin_val,
                direction.x * sin_val + direction.y * cos_val
            );
            
            auto bullet = EnemyBullet::create(getPosition());
            if (bullet && m_scene) {
                m_scene->addChild(bullet, -1);
                bullet->setColor(Color3B(255, 100, 255));  // 紫色追踪弹
                
                Vec2 velocity = rotated * GameConfig::BOSS_BULLET_SPEED * 0.6f;
                auto move = MoveBy::create(5.0f, velocity * 5.0f);
                bullet->runAction(move);
            }
        });
        
        runAction(Sequence::create(delay, fire, nullptr));
    }
}

bool Boss::takeDamage(int damage) {
    if (m_shieldActive) {
        // 护盾抵挡伤害，显示效果
        if (m_shieldSprite) {
            auto blink = Blink::create(0.2f, 2);
            m_shieldSprite->runAction(blink);
        }
        return false;
    }
    
    m_health -= damage;
    if (m_health < 0) m_health = 0;
    
    updateHealthBar();
    
    // 受伤闪烁
    auto blink = Blink::create(0.2f, 2);
    runAction(blink);
    
    // 检查是否被击败
    if (m_health <= 0) {
        // 死亡动画
        auto fadeOut = FadeOut::create(1.0f);
        auto scaleUp = ScaleTo::create(1.0f, 2.0f);
        auto rotate = RotateBy::create(1.0f, 360);
        auto spawn = Spawn::create(fadeOut, scaleUp, rotate, nullptr);
        
        // 移除血条
        if (m_healthBarBg) {
            m_healthBarBg->removeFromParent();
        }
        
        auto remove = RemoveSelf::create();
        runAction(Sequence::create(spawn, remove, nullptr));
        
        return true;  // Boss被击败
    }
    
    return false;
}

Rect Boss::getCollisionRect() const {
    return MathHelper::createCollisionRect(
        const_cast<Boss*>(this), 
        m_collisionBox
    );
}

