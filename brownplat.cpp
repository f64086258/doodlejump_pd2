#include "brownplat.h"

brownplat::brownplat(point p):Platform (1, p, ":/res/brown_1.png")
{
    browntimer = new QTimer;
}

void brownplat::stepon()
{
    browntimer->start(100);
    connect(browntimer, SIGNAL(timeout()), this, SLOT(breakplat()));
}
void brownplat::breakplat()
{
        if(platItem->y()<510)
        {
            if(state == 0)
            {
                state++;
                platItem->setPixmap(QPixmap(":/res/brown_2.png").scaled(QPixmap(":/res/p-green.png").size()*1.1));
            }
            else if (state == 1) {
                state++;
                platItem->setPixmap(QPixmap(":/res/brown_3.png").scaled(QPixmap(":/res/p-green.png").size()*1.1));
            }
            else if (state == 2) {
                state++;
                platItem->setPixmap(QPixmap(":/res/brown_4.png").scaled(QPixmap(":/res/p-green.png").size()*1.1));
            }
            else if (state == 3) {
                state++;
                disconnect(browntimer, SIGNAL(timeout()), this, SLOT(breakplat()));
                platItem->setZValue(platItem->zValue()+1);
                connect(browntimer, SIGNAL(timeout()), this, SLOT(platfall()));
            }
        }
        else {
            state = 0;
            disconnect(browntimer, SIGNAL(timeout()), this, SLOT(breakplat()));
        }
}
void brownplat::platfall()
{
    platItem->setY(platItem->y()+30);
    if(platItem->y()>510)
    {
        browntimer->stop();
        platItem->setZValue(platItem->zValue()-1);
        disconnect(browntimer, SIGNAL(timeout()), this, SLOT(platfall()));
        platItem->setPixmap(QPixmap(":/res/brown_1.png").scaled(QPixmap(":/res/p-green.png").size()*1.1));
        state = 0;
    }
}
