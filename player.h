#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
#include <string>
#include <QGraphicsPixmapItem>
using namespace std;
struct point{
    int x;
    int y;
};

class Player : public QObject
{
    Q_OBJECT
private:
    QString name;
    point current_position;

public:
    QGraphicsPixmapItem *playerItem, *bullet;
    Player(QString = "DoodleR", point = {0,0});
    ~Player();
    int score;
    void setname(QString);
    QString getname();
    void setcurretn_position(point);
    point getcurrent_position();
};

#endif // PLAYER_H
