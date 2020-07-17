#include "player.h"

Player::Player(QString n, point p)
{
    name = n;
    playerItem = new QGraphicsPixmapItem(QPixmap(":/res/doodle_right.png").scaled(QPixmap(":/res/doodleR.png").size()));
    bullet = new QGraphicsPixmapItem(QPixmap(":/res/bullet.png"));
    score = 0;
    current_position = p;
    playerItem->setPos(p.x, p.y);
}
Player::~Player()
{
    delete playerItem;
    delete bullet;
}
void Player::setname(QString n)
{
    name = n;
}
QString Player::getname()
{
    return name;
}
void Player::setcurretn_position(point p)
{
    current_position = p;
}
point Player::getcurrent_position()
{
    return current_position;
}

