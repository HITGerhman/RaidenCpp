/****************************************************************************
 * CollisionManager.cpp
 ****************************************************************************/

#include "CollisionManager.h"
#include "Entities/Player.h"
#include "Entities/Boss.h"
#include "Core/GameConfig.h"
#include "Utils/MathHelper.h"

USING_NS_CC;

bool CollisionManager::checkCollisions(Scene* scene, Player* player, int& score) {
    if (!scene || !player) return false;
    
    checkBulletEnemyCollision(scene, score);
    
    if (checkEnemyPlayerCollision(scene, player)) {
        return true;
    }
    
    if (checkEnemyBulletPlayerCollision(scene, player)) {
        return true;
    }
    
    return false;
}

void CollisionManager::checkBulletEnemyCollision(Scene* scene, int& score) {
    auto children = scene->getChildren();
    std::vector<Node*> bulletsToDelete;
    std::vector<Node*> enemiesToDelete;
    
    for (auto child : children) {
        if (child->getName() == GameConfig::Tags::BULLET) {
            Rect bulletRect = Rect(
                child->getPositionX() - GameConfig::BULLET_COLLISION_SIZE / 2,
                child->getPositionY() - GameConfig::BULLET_COLLISION_SIZE / 2,
                GameConfig::BULLET_COLLISION_SIZE,
                GameConfig::BULLET_COLLISION_SIZE
            );
            
            // 检测普通敌人
            for (auto target : children) {
                if (target->getName() == GameConfig::Tags::ENEMY) {
                    Rect enemyRect = Rect(
                        target->getPositionX() - GameConfig::ENEMY_COLLISION_SIZE / 2,
                        target->getPositionY() - GameConfig::ENEMY_COLLISION_SIZE / 2,
                        GameConfig::ENEMY_COLLISION_SIZE,
                        GameConfig::ENEMY_COLLISION_SIZE
                    );
                    
                    if (MathHelper::checkCollision(bulletRect, enemyRect)) {
                        bulletsToDelete.push_back(child);
                        enemiesToDelete.push_back(target);
                        
                        score += GameConfig::SCORE_PER_KILL;
                        createExplosion(scene, target->getPosition());
                    }
                }
                // 检测Boss
                else if (target->getName() == GameConfig::Tags::BOSS) {
                    Boss* boss = dynamic_cast<Boss*>(target);
                    if (boss) {
                        Rect bossRect = boss->getCollisionRect();
                        
                        if (MathHelper::checkCollision(bulletRect, bossRect)) {
                            bulletsToDelete.push_back(child);
                            
                            // Boss受到伤害
                            if (boss->takeDamage(1)) {
                                // Boss被击败
                                score += GameConfig::BOSS_DEFEAT_SCORE;
                                createExplosion(scene, boss->getPosition());
                            }
                        }
                    }
                }
            }
        }
    }
    
    for (auto node : bulletsToDelete) {
        if (node->getParent()) node->removeFromParent();
    }
    for (auto node : enemiesToDelete) {
        if (node->getParent()) node->removeFromParent();
    }
}

bool CollisionManager::checkEnemyPlayerCollision(Scene* scene, Player* player) {
    auto children = scene->getChildren();
    
    for (auto child : children) {
        if (child->getName() == GameConfig::Tags::ENEMY || child->getName() == GameConfig::Tags::BOSS) {
            Rect enemyRect = child->getBoundingBox();
            Rect playerRect = player->getCollisionRect();
            
            if (MathHelper::checkCollision(enemyRect, playerRect)) {
                return true;
            }
        }
    }
    
    return false;
}

bool CollisionManager::checkEnemyBulletPlayerCollision(Scene* scene, Player* player) {
    auto children = scene->getChildren();
    
    for (auto child : children) {
        if (child->getName() == GameConfig::Tags::ENEMY_BULLET) {
            Rect bulletRect = Rect(
                child->getPositionX() - GameConfig::ENEMY_BULLET_COLLISION_SIZE / 2,
                child->getPositionY() - GameConfig::ENEMY_BULLET_COLLISION_SIZE / 2,
                GameConfig::ENEMY_BULLET_COLLISION_SIZE,
                GameConfig::ENEMY_BULLET_COLLISION_SIZE
            );
            Rect playerRect = player->getCollisionRect();
            
            if (MathHelper::checkCollision(bulletRect, playerRect)) {
                return true;
            }
        }
    }
    
    return false;
}

void CollisionManager::createExplosion(Scene* scene, const Vec2& position) {
    auto explosion = DrawNode::create();
    explosion->drawDot(Vec2::ZERO, GameConfig::EXPLOSION_RADIUS, Color4F::ORANGE);
    explosion->setPosition(position);
    scene->addChild(explosion);
    
    auto scaleAction = ScaleTo::create(GameConfig::EXPLOSION_DURATION, GameConfig::EXPLOSION_SCALE);
    auto removeAction = RemoveSelf::create();
    explosion->runAction(Sequence::create(scaleAction, removeAction, nullptr));
}

