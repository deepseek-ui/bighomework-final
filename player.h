#ifndef PLAYER_H
#define PLAYER_H
//#include"gamewindow.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include<QTimer>

class Player :public QObject, public QGraphicsPixmapItem
{ Q_OBJECT
public:
    Player();
    void move();
     void animate();  // 新增动画函数
    int lives() const { return m_lives; }
    int coins() const { return m_coins; }
    void updateLivesDisplay();
    void keyReleaseEvent(QKeyEvent*event);

    enum PlayerState { Running, Jumping, Ducking };
    PlayerState currentState=Running;
    void setgroundY(int y);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

public slots:
    void takeDamage();

signals:
    void livesChanged(int);
    void coinsChanged(int);
    void gameOver();


private:
    qreal velocityY;
    bool isJumping;

    QList<QPixmap> runFrames;  // run的动画帧数组
    QList<QPixmap> duckFrames; // duck的动画帧数组

    QTimer *animationTimer;

    int currentFrame;


    int m_lives = 5;  // 初始5条命
    int m_coins = 0;   // 金币计数

    int animationInterval = 150;

    int groundY=0;

};

#endif // PLAYER_H
