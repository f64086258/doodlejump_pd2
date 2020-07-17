#ifndef PLATFORM_H
#define PLATFORM_H

//#include <QObject>
//#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "player.h"

class Platform : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
protected:
    point position;

public:
    QGraphicsPixmapItem *platItem;
    int plattype, state = 0;
    Platform(int, point, QString);
    void setposition(point);
    point getposition();
    virtual void stepon() = 0;
signals:

public slots:
};

#endif // PLATFORM_H
