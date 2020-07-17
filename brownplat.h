#ifndef BROWNPLAT_H
#define BROWNPLAT_H
#include <QTimer>
#include "platform.h"
#include "player.h"
class brownplat:public Platform
{
    Q_OBJECT
private:
    QTimer *browntimer;
public:
    brownplat(point);
    void stepon();
public slots:
    void breakplat();
    void platfall();
};

#endif // BROWNPLAT_H
