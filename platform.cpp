#include "platform.h"
using namespace std;
Platform::Platform(int t, point p, QString str)
{
    plattype = t;
    position = p;
    platItem = new QGraphicsPixmapItem;
    platItem->setPixmap(QPixmap(str).scaled(QPixmap(":/res/p-green.png").size()*1.1));
    platItem->setPos(position.x, position.y);
}
void Platform::setposition(point p)
{
    position = p;
}
point Platform::getposition()
{
    return position;
}
