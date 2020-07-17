#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include "platform.h"
#include "normalplat.h"
#include "player.h"
#include "brownplat.h"
#include <QGraphicsView>
#include <QTime>
#include <QTimer>
#define PLATSIZE 15
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *e)
    {
        if(!gameopen)
        {
            int mx = e->x(), my = e->y();
            if(mx > 63 && mx < 203 && my > 140 && my < 185)
            {
                emit clicked();
            }
        }
        if(gameopen && !gamepause && !gameover)
        {
            int mx = e->x(), my = e->y();
            if(mx > 370 && mx < 400 && my > 4 && my < 40)
            {
                emit paused();
            }
        }
        if(gamepause)
        {
            int mx = e->x(), my = e->y();
            if(mx > 200 && mx < 340 && my > 400 && my < 445)
            {
                emit resumed();
            }
        }
    }
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
signals:
    void clicked();
    void paused();
    void resumed();
public slots:
    void setmenu();
    void startgame();
    void doodlefly();
    void doodleflyup();
    void doodleleft();
    void doodleright();
    void gameoverfall();
    void monstermoveleft();
    void monstermoveright();
    void holegameover();
    void rocketfly();
    void capfly();
    void pause();
    void resume();
    void shoot();
private:
    Ui::MainWindow *ui;
    QWidget *mainwidget = nullptr;
    Player *doodleplayer = nullptr;
    Platform **plat;
    QGraphicsScene *scene = nullptr;
    QGraphicsView *graphicsView = nullptr;
    QGraphicsPixmapItem *gameoverItem = nullptr, *monsterItem = nullptr, *pauseback = nullptr, *resumeItem = nullptr, *holeItem = nullptr, *rocket = nullptr, *cap = nullptr;
    QGraphicsTextItem *scoreItem = nullptr, *pauseItem = nullptr, *finalscore = nullptr;
    QTimer *timer = nullptr;
    int timecount = 0;
    double currentplat; //playersize = 0;
    int falltimecount = 0,lcount = 0, rcount = 0, browncount = 0, brownfallcount = 0, propcount = 0, rocketpropcount = 0;
    int face = 0;
    int scoreincrement = 0;
    bool iskey = false, gameover = false, gameopen = false, isshoot = false, scoreup = true, ismonster = false, gamepause = false, ishole = false;
    bool isrocket = false, iscap = false, equiperocket = false, equipecap = false;
};

#endif // MAINWINDOW_H
