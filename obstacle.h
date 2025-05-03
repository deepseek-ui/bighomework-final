#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>                  // 必须包含
#include <QGraphicsPixmapItem>      // 必须包含
#include <QTimer>                   // 必须包含

class Obstacle : public QObject, public QGraphicsPixmapItem  // 多重继承
{
    Q_OBJECT                       // 必须添加宏

public:
    Obstacle(QGraphicsItem *parent = nullptr);  // 修改构造函数

private slots:
    void move();

private:
    QTimer *timer;
};

#endif // OBSTACLE_H
