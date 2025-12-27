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
    // [新增] 初始化变量
    _score = 0;
    _isGameOver = false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 1. 创建第一张背景，放在屏幕中心
    _bg1 = Sprite::create("background.png");
    _bg1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    // 计算缩放比例，让背景填满屏幕宽或高
    // 这里我们简单粗暴地把它拉大一点，保证覆盖全屏
    float scaleX = visibleSize.width / _bg1->getContentSize().width;
    float scaleY = visibleSize.height / _bg1->getContentSize().height;
    // 取较大的缩放比，保证填满
    float finalScale = std::max(scaleX, scaleY);

    // [关键修正] 获取“缩放后的实际高度”
    // 实际高度 = 原始高度 * 缩放比例
    float realHeight = _bg1->getContentSize().height * finalScale;
    _bg1->setScale(finalScale);
   
    this->addChild(_bg1, -10); // Z轴设为 -10，保证在最底层

    // 2. 创建第二张背景，放在第一张的“头顶”
    // 注意：它的 Y 坐标 = 第一张的 Y 坐标 + 图片的高度
    _bg2 = Sprite::create("background.png");
    _bg2->setScale(finalScale);
    _bg2->setPosition(visibleSize.width / 2, _bg1->getPositionY() + realHeight);

    this->addChild(_bg2, -10);

    // 3. 开启背景更新调度器
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::updateBackground));


    // [新增] 创建分数 UI
    // 使用 Arial 字体，字号 36
    _scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 36);
    // 放在屏幕顶部正中间，稍微留点空隙 (height - 40)
    _scoreLabel->setPosition(visibleSize.width / 2, visibleSize.height - 40);
    // 设置颜色为白色
    _scoreLabel->setTextColor(Color4B::WHITE);
    // 设置 Z轴为 100，保证它永远显示在最上层，不会被飞机遮住
    this->addChild(_scoreLabel, 100);
    // 1. Draw the Player 
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
    // [新增] 键盘监听器 (WASD)
    // ==========================================
    auto keyListener = EventListenerKeyboard::create();

    // 当键被按下时：标记为 true
    keyListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event){
        switch(code){
            case EventKeyboard::KeyCode::KEY_W: _isPressingW = true; break;
            case EventKeyboard::KeyCode::KEY_S: _isPressingS = true; break;
            case EventKeyboard::KeyCode::KEY_A: _isPressingA = true; break;
            case EventKeyboard::KeyCode::KEY_D: _isPressingD = true; break;
            default: break;
        }
    };

    // 当键松开时：标记为 false
    keyListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event){
        switch(code){
            case EventKeyboard::KeyCode::KEY_W: _isPressingW = false; break;
            case EventKeyboard::KeyCode::KEY_S: _isPressingS = false; break;
            case EventKeyboard::KeyCode::KEY_A: _isPressingA = false; break;
            case EventKeyboard::KeyCode::KEY_D: _isPressingD = false; break;
            default: break;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    // [重要] 开启移动更新的调度器 (每一帧都去检查按键状态)
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::updatePlayerMovement));
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
// [修正] 敌机生成逻辑
void HelloWorld::spawnEnemy(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto enemy = Sprite::create("enemy.png");
    enemy->setScale(0.5f);
    enemy->setName("Enemy");

    // 随机位置
    float randomX = RandomHelper::random_real(30.0f, visibleSize.width - 30.0f);
    enemy->setPosition(Vec2(randomX, visibleSize.height + 30));
    this->addChild(enemy);

    // --- 动作A：负责移动 ---
    auto moveAction = MoveTo::create(2.0f, Vec2(randomX, -50));
    auto removeAction = RemoveSelf::create();
    auto flySeq = Sequence::create(moveAction, removeAction, nullptr);
    enemy->runAction(flySeq);

    // --- 动作B：负责开火 ---
    auto wait = DelayTime::create(0.3f);
    auto fire = CallFunc::create([=](){
        // 只要飞机还在，就发射子弹
        // 注意：这里调用 createEnemyBullet
        this->createEnemyBullet(enemy->getPosition());
    });
    
    // 组合：等待 -> 开火
    auto fireSeq = Sequence::create(wait, fire, nullptr);
    // 让这个组合无限循环执行 (RepeatForever)
    enemy->runAction(RepeatForever::create(fireSeq));
}
// [新增] 碰撞检测的具体实现
// [修正] 碰撞检测的具体实现
void HelloWorld::updateCollision(float dt)
{
    if (_isGameOver) return;

    auto children = this->getChildren();
    std::vector<Node*> bulletsToDelete;
    std::vector<Node*> enemiesToDelete;

    for (auto child : children)
    {
        // 1. 我方子弹 打 敌人
        if (child->getName() == "Bullet")
        {
            Rect bulletRect = Rect(child->getPositionX()-10, child->getPositionY()-10, 20, 20);
            for (auto target : children)
            {
                if (target->getName() == "Enemy")
                {
                    Rect enemyRect = Rect(target->getPositionX()-20, target->getPositionY()-20, 40, 40);
                    if (bulletRect.intersectsRect(enemyRect))
                    {
                        bulletsToDelete.push_back(child);
                        enemiesToDelete.push_back(target);
                        
                        _score += 100;
                        _scoreLabel->setString("Score: " + std::to_string(_score));

                        auto boom = DrawNode::create();
                        boom->drawDot(Vec2::ZERO, 30, Color4F::ORANGE);
                        boom->setPosition(target->getPosition());
                        this->addChild(boom);
                        boom->runAction(Sequence::create(ScaleTo::create(0.1f, 1.5f), RemoveSelf::create(), nullptr));
                    }
                }
            }
        }
        
        // 2. 敌人 撞 主角
        if (child->getName() == "Enemy" && _player)
        {
             Rect enemyRect = child->getBoundingBox();
             Rect playerRect = _player->getBoundingBox();
             playerRect.origin.x += 10; playerRect.size.width -= 20;

             if (enemyRect.intersectsRect(playerRect))
             {
                 // ✅ 直接调用封装好的函数
                 this->doGameOver(); 
             }
        }

        // 3. 敌方子弹 撞 主角
        if (child->getName() == "EnemyBullet" && _player)
        {
             Rect bulletRect = Rect(child->getPositionX()-5, child->getPositionY()-5, 10, 10);
             Rect playerRect = _player->getBoundingBox();
             playerRect.origin.x += 15; playerRect.size.width -= 30;
             playerRect.origin.y += 10; playerRect.size.height -= 20;

             if (bulletRect.intersectsRect(playerRect))
             {
                 // ✅ 直接调用封装好的函数
                 this->doGameOver();
             }
        }
    }

    for (auto node : bulletsToDelete) { if(node->getParent()) node->removeFromParent(); }
    for (auto node : enemiesToDelete) { if(node->getParent()) node->removeFromParent(); }
}
void HelloWorld::updatePlayerMovement(float dt)
{
    if (!_player) return;

    // 如果没有任何按键按下，直接返回，省电
    if (!_isPressingW && !_isPressingS && !_isPressingA && !_isPressingD) return;

    Vec2 currentPos = _player->getPosition();
    
    // 移动速度 (像素/秒)
    float speed = 500.0f * dt; 

    // 根据按键状态改变位置
    if (_isPressingW) currentPos.y += speed;
    if (_isPressingS) currentPos.y -= speed;
    if (_isPressingA) currentPos.x -= speed;
    if (_isPressingD) currentPos.x += speed;

    // --- 边界限制 (复制之前的逻辑，防止飞出屏幕) ---
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if(currentPos.x < 0) currentPos.x = 0;
    if(currentPos.x > visibleSize.width) currentPos.x = visibleSize.width;
    if(currentPos.y < 0) currentPos.y = 0;
    if(currentPos.y > visibleSize.height) currentPos.y = visibleSize.height;

    _player->setPosition(currentPos);
}
void HelloWorld::updateBackground(float dt)
{
    // 设定滚动速度 (像素/秒)，数值越大滚得越快
    float scrollSpeed = 200.0f;

    // 1. 让两张图都向下移动
    _bg1->setPositionY(_bg1->getPositionY() - scrollSpeed * dt);
    _bg2->setPositionY(_bg2->getPositionY() - scrollSpeed * dt);

    // [关键修正] 动态获取当前的实际高度
    // getScaleY() 会返回你刚才设置的 finalScale
    float realHeight = _bg1->getContentSize().height * _bg1->getScaleY();

    // 2. 检查第一张图是否完全跑出了屏幕下方
    // 如果它的 Y 坐标 <= -(图片高度的一半)，说明它已经完全看不见了
    if (_bg1->getPositionY() <= -realHeight / 2)
    {
        // 瞬移到 _bg2 的头顶 (用实际高度接力)
        _bg1->setPositionY(_bg2->getPositionY() + realHeight);
    }

    if (_bg2->getPositionY() <= -realHeight / 2)
    {
        // 瞬移到 _bg1 的头顶
        _bg2->setPositionY(_bg1->getPositionY() + realHeight);
    }
}
// [新增] 生成敌方子弹的实现
void HelloWorld::createEnemyBullet(Vec2 pos)
{
    // 借用原来的子弹图片
    auto bullet = Sprite::create("bullet.png");
    
    // 区别1：变红！
    bullet->setColor(Color3B::RED); 
    // 区别2：倒过来（弹头朝下）
    bullet->setRotation(180);
    // 区别3：稍微小一点
    bullet->setScale(0.3f);
    
    // 关键：名字叫 EnemyBullet，用于区分
    bullet->setName("EnemyBullet");
    
    bullet->setPosition(pos);
    this->addChild(bullet, -1); // 放在下面

    // 1. 设定子弹速度 (像素/秒)
    // 数值越大，子弹飞得越快。你可以调成 800 或 1000 试试刺激感。
    float velocity = 1000.0f;
    // 2. 计算要飞行的距离
    // 目标 Y 是 -100 (保证完全飞出屏幕外)
    float targetY = -100.0f;
    float distance = pos.y - targetY;

    // 3. 动态计算飞行时间 (时间 = 距离 / 速度)
    float duration = distance / velocity;

    // 子弹向下飞
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // 飞到屏幕最下方外面一点
    auto move = MoveTo::create(duration, Vec2(pos.x, targetY));
    auto remove = RemoveSelf::create();
    
    bullet->runAction(Sequence::create(move, remove, nullptr));
}
// [新增] 游戏结束处理函数
void HelloWorld::doGameOver()
{
    // 1. 如果已经结束了，就别再执行了（防止重复触发）
    if (_isGameOver) return;
    _isGameOver = true;

    CCLOG("GAME OVER!");

    // 2. 核心大招：暂停当前场景！(定格画面)
    this->pause();

    // 3. 主角变红提示
    if (_player) {
        _player->setColor(Color3B::RED);
    }

    // 4. 显示 GAME OVER 大字
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto labelGO = Label::createWithSystemFont("GAME OVER", "Arial", 64);
    labelGO->setPosition(visibleSize.width/2, visibleSize.height/2 + 50); // 稍微靠上
    labelGO->setTextColor(Color4B::RED);
    labelGO->enableOutline(Color4B::BLACK, 2);
    this->addChild(labelGO, 1000);

    // 5. 显示“点击重开”提示文字
    auto labelRestart = Label::createWithSystemFont("Click to Restart", "Arial", 32);
    labelRestart->setPosition(visibleSize.width/2, visibleSize.height/2 - 50); // 稍微靠下
    labelRestart->setTextColor(Color4B::WHITE);
    labelRestart->enableOutline(Color4B::BLACK, 1);
    // 给这个提示加个永远闪烁的动作
    labelRestart->runAction(RepeatForever::create(Blink::create(1.0f, 1))); 
    this->addChild(labelRestart, 1000);

    // ==========================================
    // [核心] 添加“重开”监听器
    // ==========================================
    auto restartListener = EventListenerTouchOneByOne::create();
    restartListener->setSwallowTouches(true); // 吞噬触摸，防止穿透
    
    restartListener->onTouchBegan = [=](Touch* t, Event* e){
        // 一旦点击，执行场景切换
        CCLOG("Restarting Game...");
        
        // 创建一个新的 HelloWorld 场景
        auto newScene = HelloWorld::createScene();
        
        // 使用“淡入淡出”的转场特效来切换 (0.5秒)
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, newScene));
        
        return true;
    };

    // 把监听器加到 EventDispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(restartListener, this);
}