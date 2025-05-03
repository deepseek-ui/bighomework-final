#include "coin.h"
#include <QPixmap>
#include <QPropertyAnimation>
Coin::Coin(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/coin/image/coin.png").scaled(50, 50));
}
