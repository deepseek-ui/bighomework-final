#include "obstacle.h"
#include <QGraphicsScene>

Obstacle::Obstacle(QGraphicsItem *parent) :
    QObject(nullptr), QGraphicsPixmapItem(parent)
{
    // 加载图片并缩放
    QPixmap originalPixmap(":/obstacle/image/obstacle.png");
    QPixmap scaledPixmap = originalPixmap.scaled(
        360, 360,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    setPixmap(scaledPixmap);

    // 设置碰撞检测
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Obstacle::move);
    timer->start(30);
}

void Obstacle::move()
{
    setPos(x() - 5, y());

    if (x() + pixmap().width() < 0) {
        scene()->removeItem(this);
        delete this;
    }
}
