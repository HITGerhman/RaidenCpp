/****************************************************************************
 * GameScene.cpp
 ****************************************************************************/

#include "GameScene.h"
#include "GameConfig.h"
#include "Entities/Player.h"
#include "Entities/PlayerBullet.h"
#include "Entities/Enemy.h"
#include "Entities/Boss.h"
#include "Systems/InputManager.h"
#include "Systems/CollisionManager.h"
#include "Systems/BackgroundScroller.h"
#include "Utils/MathHelper.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    auto scene = GameScene::create();
    return scene;
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    // 初始化游戏状态
    m_gameState = GameState::RUNNING;
    m_score = GameConfig::INITIAL_SCORE;
    m_player = nullptr;
    m_boss = nullptr;
    m_bossSpawned = false;
    m_inputManager = new InputManager();
    m_backgroundScroller = new BackgroundScroller();
    
    // 初始化各个模块
    initBackground();
    initPlayer();
    initUI();
    initInput();
    startGameLoop();
    
    return true;
}

void GameScene::initBackground() {
    m_backgroundScroller->initialize(this);
}

void GameScene::initPlayer() {
    m_player = Player::create();
    if (m_player) {
        this->addChild(m_player);
    }
}

void GameScene::initUI() {
    auto visibleSize = MathHelper::getVisibleSize();
    
    m_scoreLabel = Label::createWithSystemFont(
        "Score: 0", 
        "Arial", 
        GameConfig::SCORE_LABEL_FONT_SIZE
    );
    m_scoreLabel->setPosition(
        visibleSize.width / 2, 
        visibleSize.height - GameConfig::SCORE_LABEL_Y_OFFSET
    );
    m_scoreLabel->setTextColor(Color4B::WHITE);
    this->addChild(m_scoreLabel, GameConfig::SCORE_LABEL_Z_ORDER);
}

void GameScene::initInput() {
    m_inputManager->initialize(this, m_player);
}

void GameScene::startGameLoop() {
    this->schedule([this](float dt) { this->updateBackground(dt); }, "update_background_key");
    this->schedule([this](float dt) { this->updatePlayerMovement(dt); }, "update_player_movement_key");
    this->schedule([this](float dt) { this->updateFire(dt); }, GameConfig::FIRE_INTERVAL, "update_fire_key");
    this->schedule([this](float dt) { this->updateSpawnEnemy(dt); }, GameConfig::ENEMY_SPAWN_INTERVAL, "update_spawn_enemy_key");
    this->schedule([this](float dt) { this->updateCollision(dt); }, "update_collision_key");
}

void GameScene::updateBackground(float dt) {
    m_backgroundScroller->update(dt);
}

void GameScene::updatePlayerMovement(float dt) {
    if (m_gameState != GameState::RUNNING || !m_player) return;
    
    const auto& inputState = m_inputManager->getInputState();
    m_player->updateMovement(inputState, dt);
}

void GameScene::updateFire(float dt) {
    if (m_gameState != GameState::RUNNING) return;
    shootBullet();
}

void GameScene::updateSpawnEnemy(float dt) {
    if (m_gameState != GameState::RUNNING) return;
    
    // Boss出现后停止生成普通敌人
    if (m_bossSpawned) return;
    
    spawnEnemy();
}

void GameScene::updateCollision(float dt) {
    if (m_gameState != GameState::RUNNING) return;
    
    bool playerHit = CollisionManager::checkCollisions(this, m_player, m_score);
    
    if (playerHit) {
        handleGameOver();
    } else {
        updateScoreDisplay();
        checkBossTrigger();  // 检查是否触发Boss战
    }
}

void GameScene::shootBullet() {
    if (!m_player) return;
    
    auto bullet = PlayerBullet::create(m_player->getFirePosition());
    if (bullet) {
        this->addChild(bullet, -1);
    }
}

void GameScene::spawnEnemy() {
    auto enemy = Enemy::createAndStart(this);
    if (enemy) {
        this->addChild(enemy);
    }
}

void GameScene::updateScoreDisplay() {
    m_scoreLabel->setString("Score: " + std::to_string(m_score));
}

void GameScene::handleGameOver() {
    if (m_gameState == GameState::GAME_OVER) return;
    
    m_gameState = GameState::GAME_OVER;
    this->pause();
    
    if (m_player) {
        m_player->setGameOverState();
    }
    
    showGameOverUI();
    setupRestartListener();
}

void GameScene::showGameOverUI() {
    auto visibleSize = MathHelper::getVisibleSize();
    
    // 游戏结束大字
    auto gameOverLabel = Label::createWithSystemFont(
        "GAME OVER", 
        "Arial", 
        GameConfig::GAMEOVER_FONT_SIZE
    );
    gameOverLabel->setPosition(
        visibleSize.width / 2, 
        visibleSize.height / 2 + GameConfig::GAMEOVER_Y_OFFSET
    );
    gameOverLabel->setTextColor(Color4B::RED);
    gameOverLabel->enableOutline(Color4B::BLACK, 2);
    this->addChild(gameOverLabel, GameConfig::UI_Z_ORDER);
    
    // 重开提示
    auto restartLabel = Label::createWithSystemFont(
        "Click to Restart", 
        "Arial", 
        GameConfig::RESTART_FONT_SIZE
    );
    restartLabel->setPosition(
        visibleSize.width / 2, 
        visibleSize.height / 2 - GameConfig::GAMEOVER_Y_OFFSET
    );
    restartLabel->setTextColor(Color4B::WHITE);
    restartLabel->enableOutline(Color4B::BLACK, 1);
    restartLabel->runAction(RepeatForever::create(Blink::create(1.0f, 1)));
    this->addChild(restartLabel, GameConfig::UI_Z_ORDER);
}

void GameScene::setupRestartListener() {
    auto restartListener = EventListenerTouchOneByOne::create();
    restartListener->setSwallowTouches(true);
    
    restartListener->onTouchBegan = [](Touch* touch, Event* event) {
        auto newScene = GameScene::createScene();
        Director::getInstance()->replaceScene(
            TransitionFade::create(GameConfig::SCENE_TRANSITION_DURATION, newScene)
        );
        return true;
    };
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(restartListener, this);
}

void GameScene::checkBossTrigger() {
    // 检查是否达到Boss触发分数
    if (!m_bossSpawned && m_score >= GameConfig::BOSS_TRIGGER_SCORE) {
        spawnBoss();
    }
}

void GameScene::spawnBoss() {
    if (m_bossSpawned) return;
    
    m_bossSpawned = true;
    
    // 清除所有普通敌人
    auto enemies = this->getChildren();
    for (auto child : enemies) {
        if (child->getName() == GameConfig::Tags::ENEMY) {
            child->removeFromParent();
        }
    }
    
    // 生成Boss
    m_boss = Boss::createAndStart(this);
    if (m_boss) {
        this->addChild(m_boss, 1);
        
        // 显示Boss警告
        auto visibleSize = MathHelper::getVisibleSize();
        auto warningLabel = Label::createWithSystemFont(
            "WARNING! BOSS APPROACHING!", 
            "Arial", 
            48
        );
        warningLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        warningLabel->setTextColor(Color4B::RED);
        warningLabel->enableOutline(Color4B::BLACK, 2);
        this->addChild(warningLabel, 1000);
        
        // 警告文字闪烁后消失
        auto blink = Blink::create(2.0f, 6);
        auto fadeOut = FadeOut::create(0.5f);
        auto remove = RemoveSelf::create();
        warningLabel->runAction(Sequence::create(blink, fadeOut, remove, nullptr));
    }
}

