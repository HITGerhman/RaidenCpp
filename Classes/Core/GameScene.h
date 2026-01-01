/****************************************************************************
 * GameScene.h
 * 
 * 游戏主场景 - 雷电射击游戏
 * 重构自 HelloWorldScene，采用模块化架构
 ****************************************************************************/

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Utils/GameTypes.h"

// 前向声明
class Player;
class InputManager;
class BackgroundScroller;

/**
 * @brief 游戏主场景类
 */
class GameScene : public cocos2d::Scene {
public:
    /**
     * @brief 创建场景
     * @return 场景实例
     */
    static cocos2d::Scene* createScene();
    
    /**
     * @brief 初始化场景
     * @return 是否成功
     */
    virtual bool init() override;
    
    CREATE_FUNC(GameScene);
    
private:
    // ==================== 核心对象 ====================
    Player* m_player;
    InputManager* m_inputManager;
    BackgroundScroller* m_backgroundScroller;
    
    // ==================== 游戏状态 ====================
    GameState m_gameState;
    int m_score;
    cocos2d::Label* m_scoreLabel;
    
    // ==================== 初始化方法 ====================
    
    /**
     * @brief 初始化背景
     */
    void initBackground();
    
    /**
     * @brief 初始化玩家
     */
    void initPlayer();
    
    /**
     * @brief 初始化 UI
     */
    void initUI();
    
    /**
     * @brief 初始化输入系统
     */
    void initInput();
    
    /**
     * @brief 启动游戏循环
     */
    void startGameLoop();
    
    // ==================== 更新方法 ====================
    
    /**
     * @brief 更新玩家移动
     * @param dt 时间间隔
     */
    void updatePlayerMovement(float dt);
    
    /**
     * @brief 更新射击
     * @param dt 时间间隔
     */
    void updateFire(float dt);
    
    /**
     * @brief 更新敌人生成
     * @param dt 时间间隔
     */
    void updateSpawnEnemy(float dt);
    
    /**
     * @brief 更新碰撞检测
     * @param dt 时间间隔
     */
    void updateCollision(float dt);
    
    /**
     * @brief 更新背景滚动
     * @param dt 时间间隔
     */
    void updateBackground(float dt);
    
    // ==================== 游戏逻辑 ====================
    
    /**
     * @brief 发射玩家子弹
     */
    void shootBullet();
    
    /**
     * @brief 生成敌人
     */
    void spawnEnemy();
    
    /**
     * @brief 更新分数显示
     */
    void updateScoreDisplay();
    
    /**
     * @brief 处理游戏结束
     */
    void handleGameOver();
    
    /**
     * @brief 显示游戏结束 UI
     */
    void showGameOverUI();
    
    /**
     * @brief 设置重开监听器
     */
    void setupRestartListener();
};

#endif // __GAME_SCENE_H__

