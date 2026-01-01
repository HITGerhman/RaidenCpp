/****************************************************************************
 * InputManager.cpp
 ****************************************************************************/

#include "InputManager.h"
#include "Entities/Player.h"

USING_NS_CC;

InputManager::InputManager() : m_player(nullptr) {
}

InputManager::~InputManager() {
}

void InputManager::initialize(Scene* scene, Player* player) {
    m_player = player;
    setupKeyboardListener(scene);
    setupTouchListener(scene);
}

void InputManager::setupKeyboardListener(Scene* scene) {
    auto keyListener = EventListenerKeyboard::create();
    
    keyListener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event* event) {
        switch (code) {
            case EventKeyboard::KeyCode::KEY_W:
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                m_inputState.up = true;
                break;
            case EventKeyboard::KeyCode::KEY_S:
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                m_inputState.down = true;
                break;
            case EventKeyboard::KeyCode::KEY_A:
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                m_inputState.left = true;
                break;
            case EventKeyboard::KeyCode::KEY_D:
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                m_inputState.right = true;
                break;
            default:
                break;
        }
    };
    
    keyListener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event) {
        switch (code) {
            case EventKeyboard::KeyCode::KEY_W:
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                m_inputState.up = false;
                break;
            case EventKeyboard::KeyCode::KEY_S:
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                m_inputState.down = false;
                break;
            case EventKeyboard::KeyCode::KEY_A:
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                m_inputState.left = false;
                break;
            case EventKeyboard::KeyCode::KEY_D:
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                m_inputState.right = false;
                break;
            default:
                break;
        }
    };
    
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, scene);
}

void InputManager::setupTouchListener(Scene* scene) {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    touchListener->onTouchBegan = [](Touch* touch, Event* event) {
        return true;
    };
    
    touchListener->onTouchMoved = [this](Touch* touch, Event* event) {
        if (m_player) {
            Vec2 delta = touch->getDelta();
            m_player->moveByDelta(delta);
        }
    };
    
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, scene);
}

