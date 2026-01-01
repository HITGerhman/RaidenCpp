/****************************************************************************
 * InputManager.h
 * 
 * 输入管理器 - 统一管理键盘和触摸输入
 ****************************************************************************/

#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "cocos2d.h"
#include "Utils/GameTypes.h"

// 前向声明
class Player;

class InputManager {
public:
    InputManager();
    ~InputManager();
    
    /**
     * @brief 初始化输入监听器
     * @param scene 场景对象
     * @param player 玩家对象
     */
    void initialize(cocos2d::Scene* scene, Player* player);
    
    /**
     * @brief 获取当前输入状态
     * @return 输入状态
     */
    const InputState& getInputState() const { return m_inputState; }
    
private:
    InputState m_inputState;
    Player* m_player;
    
    /**
     * @brief 设置键盘监听器
     * @param scene 场景对象
     */
    void setupKeyboardListener(cocos2d::Scene* scene);
    
    /**
     * @brief 设置触摸监听器
     * @param scene 场景对象
     */
    void setupTouchListener(cocos2d::Scene* scene);
};

#endif // __INPUT_MANAGER_H__

