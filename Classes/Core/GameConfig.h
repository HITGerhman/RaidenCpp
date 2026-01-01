/****************************************************************************
 * GameConfig.h
 * 
 * 游戏配置常量 - 集中管理所有魔法数字
 * 便于调试和平衡性调整
 ****************************************************************************/

#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

namespace GameConfig {
    // ==================== 玩家配置 ====================
    constexpr float PLAYER_SPEED = 500.0f;          // 玩家移动速度 (像素/秒)
    constexpr float PLAYER_SCALE = 0.5f;            // 玩家精灵缩放比例
    constexpr int PLAYER_START_Y = 100;             // 玩家初始 Y 坐标
    constexpr float PLAYER_COLLISION_WIDTH = 40.0f; // 碰撞体宽度
    constexpr float PLAYER_COLLISION_HEIGHT = 40.0f;// 碰撞体高度
    constexpr float PLAYER_COLLISION_OFFSET_X = 10.0f; // 碰撞体 X 偏移
    constexpr float PLAYER_COLLISION_OFFSET_Y = 10.0f; // 碰撞体 Y 偏移
    
    // ==================== 射击配置 ====================
    constexpr float FIRE_INTERVAL = 0.2f;           // 玩家发射间隔 (秒)
    constexpr float BULLET_SPEED = 1000.0f;         // 子弹速度 (像素/秒)
    constexpr float BULLET_SCALE = 0.5f;            // 子弹缩放
    constexpr float BULLET_OFFSET_Y = 50.0f;        // 子弹生成偏移（从飞机头部射出）
    constexpr float BULLET_COLLISION_SIZE = 20.0f;  // 子弹碰撞体大小
    
    // ==================== 敌人配置 ====================
    constexpr float ENEMY_SPAWN_INTERVAL = 1.0f;    // 敌人生成间隔 (秒)
    constexpr float ENEMY_SCALE = 0.5f;             // 敌人缩放
    constexpr float ENEMY_SPEED = 200.0f;           // 敌人移动速度 (像素/秒)
    constexpr float ENEMY_FIRE_INTERVAL_MIN = 0.2f; // 敌人射击最小间隔 (秒) - 快速射击
    constexpr float ENEMY_FIRE_INTERVAL_MAX = 0.5f; // 敌人射击最大间隔 (秒) - 平均约0.35秒
    constexpr float ENEMY_COLLISION_SIZE = 40.0f;   // 敌人碰撞体大小
    constexpr float ENEMY_SPAWN_MARGIN = 30.0f;     // 敌人生成边距
    constexpr float ENEMY_MOVE_DURATION = 2.0f;     // 敌人飞行时间
    
    // ==================== 敌方子弹配置 ====================
    constexpr float ENEMY_BULLET_SCALE = 0.3f;      // 敌方子弹缩放
    constexpr float ENEMY_BULLET_SPEED = 1000.0f;   // 敌方子弹速度
    constexpr float ENEMY_BULLET_COLLISION_SIZE = 10.0f; // 敌方子弹碰撞体
    
    // ==================== 背景配置 ====================
    constexpr float BACKGROUND_SCROLL_SPEED = 200.0f; // 背景滚动速度 (像素/秒)
    constexpr int BACKGROUND_Z_ORDER = -10;         // 背景层级
    
    // ==================== 游戏规则 ====================
    constexpr int SCORE_PER_KILL = 100;             // 击杀敌人得分
    constexpr int INITIAL_SCORE = 0;                // 初始分数
    
    // ==================== UI 配置 ====================
    constexpr int SCORE_LABEL_FONT_SIZE = 36;       // 分数字体大小
    constexpr int SCORE_LABEL_Y_OFFSET = 40;        // 分数标签距顶部距离
    constexpr int SCORE_LABEL_Z_ORDER = 100;        // 分数标签层级
    
    constexpr int GAMEOVER_FONT_SIZE = 64;          // 游戏结束字体大小
    constexpr int GAMEOVER_Y_OFFSET = 50;           // 游戏结束文字偏移
    constexpr int RESTART_FONT_SIZE = 32;           // 重开提示字体大小
    constexpr int UI_Z_ORDER = 1000;                // UI 最高层级
    
    // ==================== 特效配置 ====================
    constexpr float EXPLOSION_RADIUS = 30.0f;       // 爆炸半径
    constexpr float EXPLOSION_SCALE = 1.5f;         // 爆炸缩放
    constexpr float EXPLOSION_DURATION = 0.1f;      // 爆炸持续时间
    constexpr float SCENE_TRANSITION_DURATION = 0.5f; // 场景切换时间
    
    // ==================== 实体标签名 ====================
    namespace Tags {
        constexpr const char* PLAYER = "Player";
        constexpr const char* BULLET = "Bullet";
        constexpr const char* ENEMY = "Enemy";
        constexpr const char* ENEMY_BULLET = "EnemyBullet";
    }
}

#endif // __GAME_CONFIG_H__

