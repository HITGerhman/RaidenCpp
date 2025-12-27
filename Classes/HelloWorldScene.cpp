#include "HelloWorldScene.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Init function
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. Draw the Player (Triangle)
    // auto playerNode = DrawNode::create();
    
    // Vec2 vertices[] = {
    //     Vec2(0, 50),
    //     Vec2(-30, -30),
    //     Vec2(30, -30)
    // };
    
    // playerNode->drawSolidPoly(vertices, 3, Color4F::YELLOW);
    // playerNode->setPosition(Vec2(visibleSize.width/2, 100)); // 初始位置放低一点
    // playerNode->setName("Player");
    
    // this->addChild(playerNode);
    // ✅ 新代码 (使用图片)：
    auto playerNode = Sprite::create("player.png"); 
    
    // 如果图片太大，可以缩放一下 (0.5 就是缩小一半)
    playerNode->setScale(0.5f); 

    playerNode->setPosition(Vec2(visibleSize.width/2, 100));
    playerNode->setName("Player");
    this->addChild(playerNode);
    _player = playerNode; // <--- 关键：保存指针！
    // ==========================================
    //  开始：添加触摸/鼠标控制逻辑
    // ==========================================
    
    // 1. 创建一个单点触摸监听器
    auto listener = EventListenerTouchOneByOne::create();
    
    // 2. 设置吞噬触摸（保证触摸事件不会传给下层，虽然现在只有这一层）
    listener->setSwallowTouches(true);

    // 3. 触摸开始 (必须实现，且必须返回 true，否则无法接收后续的 Move 事件)
    listener->onTouchBegan = [](Touch* touch, Event* event){
        return true; 
    };

    // 4. 触摸移动 (核心逻辑在这里)
    listener->onTouchMoved = [=](Touch* touch, Event* event){
        // 获取当前场景
        //auto currentScene = event->getCurrentTarget();
        
        // 通过名字找到我们的主角 (playerNode->setName("Player") )
        //auto player = currentScene->getChildByName("Player");
        
        if (_player)
        {
            // 获取手指/鼠标这一帧移动了多少距离 (Delta)
            Vec2 delta = touch->getDelta();
            
            // 获取主角当前位置
            Vec2 currentPos = _player->getPosition();
            
            // 计算新位置 = 旧位置 + 移动量
            Vec2 newPos = currentPos + delta;
            
            // --- 边界限制 (防止飞出屏幕) ---
            auto visibleSize = Director::getInstance()->getVisibleSize();
            
            // 限制 X 轴 (左右)
            // 0 是左边界，visibleSize.width 是右边界
            if(newPos.x < 0) newPos.x = 0;
            if(newPos.x > visibleSize.width) newPos.x = visibleSize.width;
            
            // 限制 Y 轴 (上下)
            // 0 是下边界，visibleSize.height 是上边界
            if(newPos.y < 0) newPos.y = 0;
            if(newPos.y > visibleSize.height) newPos.y = visibleSize.height;

            // 应用新位置
            _player->setPosition(newPos);
        }
    };

    // 5. 将监听器添加到事件分发器中
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // ==========================================
    // 3. 开启“自动开火”调度器
    // ==========================================
    // 语法：schedule(函数指针, 间隔时间, 重复次数, 延迟)
    // 这里意思是：每隔 0.2 秒调用一次 updateFire 函数
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::updateFire), 0.2f);
    // [新增] 启动敌机生成器：每 1.0 秒召唤一个敌人
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::spawnEnemy), 1.0f);
    // [新增] 开启碰撞检测，update 每一帧都会调用
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::updateCollision));
    CCLOG("Fire System Online!");
    return true;
    // ==========================================
    //  结束
    // ==========================================

    CCLOG("Raiden Game Initialized!");
    return true;

    // 2. Log success
    CCLOG("Raiden Game Started!");

    return true;
}
void HelloWorld::updateFire(float dt)
{
    // 如果主角没了，就不发子弹了
    if (!_player) return;

    // // --- A. 创建子弹 (画一个小黄点) ---
    // auto bullet = DrawNode::create();
    // bullet->drawDot(Vec2::ZERO, 5, Color4F::YELLOW); // 半径5的圆点
    // ✅ 新代码：使用子弹图片
    auto bullet = Sprite::create("bullet.png");
    bullet->setScale(0.5f); // 根据图片大小适当调整
    // // [新增] 给子弹设置名字，这一步至关重要！
    bullet->setName("Bullet");
    
    // 子弹位置 = 飞机当前位置 + 一点点向上的偏移(机头位置)
    Vec2 firePos = _player->getPosition() + Vec2(0, 50);
    bullet->setPosition(firePos);
    
    // 把子弹加到场景里 (注意 z-order 设为 -1，让它在飞机下面生成，更有立体感)
    this->addChild(bullet, -1);

    // --- B. 让子弹飞 (Cocos Action 系统) ---
    
    // 获取屏幕高度，算出子弹要飞多远（飞出屏幕上方）
    float flyHeight = Director::getInstance()->getVisibleSize().height;
    
    // 动作1：移动。0.5秒内，向上飞到屏幕顶端外一点的位置
    // MoveTo 是飞到绝对坐标，MoveBy 是飞相对距离。这里飞出屏幕即可。
    auto moveAction = MoveTo::create(1.0f, Vec2(bullet->getPositionX(), flyHeight + 50));

    // 动作2：销毁。飞出屏幕后，必须把自己删掉，否则内存会爆！
    auto removeAction = RemoveSelf::create();

    // 动作序列：先移动，再销毁
    auto seq = Sequence::create(moveAction, removeAction, nullptr);

    // 执行动作
    bullet->runAction(seq);
}
// [新增] 敌机生成逻辑
void HelloWorld::spawnEnemy(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 1. 画一个敌机 (红色倒三角)
    // auto enemy = DrawNode::create();
    // Vec2 vertices[] = { Vec2(0, -30), Vec2(-20, 20), Vec2(20, 20) }; // 倒过来的三角形
    // enemy->drawSolidPoly(vertices, 3, Color4F::RED); // 红色

    auto enemy = Sprite::create("enemy.png");
    enemy->setScale(0.5f);
    // 给它设个名字，或者 tag，以后碰撞检测要用
    enemy->setName("Enemy");

    // 2. 随机出生位置
    // RandomHelper 是 Cocos 自带的神器
    float randomX = RandomHelper::random_real(30.0f, visibleSize.width - 30.0f);
    
    // 位置：屏幕最上方 (Y = height + 30)，X 是随机的
    enemy->setPosition(Vec2(randomX, visibleSize.height + 30));
    
    this->addChild(enemy);

    // 3. 制定行动路线 (向下飞到底)
    // 目标 Y 值：飞出屏幕下方 (-50)
    float flyTime = 2.0f; // 飞行时间，越小飞得越快
    auto moveAction = MoveTo::create(flyTime, Vec2(randomX, -50));
    
    // 飞完就销毁 (内存回收)
    auto removeAction = RemoveSelf::create();
    
    // 执行动作
    enemy->runAction(Sequence::create(moveAction, removeAction, nullptr));
}
// [新增] 碰撞检测的具体实现
void HelloWorld::updateCollision(float dt)
{
    // 1. 获取场景中所有子节点
    auto children = this->getChildren();

    // 准备两个容器，用来暂存需要被销毁的子弹和敌人
    // (注意：不能在遍历过程中直接 removeChild，会导致迭代器失效崩溃，所以要先记下来)
    std::vector<Node*> bulletsToDelete;
    std::vector<Node*> enemiesToDelete;

    // 2. 遍历所有物体，寻找碰撞
    for (auto child : children)
    {
        // 逻辑 A：子弹打敌人
        if (child->getName() == "Bullet")
        {
            // 💀 修复核心：手动构造子弹的“判定框”
            // 以子弹位置为中心，创造一个 20x20 的矩形
            Rect bulletRect = Rect(
                child->getPositionX() - 10, 
                child->getPositionY() - 10, 
                20, 20
            // 再遍历一次所有物体，找敌人
            );
            for (auto target : children)
            {
                if (target->getName() == "Enemy")
                {
                    // 拿到敌人的包围盒
                    // 💀 修复核心：手动构造敌人的“判定框”
                    // 以敌人位置为中心，创造一个 40x40 的矩形
                    Rect enemyRect = Rect(
                        target->getPositionX() - 20, 
                        target->getPositionY() - 20, 
                        40, 40
                    );

                    // 核心判断：两个矩形是否相交？
                    if (bulletRect.intersectsRect(enemyRect))
                    {
                        // 撞上了！记录下来，稍后删除
                        bulletsToDelete.push_back(child);
                        enemiesToDelete.push_back(target);
                        
                        // 这里可以加一个简单的爆炸特效 (画个橙色圆圈闪一下)
                        auto boom = DrawNode::create();
                        boom->drawDot(Vec2::ZERO, 30, Color4F::ORANGE);
                        boom->setPosition(target->getPosition());
                        this->addChild(boom);
                        // 0.1秒后放大并消失
                        boom->runAction(Sequence::create(
                            ScaleTo::create(0.1f, 1.5f),
                            RemoveSelf::create(),
                            nullptr
                        ));
                    }
                }
            }
        }
        
        // 逻辑 B：敌人撞主角 (简单的 Game Over 判定)
        if (child->getName() == "Enemy" && _player)
        {
             Rect enemyRect = child->getBoundingBox();
             Rect playerRect = _player->getBoundingBox();
             
             // 为了手感好一点，把主角的判定框缩小一点点 (Rect 缩小 10像素)
             playerRect.origin.x += 10;
             playerRect.size.width -= 20;

             if (enemyRect.intersectsRect(playerRect))
             {
                 CCLOG("GAME OVER!");
                 // 简单处理：主角变红，或者直接移除
                 _player->runAction(Blink::create(1.0f, 5)); // 闪烁效果
             }
        }
    }

    // 3. 统一清理战场 (真正执行删除)
    for (auto node : bulletsToDelete) {
        // 加上判断防止重复删除
        if(node->getParent()) node->removeFromParent();
    }
    for (auto node : enemiesToDelete) {
        if(node->getParent()) node->removeFromParent();
    }
}