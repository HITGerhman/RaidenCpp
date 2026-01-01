/****************************************************************************
 * BackgroundScroller.cpp
 ****************************************************************************/

#include "BackgroundScroller.h"
#include "Core/GameConfig.h"
#include "Utils/MathHelper.h"

USING_NS_CC;

BackgroundScroller::BackgroundScroller() 
    : m_background1(nullptr)
    , m_background2(nullptr)
    , m_scrollSpeed(GameConfig::BACKGROUND_SCROLL_SPEED)
    , m_realHeight(0.0f) {
}

BackgroundScroller::~BackgroundScroller() {
}

void BackgroundScroller::initialize(Scene* scene) {
    auto visibleSize = MathHelper::getVisibleSize();
    
    // 创建第一张背景
    m_background1 = Sprite::create("background.png");
    m_background1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    
    // 计算缩放比例
    float scaleX = visibleSize.width / m_background1->getContentSize().width;
    float scaleY = visibleSize.height / m_background1->getContentSize().height;
    float finalScale = std::max(scaleX, scaleY);
    
    m_realHeight = m_background1->getContentSize().height * finalScale;
    m_background1->setScale(finalScale);
    scene->addChild(m_background1, GameConfig::BACKGROUND_Z_ORDER);
    
    // 创建第二张背景（拼接在第一张上方）
    m_background2 = Sprite::create("background.png");
    m_background2->setScale(finalScale);
    m_background2->setPosition(visibleSize.width / 2, m_background1->getPositionY() + m_realHeight);
    scene->addChild(m_background2, GameConfig::BACKGROUND_Z_ORDER);
}

void BackgroundScroller::update(float dt) {
    if (!m_background1 || !m_background2) return;
    
    // 向下滚动
    m_background1->setPositionY(m_background1->getPositionY() - m_scrollSpeed * dt);
    m_background2->setPositionY(m_background2->getPositionY() - m_scrollSpeed * dt);
    
    // 循环拼接
    if (m_background1->getPositionY() <= -m_realHeight / 2) {
        m_background1->setPositionY(m_background2->getPositionY() + m_realHeight);
    }
    
    if (m_background2->getPositionY() <= -m_realHeight / 2) {
        m_background2->setPositionY(m_background1->getPositionY() + m_realHeight);
    }
}

