#include "player.h"
#include "gamewindow.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsEffect>


Player::Player() : velocityY(0), isJumping(0), currentState(Running),currentFrame(-1)
{
    // 加载动画帧-跑
    for(int i=1; i<=4; ++i) {
        QString p = (QString(":/run/image/run/fly(%1).png").arg(i));
        QPixmap frame(p);
        runFrames.append(frame);
    }

    //加载动画帧-趴
    for (int i = 1; i <= 2; ++i) {
        QString p=(QString(":/duck/image/duck/liedown(%1).png").arg(i));
        QPixmap frame(p);
        duckFrames.append(frame);
    }


    // 动画定时器
    animationTimer = new QTimer(nullptr);
    connect(animationTimer, &QTimer::timeout, this, &Player::animate);
   QTimer::singleShot(150, this, &Player::animate);

    animate();
   setScale(0.7);//角色太大了，缩小一下
}


void Player::move()
{

    const float GRAVITY = 0.8f;
    const int GROUND_LEVEL = 500 - pixmap().height();

    if (isJumping) {
        velocityY += GRAVITY;
        setPos(x(), y() + velocityY);
    }
  if (y() >= GROUND_LEVEL) {
      setPos(x(), GROUND_LEVEL);
      if (isJumping || currentState != Running) {
          isJumping = false;
          velocityY = 0;
          currentState = Running;
          animationTimer->setInterval(150);
          currentFrame = -1;
          animate();
      }
   }
}

void Player::animate()
{
    ++currentFrame;
    switch (currentState) {
    case Running:
        if (!runFrames.isEmpty()){
            setPixmap(runFrames[currentFrame % runFrames.size()]);     

            update();
        }
        animationInterval = 150;
        break;

    case Ducking:
        if (!duckFrames.isEmpty()){
            setPixmap(duckFrames[currentFrame % duckFrames.size()]);

             update();

        }
         animationInterval = 200;
        break;
    case Jumping:
        if (!runFrames.isEmpty()){
            setPixmap(runFrames[currentFrame % runFrames.size()]);

             update();
        }
        animationInterval = 100;
        break;
    }
    QTimer::singleShot(animationInterval, this, &Player::animate);
}


void Player::keyPressEvent(QKeyEvent *event)
{
    //W起跳
    if (event->key() == Qt::Key_W && !isJumping) {
        isJumping = true;
        velocityY = -15;
        currentState = Jumping;
        // 跳跃时改变动画速度
        animationTimer->setInterval(120);  // 加速动画
        animate();
    }
    //S趴下
    if (event->key() == Qt::Key_S&&!isJumping) {

        currentState = Ducking;
        animationTimer->setInterval(200); // 趴下帧慢点
        setPos(x(), groundY + 55);//趴下时位置稍微低一点
        animate();
    }

    QGraphicsPixmapItem::keyPressEvent(event); // 传递事件
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_S && currentState == Ducking) {
        currentState = Running;
        animationTimer->setInterval(150); // 恢复跑步动画速度
    }
    QGraphicsPixmapItem::keyReleaseEvent(event);
}





void Player::takeDamage()
{
    if(--m_lives <= 0) {
        emit gameOver();
    }
    emit livesChanged(m_lives);
}
void Player::setgroundY(int y){
    groundY=y;
}


void Player::focusInEvent(QFocusEvent *event)
{
    setFocus(); // 确保获得焦点
    QGraphicsPixmapItem::focusInEvent(event);
}

