#include "gamewindow.h"
#include "coin.h"
#include"player.h"
#include <QKeyEvent>
#include <QDebug>
#include <QRandomGenerator>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

GameWindow::GameWindow(QWidget *parent, bool standalone) : QMainWindow(parent)
{
    const int GROUND_Y = 500;

    // 移除默认菜单栏和状态栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    // 创建场景和视图
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
     setCentralWidget(view);  // 将视图设为中心部件

     //视图设置
    view->setRenderHint(QPainter::Antialiasing);
    view->setSceneRect(0, 0, 800, 600);
    view->setFixedSize(800, 600);

    // 加载背景图片
    QPixmap bgPixmap1;
    if(!bgPixmap1.load(":/background/image/background-game1.png")) {
        bgPixmap1 = QPixmap(800, 600);
    }

    QPixmap bgPixmap2;
    if(!bgPixmap2.load(":/background/image/background-game2.png")) {
    }


    // 创建背景图元
    bgItem1 = new QGraphicsPixmapItem(bgPixmap1.scaled(800, 600));
    bgItem2 = new QGraphicsPixmapItem(bgPixmap2.scaled(800, 600));

    // 位置和层级设置
    bgItem1->setPos(0,0);
    bgItem2->setPos(800,0);

    scene->addItem(bgItem1);
    scene->addItem(bgItem2);
    bgItem1->setZValue(-100);
    bgItem2->setZValue(-100);

    //玩家
    player = new Player();
    scene->addItem(player);
    player->setPos(100, GROUND_Y - player->pixmap().height()); // 角色底部接触地面
    player->setgroundY(player->y());
    player->setZValue(100);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();


    //定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::update);
    timer->start(16); // 大约60帧每秒

    // 初始化随机数生成器
    randomGen = QRandomGenerator::global();
    nextSpawnInterval = randomGen->bounded(50, 200);  // 初始随机间隔(50-200帧)

    // 蛋糕生成定时器（每2-5秒生成一个）
    m_coinSpawnTimer = new QTimer(this);
    connect(m_coinSpawnTimer, &QTimer::timeout, this, &GameWindow::spawnCoin);
    m_coinSpawnTimer->start(2000); // 初始2秒生成
 coins = QList<Coin*>();  // 显式初始化
    // 蛋糕计数显示
    m_coinText = new QGraphicsTextItem("蛋糕: 0", nullptr);
    m_coinText->setPos(650, 10);
    m_coinText->setDefaultTextColor(Qt::yellow);
    m_coinText->setFont(QFont("Arial", 16));
    scene->addItem(m_coinText);
     m_coinSpawnTimer = new QTimer(this);

     //生命计数显示
    livesText = new QGraphicsTextItem("体力: 5",nullptr);
     livesText->setPos(650, 40); // 设置显示位置
     livesText->setDefaultTextColor(Qt::red);
     livesText->setFont(QFont("Arial", 16));
     scene->addItem(livesText);

    this->resize(800, 600);



}

GameWindow::~GameWindow()
{
    delete scene;
    delete view;
    delete player;
    delete timer;
    qDeleteAll(coins);
    delete m_coinSpawnTimer;
}

void GameWindow::update()
{
    // 滚动背景逻辑
    bgItem1->setX(bgItem1->x() - bgScrollSpeed);
    bgItem2->setX(bgItem2->x() - bgScrollSpeed);
    if(bgItem1->x() + bgItem1->pixmap().width() <= 0) {
        bgItem1->setX(bgItem2->x() + bgItem2->pixmap().width());
    }
    if(bgItem2->x() + bgItem2->pixmap().width() <= 0) {
        bgItem2->setX(bgItem1->x() + bgItem1->pixmap().width());
    }
    //玩家和障碍物
    player->move();
    spawnObstacle();
    // 移动所有蛋糕
    for(Coin* coin : coins) {
        coin->setX(coin->x() - 3); // 比障碍物慢

        // 移除屏幕外的蛋糕
        if(coin->x() + coin->pixmap().width() < 0) {
            scene->removeItem(coin);
            coins.removeOne(coin);
            delete coin;
        }
    }
     checkCollisions();
}

//障碍物生成逻辑
void GameWindow::spawnObstacle()
{
    static int frameCounter = 0;

    if (++frameCounter >= nextSpawnInterval) {
        // 重置计数器和下次间隔
        frameCounter = 0;
        nextSpawnInterval = randomGen->bounded(250, 500);  // 新随机间隔

        // 创建障碍物
        Obstacle *obstacle = new Obstacle();
        scene->addItem(obstacle);

        // 固定高度
        obstacle->setPos(800,-10);
    }
}


//蛋糕生成逻辑
void GameWindow::spawnCoin()
{
    Coin *coin = new Coin();
    coin->setPos(800, QRandomGenerator::global()->bounded(200, 400));
    scene->addItem(coin);
    coins.append(coin);

    connect(coin, &Coin::collected, this, [this, coin]() {
        scene->removeItem(coin);
        coins.removeOne(coin);
        coin->deleteLater();

        coinCount++;  // 直接加1
       m_coinText->setPlainText(QString("蛋糕: %1").arg(coinCount));

      showFloatingText(coin->pos(), "+1");
    });

}

//碰撞检测
void GameWindow::checkCollisions()
{
    QList<QGraphicsItem*> colliding = player->collidingItems();
    for (QGraphicsItem* item : colliding) {
        if (Coin *coin = dynamic_cast<Coin*>(item)) {
            emit coin->collected();
            break;
        }
        if (Obstacle *obstacle = dynamic_cast<Obstacle*>(item)) {
            m_lives--;
            livesText->setPlainText(QString("体力: %1").arg(m_lives));
            scene->removeItem(obstacle);
            delete obstacle;

            if (m_lives <= 0) {
                timer->stop();
                m_coinSpawnTimer->stop();
                QMessageBox::information(this, "游戏结束", "小八体力已耗尽！");
                close();
            }
            break;
        }
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event){//消耗蛋糕加体力{
    if (event->key() == Qt::Key_Q && coinCount>=10) {
        coinCount-=10;
        m_lives++;
         // 更新UI
        m_coinText->setPlainText(QString("蛋糕: %1").arg(coinCount));
        livesText->setPlainText(QString("体力: %1").arg(m_lives));
    }
    else {
        QMainWindow::keyPressEvent(event);
    }
}
// 显示浮动的 "+1"





void GameWindow::showFloatingText(const QPointF &position, const QString &text)
{
    QGraphicsTextItem *floatingText = new QGraphicsTextItem(text);
    floatingText->setDefaultTextColor(Qt::blue);
    floatingText->setFont(QFont("Arial", 14, QFont::Bold));
    floatingText->setPos(position);
    scene->addItem(floatingText);

    // 向上飘
    QPropertyAnimation *moveAnim = new QPropertyAnimation(floatingText, "y");
    moveAnim->setDuration(1000);
    moveAnim->setStartValue(position.y());
    moveAnim->setEndValue(position.y() - 40);

    // 渐隐效果
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(floatingText);
    floatingText->setGraphicsEffect(opacity);
    QPropertyAnimation *fadeAnim = new QPropertyAnimation(opacity, "opacity");
    fadeAnim->setDuration(1000);
    fadeAnim->setStartValue(1.0);
    fadeAnim->setEndValue(0.0);

    // 动画完成后自动删除
    connect(fadeAnim, &QPropertyAnimation::finished, floatingText, [floatingText]() {
        floatingText->scene()->removeItem(floatingText);
        delete floatingText;
    });

    moveAnim->start(QAbstractAnimation::DeleteWhenStopped);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
}
