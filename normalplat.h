#ifndef NORMALPLAT_H
#define NORMALPLAT_H

#include <QWidget>
#include "platform.h"
#include "player.h"
class Normalplat : public Platform
{
public:
    Normalplat(point);
    void stepon();
};

#endif // NORMALPLAT_H
