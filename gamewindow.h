#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QTimer>
#include <QRandomGenerator>
#include <QKeyEvent>

#include "coin.h"
#include "player.h"
#include "obstacle.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr, bool standalone = true);
    ~GameWindow();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    Player *player;
    QTimer *timer;
    QTimer* m_coinSpawnTimer = nullptr;  // 金币生成定时器

    QGraphicsPixmapItem *bgItem1;//背景图
    QGraphicsPixmapItem *bgItem2;

    QList<Coin*>coins;  // 存储金币指针的列表
    QGraphicsTextItem* livesText;//显示生命值
    QGraphicsTextItem* m_coinText = nullptr;  // 金币计数文本

    QRandomGenerator *randomGen;  // 随机数生成器

    int bgScrollSpeed=2;//图片滚动速度
    int nextSpawnInterval;        // 下次生成间隔
    int coinCount = 0;  // 当前蛋糕数
    int scrollSpeed = 2;
    int m_lives = 5;
    int nextObstacleInterval;



    void spawnCoin();
    void spawnObstacle();
    void checkBackgroundLoad(const QPixmap& pixmap, const QString& path);
    void checkCollisions();
    void showFloatingText(const QPointF &position, const QString &text);
    void keyPressEvent(QKeyEvent *event)override;

private slots:
    void update();
   // void spawnCoin();
    //void spawnObstacle();
    //void checkCollisions();

};

#endif // GAMEWINDOW_H
