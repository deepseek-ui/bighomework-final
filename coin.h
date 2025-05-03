#ifndef COIN_H
#define COIN_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class Coin : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit Coin(QGraphicsItem *parent = nullptr);

signals:
    void collected();
};



#endif // COIN_H
