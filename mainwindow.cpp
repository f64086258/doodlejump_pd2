#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QGridLayout>
//#include <QLayout>
//#include <QPalette>
//#include <QLayout>
//#include <QDebug>
#include <QGraphicsTextItem>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0, 0, 400, 533))
{
    ui->setupUi(this);
    mainwidget = new QWidget;
    setCentralWidget(mainwidget);
    graphicsView = new QGraphicsView(mainwidget);
    graphicsView->setFocusPolicy(Qt::NoFocus);
    graphicsView->setScene(scene);
    setmenu();
}
void MainWindow::setmenu()
{
    scene->clear();
    setFixedSize(400, 533);
    gameopen = false;
    gameover = false;
    doodleplayer = new Player;
    scoreItem = new QGraphicsTextItem;
    gameoverItem = new QGraphicsPixmapItem;
    finalscore = new QGraphicsTextItem;
    pauseItem = new QGraphicsTextItem;
    plat = new Platform*[PLATSIZE];
    for(int i = 0; i < PLATSIZE; i++)
    {
        plat[i] = nullptr;
    }

    QGraphicsPixmapItem *back = new QGraphicsPixmapItem;
    back->setPixmap(QPixmap(":/res/atlas3.png 2.jpg").scaled(400,533));
    back->setPos(0,0);
    scene->addItem(back);
    QGraphicsPixmapItem *playbutton = new QGraphicsPixmapItem(QPixmap(":/res/play-button.png").scaled(140, 45));
    playbutton->setPos(63, 140);
    scene->addItem(playbutton);
    connect(this, SIGNAL(clicked()), this, SLOT(startgame()));
}
MainWindow::~MainWindow()
{
    delete ui;
    delete  doodleplayer;
    for(int i = 0; i < PLATSIZE; i++)
    {
        delete plat[i];
    }
    delete [] plat;

    delete gameoverItem;
    delete scoreItem;
    delete finalscore;
    delete monsterItem;
    delete rocket;
    delete cap;
    delete holeItem;
    delete pauseItem;
    delete timer;
    delete scene;
    delete graphicsView;
    delete  mainwidget;

}
void MainWindow::startgame()
{
    rcount = 0;
    lcount = 0;
    timecount = 0;
    falltimecount = 0;
    propcount = 0;
    rocketpropcount = 0;
    iskey=false;
    disconnect(this, SIGNAL(clicked()), this, SLOT(startgame()));
    disconnect(this, SIGNAL(clicked()), this, SLOT(setmenu()));
    gameopen = true;
    gameover=false;
    gamepause=false;
    isshoot = false;
    ismonster = false;
    ishole = false;
    isrocket= false;
    iscap = false;
    equipecap = false;
    equiperocket = false;
    qsrand(uint(QTime::currentTime().msec()));
    timer = new QTimer;
    timer->start(10);
    scene->clear();
    QGraphicsPixmapItem *back = new QGraphicsPixmapItem;
    back->setPixmap(QPixmap(":/res/atlas2.png.jpg").scaled(400,533));
    back->setZValue(-10);
    scene->addItem(back);
    QGraphicsPixmapItem *topbar = new QGraphicsPixmapItem;
    topbar->setPixmap(QPixmap(":/res/topbar.png"));
    topbar->setZValue(12);
    scene->addItem(topbar);
    scoreItem->setPlainText(QString::number(doodleplayer->score));
    scoreItem->setFont(QFont("Doodle Jump", 22, 75));
    scoreItem->setZValue(12);
    scoreItem->setPos(0,-4);
    scene->addItem(scoreItem);
    pauseItem->setPlainText("| |");
    pauseItem->setFont(QFont("Doodle Jump", 12, 75));
    pauseItem->setZValue(12);
    pauseItem->setPos(370, 5);
    scene->addItem(pauseItem);
    connect(this, SIGNAL(paused()), this, SLOT(pause()));

    plat[0] = new Normalplat({200-QPixmap(":/res/p-green.png").width()/2, 500-QPixmap(":/res/p-green.png").height()});
    currentplat = plat[0]->platItem->y();
    scene->addItem(plat[0]->platItem);
    int render;
    for(int i = 1; i < PLATSIZE; i++)
    {
        render = qrand()%50 + 1;
        bool repeat = false;
        int px, py;
        px = qrand()%(400-QPixmap(":/res/p-green.png").width());
        py = qrand()%(500-QPixmap(":/res/p-green.png").height());
        do
        {
            repeat = false;

            for(int j = 0; j < i; j++)
            {
                if(px>=plat[j]->platItem->x()&&px<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py>=plat[j]->platItem->y()&&py<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height())
                {
                    px+=plat[j]->platItem->pixmap().width();
                    py-=plat[j]->platItem->pixmap().height();
                    repeat=true;
                }
                else if(px+plat[j]->platItem->pixmap().width()>=plat[j]->platItem->x()&&px+plat[j]->platItem->pixmap().width()<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py+plat[j]->platItem->pixmap().height()>=plat[j]->platItem->y()&&py+plat[j]->platItem->pixmap().height()<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height())
                {
                    px+=plat[j]->platItem->pixmap().width();
                    py-=plat[j]->platItem->pixmap().height();
                    repeat=true;
                }
                else if (px>=plat[j]->platItem->x()&&px<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py+plat[j]->platItem->pixmap().height()>=plat[j]->platItem->y()&&py+plat[j]->platItem->pixmap().height()<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height()) {
                    px+=plat[j]->platItem->pixmap().width();
                    py-=plat[j]->platItem->pixmap().height();
                    repeat=true;
                }
                else if (px+plat[j]->platItem->pixmap().width()>=plat[j]->platItem->x()&&px+plat[j]->platItem->pixmap().width()<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py>=plat[j]->platItem->y()&&py<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height()) {

                    px+=plat[j]->platItem->pixmap().width();
                    py-=plat[j]->platItem->pixmap().height();
                    repeat=true;
                }
                if(px>400-plat[j]->platItem->pixmap().width())
                {
                    px = qrand()%(400-QPixmap(":/res/p-green.png").width());
                    repeat=true;
                }
                if(py<0)
                {
                    py = qrand()%(500-QPixmap(":/res/p-green.png").height());
                    repeat=true;
                }
            }
        }while(repeat);
        if(render >= 1 && render <= 10)
        {
            plat[i] = new brownplat({px,py});
        }
        else {
            plat[i] = new Normalplat({px,py});
        }
        scene->addItem(plat[i]->platItem);
    }
    doodleplayer = new Player("DoodleR", {plat[0]->getposition().x +QPixmap(":/res/p-green.png").width()/2- QPixmap(":/res/doodleR.png").width()/2, plat[0]->getposition().y-QPixmap(":/res/doodleR.png").height()});
    doodleplayer->playerItem->setZValue(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
    scene->addItem(doodleplayer->playerItem);

}
void MainWindow::pause()
{
    gamepause = true;
    pauseback = new QGraphicsPixmapItem(QPixmap(":/res/atlas3.png.jpg").scaled(400,533));
    pauseback->setZValue(11);
    scene->addItem(pauseback);
    resumeItem = new QGraphicsPixmapItem(QPixmap(":/res/atlas.png").scaled(140,45));
    resumeItem->setPos(200, 400);
    resumeItem->setZValue(11);
    scene->addItem(resumeItem);
    connect(this, SIGNAL(resumed()), this, SLOT(resume()));
}
void MainWindow::resume()
{
    scene->removeItem(pauseback);
    delete pauseback;
    scene->removeItem(resumeItem);
    delete resumeItem;
    gamepause = false;
    disconnect(this, SIGNAL(resumed()), this, SLOT(resume()));
}
void MainWindow::doodlefly()
{
    if(!gamepause)
    {
    if(timecount == 0)
    {
        if(falltimecount == 0)
        {
            if(isshoot)
            {
                doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodleshoot.png").scaled(40, QPixmap(":/res/doodleR.png").height()));
            }
            else if(!face)
            {
                doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodle_right.png").scaled(QPixmap(":/res/doodleR.png").size()));

            }
            else {
                doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodle_left.png").scaled(QPixmap(":/res/doodleR.png").size()));

            }
        }
        falltimecount++;
        doodleplayer->playerItem->setPos(doodleplayer->playerItem->x(), doodleplayer->playerItem->y()+6*falltimecount*0.01);
    }
    if(!gameover)
    {
        for(int i = 0; i < PLATSIZE; i++)
        {
            if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>plat[i]->platItem->x())&&
                    (doodleplayer->playerItem->x()<(plat[i]->platItem->x()+plat[i]->platItem->pixmap().width()))&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>plat[i]->platItem->y())&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(plat[i]->platItem->y()+plat[i]->platItem->pixmap().height()))&&timecount==0&&falltimecount>1)
            {
                if(plat[i]->plattype == 0)
                {
                    if(isshoot)
                    {
                        doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodleshootjump.png").scaled(40, QPixmap(":/res/doodleR.png").height()));
                    }
                    else if(!face)
                    {
                        doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodle_rightjump.png").scaled(QPixmap(":/res/doodleR.png").size()));
                    }
                    else {
                        doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodle_leftjump.png").scaled(QPixmap(":/res/doodleR.png").size()));
                    }
                    if(plat[i]->platItem->y() < currentplat)
                    {
                        scoreincrement = 145-(currentplat - plat[i]->platItem->y());
                        currentplat = plat[i]->platItem->y();
                        scoreup = true;
                    }
                    connect(timer, SIGNAL(timeout()), this, SLOT(doodleflyup()));
                }

                plat[i]->stepon();
            }
        }
        if(ismonster)
        {
            if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>monsterItem->x())&&
                    (doodleplayer->playerItem->x()<(monsterItem->x()+monsterItem->pixmap().width()))&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>monsterItem->y())&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(monsterItem->y()+monsterItem->pixmap().height())))
            {
                gameover = true;
            }
        }
        if(ishole)
        {
            if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>holeItem->x())&&
                    (doodleplayer->playerItem->x()<(holeItem->x()+holeItem->pixmap().width()))&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>holeItem->y())&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(holeItem->y()+holeItem->pixmap().height())))
            {
                gameover = true;
                lcount = 0;
                rcount = 0;
                falltimecount = 0;
                disconnect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
                disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleleft()));
                disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleright()));
                connect(timer, SIGNAL(timeout()), this, SLOT(holegameover()));
            }
        }
        if(iscap && propcount == 0 && rocketpropcount == 0)
        {
            if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>cap->x())&&
                    (doodleplayer->playerItem->x()<(cap->x()+cap->pixmap().width()))&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>cap->y())&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(cap->y()+cap->pixmap().height())) && !equipecap && !equiperocket && !isshoot)
            {
                equipecap = true;
                scoreup = true;
                cap->setZValue(doodleplayer->playerItem->zValue()+1);
                cap->setPixmap(QPixmap(":/res/cap/cap1.png").scaled(50, 30));
                if(!face)
                    cap->setPos(doodleplayer->playerItem->x()-5, doodleplayer->playerItem->y()+20-cap->pixmap().height());
                else {
                    cap->setPos(doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-cap->pixmap().width()+5, doodleplayer->playerItem->y()+20-cap->pixmap().height());
                }
                timecount=0;
                falltimecount = 0;
                disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleflyup()));
                disconnect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
                connect(timer, SIGNAL(timeout()), this, SLOT(capfly()));

            }
        }
        if(isrocket && rocketpropcount == 0 && propcount == 0)
        {
            if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>rocket->x())&&
                    (doodleplayer->playerItem->x()<(rocket->x()+rocket->pixmap().width()))&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>rocket->y())&&
                    (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(rocket->y()+rocket->pixmap().height())) && !equipecap && !equiperocket && !isshoot)
            {
                equiperocket = true;
                scoreup = true;
                rocket->setZValue(doodleplayer->playerItem->zValue()+1);
                if(!face)
                {
                    rocket->setPixmap(QPixmap(":/res/rocket/rleft.png").scaled(56, 100));
                    rocket->setPos(doodleplayer->playerItem->x()-rocket->pixmap().width()+15, doodleplayer->playerItem->y()-10);
                }
                else {
                    rocket->setPixmap(QPixmap(":/res/rocket/rrightbig.png").scaled(56, 100));
                    rocket->setPos(doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-15, doodleplayer->playerItem->y()-10);
                }
                timecount=0;
                falltimecount = 0;
                disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleflyup()));
                disconnect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
                connect(timer, SIGNAL(timeout()), this, SLOT(rocketfly()));

            }
        }
    }
    if(doodleplayer->playerItem->x()<0-doodleplayer->playerItem->pixmap().width())
    {
        doodleplayer->playerItem->setX(400);
    }
    if(doodleplayer->playerItem->x()>400)
    {
        doodleplayer->playerItem->setX(0-doodleplayer->playerItem->pixmap().width());
    }
    if(doodleplayer->playerItem->y()>500)
    {
            gameover = true;
            disconnect(this, SIGNAL(paused()), this, SLOT(pause()));
            for(int i = 0; i < PLATSIZE; i++)
            {
                scene->removeItem(plat[i]->platItem);
            }
            if(ismonster)
            {
                ismonster = false;
                scene->removeItem(monsterItem);
            }
            if(ishole)
            {
                ishole = false;
                scene->removeItem(holeItem);
            }
            if(isrocket)
            {
                isrocket = false;
                scene->removeItem(rocket);
            }
            if(iscap)
            {
                iscap = false;
                scene->removeItem(cap);
            }
            doodleplayer->playerItem->setY(0);
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
            gameoverItem = new QGraphicsPixmapItem;
            gameoverItem->setPixmap(QPixmap(":/res/menu/gameover0.png").scaled(400,533));
            scene->addItem(gameoverItem);
            gameoverItem->setY(533);
            finalscore = new QGraphicsTextItem;
            finalscore->setPlainText(QString::number(doodleplayer->score));
            finalscore->setFont(QFont("Doodle Jump", 30, 75));
            finalscore->setPos(210, 860);
            scene->addItem(finalscore);
            connect(timer, SIGNAL(timeout()), this, SLOT(gameoverfall()));

    }
    }
}
void MainWindow::gameoverfall()
{
    doodleplayer->playerItem->setPos(doodleplayer->playerItem->x(), doodleplayer->playerItem->y()+4.9*falltimecount*0.01);
    if(gameoverItem->y()>0)
    {
        gameoverItem->setY(gameoverItem->y()-4.9*falltimecount*0.01);
        finalscore->setY(finalscore->y()-4.9*falltimecount*0.01);
    }
    if(doodleplayer->playerItem->x()<0)
    {
        doodleplayer->playerItem->setX(400);
    }
    if(doodleplayer->playerItem->x()>400)
    {
        doodleplayer->playerItem->setX(0);
    }
    if(gameoverItem->y()<=0 && doodleplayer->playerItem->y()>533)
    {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(gameoverfall()));
        disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleleft()));
        disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleright()));
        timer->stop();
        scene->removeItem(doodleplayer->playerItem);
        connect(this, SIGNAL(clicked()), this, SLOT(setmenu()));
        gameopen = false;
    }
    else if(doodleplayer->playerItem->y()>533)
    {
        doodleplayer->playerItem->setY(0);
    }
}
void MainWindow::doodleflyup()
{
    if(!gamepause)
    {
    falltimecount=0;
    timecount++;

    if((4.3-4.9*timecount*0.01)>0)
    {
        doodleplayer->playerItem->setPos(doodleplayer->playerItem->x(), doodleplayer->playerItem->y()-(4.3-4.9*timecount*0.01));
        if(scoreup)
        {
            doodleplayer->score += (4.3-4.9*timecount*0.01-scoreincrement/87);
            scoreItem->setPlainText(QString::number(doodleplayer->score));
        }
    }
    if(doodleplayer->playerItem->y()<250)
    {
        doodleplayer->playerItem->setY(250);
        currentplat += 4.3-4.9*timecount*0.01;
        if(currentplat>500)
            currentplat = 500;
        int dice = qrand()%1000;
        if(dice == 40)
        {
            if(!ismonster)
            {
                ismonster = true;
                monsterItem = new QGraphicsPixmapItem;
                monsterItem->setPixmap(QPixmap(":/res/monsters/bat1.png").scaled(QPixmap(":/res/monsters/bat1.png").size()*0.8));
                monsterItem->setPos(qrand()%(400-monsterItem->pixmap().width()), 0);
                monsterItem->setZValue(10);
                scene->addItem(monsterItem);
                connect(timer, SIGNAL(timeout()), this, SLOT(monstermoveright()));
            }
        }
        else if(dice == 50)
        {
            if(!ishole)
            {
                ishole = true;
                holeItem = new QGraphicsPixmapItem;
                holeItem->setPixmap(QPixmap(":/res/hole.png").scaled(QPixmap(":/res/hole.png").size()*0.6));
                holeItem->setPos(qrand()%(400-holeItem->pixmap().width()), 0);
                holeItem->setZValue(5);
                scene->addItem(holeItem);
            }
        }

        if(ismonster)
        {
            monsterItem->setY(monsterItem->y()+4.3-4.9*timecount*0.01);
            if(monsterItem->y()>500)
            {
                scene->removeItem(monsterItem);
                delete monsterItem;
                monsterItem = nullptr;
                ismonster = false;
            }
        }
        if(ishole)
        {
            holeItem->setY(holeItem->y()+4.3-4.9*timecount*0.01);
            if(holeItem->y()>500)
            {
                scene->removeItem(holeItem);
                delete holeItem;
                holeItem = nullptr;
                ishole = false;
            }
        }
        if(isrocket && rocketpropcount == 0)
        {
            rocket->setY(rocket->y()+4.3-4.9*timecount*0.01);
            if(rocket->y()>500)
            {
                scene->removeItem(rocket);
                delete rocket;
                rocket = nullptr;
                isrocket = false;
            }
        }
        if(iscap && propcount == 0)
        {
            cap->setY(cap->y()+4.3-4.9*timecount*0.01);
            if(cap->y()>500)
            {
                scene->removeItem(cap);
                delete cap;
                cap = nullptr;
                iscap = false;
            }
        }
        int render;
        for(int i = 0; i < PLATSIZE; i++)
        {

            plat[i]->platItem->setY(plat[i]->platItem->y()+4.3-4.9*timecount*0.01);
            if(plat[i]->plattype == 0 || (plat[i]->plattype == 1 && plat[i]->state == 0))
            {
            if(plat[i]->platItem->y()>500)
            {
                render = qrand()%50 + 1;
                bool repeat = false;
                int px, py;
                px = qrand()%(400-QPixmap(":/res/p-green.png").width());
                py = 0;
                do
                {
                    repeat = false;

                    for(int j = 0; j < PLATSIZE; j++)
                    {
                        if(j == i)
                            continue;
                        if(px>=plat[j]->platItem->x()&&px<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py>=plat[j]->platItem->y()&&py<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height())
                        {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if(px+plat[j]->platItem->pixmap().width()>=plat[j]->platItem->x()&&px+plat[j]->platItem->pixmap().width()<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py+plat[j]->platItem->pixmap().height()>=plat[j]->platItem->y()&&py+plat[j]->platItem->pixmap().height()<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height())
                        {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if (px>=plat[j]->platItem->x()&&px<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py+plat[j]->platItem->pixmap().height()>=plat[j]->platItem->y()&&py+plat[j]->platItem->pixmap().height()<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height()) {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if (px+plat[j]->platItem->pixmap().width()>=plat[j]->platItem->x()&&px+plat[j]->platItem->pixmap().width()<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py>=plat[j]->platItem->y()&&py<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height()) {

                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        if(px>400-plat[j]->platItem->pixmap().width())
                        {
                            px = qrand()%(400-QPixmap(":/res/p-green.png").width());
                            repeat=true;
                        }
                    }
                }while(repeat);
                scene->removeItem(plat[i]->platItem);
                delete plat[i];
                if(render >= 1 && render <= 10)
                {
                    plat[i] = new brownplat({px, 0});

                }
                else {
                    plat[i] = new Normalplat({px, 0});

                }
                scene->addItem(plat[i]->platItem);
                if(dice>= 60 && dice <= 65)
                {
                    if(!isrocket && plat[i]->plattype == 0 && rocketpropcount == 0 && propcount == 0)
                    {
                        isrocket = true;
                        rocket = new QGraphicsPixmapItem;
                        rocket->setPixmap(QPixmap(":/res/rocket/rocket.png").scaled(QPixmap(":/res/rocket/rocket.png").size()*0.5));
                        rocket->setPos(plat[i]->platItem->x()+plat[i]->platItem->pixmap().width()-rocket->pixmap().width(), plat[i]->platItem->y()-rocket->pixmap().height());
                        rocket->setZValue(4);
                        scene->addItem(rocket);
                    }
                }
                else if(dice >= 70 && dice <= 80)
                {
                    if(!iscap && plat[i]->plattype == 0 && propcount == 0 && rocketpropcount == 0)
                    {
                        iscap = true;
                        cap = new QGraphicsPixmapItem;
                        cap->setPixmap(QPixmap(":/res/cap/cap.png").scaled(QPixmap(":/res/cap/cap.png").size()*0.5));
                        cap->setPos(plat[i]->platItem->x()+plat[i]->platItem->pixmap().width()-cap->pixmap().width(), plat[i]->platItem->y()-cap->pixmap().height());
                        cap->setZValue(4);
                        scene->addItem(cap);
                    }
                }
            }
            }
        }
    }
    if(ismonster)
    {
        if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>monsterItem->x())&&
                (doodleplayer->playerItem->x()<(monsterItem->x()+monsterItem->pixmap().width()))&&
                (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>monsterItem->y())&&
                (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(monsterItem->y()+monsterItem->pixmap().height())))
        {
            gameover = true;
            scoreup = false;
            timecount=0;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleflyup()));
        }
    }
    if(ishole)
    {
        if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>holeItem->x())&&
                (doodleplayer->playerItem->x()<(holeItem->x()+holeItem->pixmap().width()))&&
                (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>holeItem->y())&&
                (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(holeItem->y()+holeItem->pixmap().height())))
        {
            gameover = true;
            scoreup = false;
            timecount=0;
            lcount = 0;
            rcount = 0;
            falltimecount = 0;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleflyup()));
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleleft()));
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleright()));
            connect(timer, SIGNAL(timeout()), this, SLOT(holegameover()));
        }
    }
    if(isrocket && rocketpropcount == 0 && propcount == 0)
    {
        if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>rocket->x())&&
                (doodleplayer->playerItem->x()<(rocket->x()+rocket->pixmap().width()))&&
                (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>rocket->y())&&
                (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(rocket->y()+rocket->pixmap().height())) && !equipecap && !equiperocket && !isshoot)
        {
            equiperocket = true;
            scoreup = true;
            rocket->setZValue(doodleplayer->playerItem->zValue()+1);
            if(!face)
            {
                rocket->setPixmap(QPixmap(":/res/rocket/rleft.png").scaled(56, 100));
                rocket->setPos(doodleplayer->playerItem->x()-rocket->pixmap().width()+15, doodleplayer->playerItem->y()-10);
            }
            else {
                rocket->setPixmap(QPixmap(":/res/rocket/rrightbig.png").scaled(56, 100));
                rocket->setPos(doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-15, doodleplayer->playerItem->y()-10);
            }
            timecount=0;
            falltimecount = 0;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleflyup()));
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
            connect(timer, SIGNAL(timeout()), this, SLOT(rocketfly()));
        }
    }
    if(iscap && propcount == 0 && rocketpropcount == 0)
    {
        if((doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-5>cap->x())&&
                (doodleplayer->playerItem->x()<(cap->x()+cap->pixmap().width()))&&
                (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()>cap->y())&&
                (doodleplayer->playerItem->y()+doodleplayer->playerItem->pixmap().height()<(cap->y()+cap->pixmap().height())) && !equipecap && !equiperocket && !isshoot)
        {
            equipecap = true;
            scoreup = true;
            cap->setZValue(doodleplayer->playerItem->zValue()+1);
            cap->setPixmap(QPixmap(":/res/cap/cap1.png").scaled(50, 30));
            if(!face)
                cap->setPos(doodleplayer->playerItem->x()-5, doodleplayer->playerItem->y()+20-cap->pixmap().height());
            else {
                cap->setPos(doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-cap->pixmap().width()+5, doodleplayer->playerItem->y()+20-cap->pixmap().height());
            }
            timecount=0;
            falltimecount = 0;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleflyup()));
            disconnect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
            connect(timer, SIGNAL(timeout()), this, SLOT(capfly()));

        }
    }
    if((4.3-4.9*timecount*0.01)<0)
    {
        scoreup = false;
        timecount=0;
        disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleflyup()));
    }
    }
}
void MainWindow::rocketfly()
{
    if(!gamepause)
    {
    rocketpropcount++;

    if(rocketpropcount<400)
    {
        doodleplayer->playerItem->setPos(doodleplayer->playerItem->x(), doodleplayer->playerItem->y()-8);
        rocket->setY(rocket->y()-8);
        if(scoreup)
        {
            doodleplayer->score += 8;
            scoreItem->setPlainText(QString::number(doodleplayer->score));
        }
    }
    if(doodleplayer->playerItem->x()<0-doodleplayer->playerItem->pixmap().width())
    {
        doodleplayer->playerItem->setX(400);

    }
    if(doodleplayer->playerItem->x()>400)
    {
        doodleplayer->playerItem->setX(0-doodleplayer->playerItem->pixmap().width());
    }
    if(doodleplayer->playerItem->y()<250)
    {
        doodleplayer->playerItem->setY(250);
        rocket->setY(250);
        int dice = qrand()%1000;
        if(dice == 40)
        {
            if(!ismonster)
            {
                ismonster = true;
                monsterItem = new QGraphicsPixmapItem;
                monsterItem->setPixmap(QPixmap(":/res/monsters/bat1.png").scaled(QPixmap(":/res/monsters/bat1.png").size()*0.8));
                monsterItem->setPos(qrand()%(400-monsterItem->pixmap().width()), 0);
                monsterItem->setZValue(10);
                scene->addItem(monsterItem);
                connect(timer, SIGNAL(timeout()), this, SLOT(monstermoveright()));
            }
        }
        else if(dice == 50)
        {
            if(!ishole)
            {
                ishole = true;
                holeItem = new QGraphicsPixmapItem;
                holeItem->setPixmap(QPixmap(":/res/hole.png").scaled(QPixmap(":/res/hole.png").size()*0.6));
                holeItem->setPos(qrand()%(400-holeItem->pixmap().width()), 0);
                holeItem->setZValue(5);
                scene->addItem(holeItem);
            }
        }

        if(ismonster)
        {
            monsterItem->setY(monsterItem->y()+8);
            if(monsterItem->y()>500)
            {
                scene->removeItem(monsterItem);
                delete monsterItem;
                monsterItem = nullptr;
                ismonster = false;
            }
        }
        if(ishole)
        {
            holeItem->setY(holeItem->y()+8);
            if(holeItem->y()>500)
            {
                scene->removeItem(holeItem);
                delete holeItem;
                holeItem = nullptr;
                ishole = false;
            }
        }
        if(iscap)
        {
            cap->setY(cap->y()+8);
            if(cap->y()>500)
            {
                scene->removeItem(cap);
                delete cap;
                cap = nullptr;
                iscap = false;
            }
        }
        int render;
        for(int i = 0; i < PLATSIZE; i++)
        {

            plat[i]->platItem->setY(plat[i]->platItem->y()+8);
            if(plat[i]->plattype == 0 || (plat[i]->plattype == 1 && plat[i]->state == 0))
            {
            if(plat[i]->platItem->y()>500)
            {
                render = qrand()%50 + 1;
                bool repeat = false;
                int px, py;
                px = qrand()%(400-QPixmap(":/res/p-green.png").width());
                py = 0;
                do
                {
                    repeat = false;

                    for(int j = 0; j < PLATSIZE; j++)
                    {
                        if(j == i)
                            continue;
                        if(px>=plat[j]->platItem->x()&&px<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py>=plat[j]->platItem->y()&&py<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height())
                        {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if(px+plat[j]->platItem->pixmap().width()>=plat[j]->platItem->x()&&px+plat[j]->platItem->pixmap().width()<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py+plat[j]->platItem->pixmap().height()>=plat[j]->platItem->y()&&py+plat[j]->platItem->pixmap().height()<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height())
                        {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if (px>=plat[j]->platItem->x()&&px<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py+plat[j]->platItem->pixmap().height()>=plat[j]->platItem->y()&&py+plat[j]->platItem->pixmap().height()<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height()) {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if (px+plat[j]->platItem->pixmap().width()>=plat[j]->platItem->x()&&px+plat[j]->platItem->pixmap().width()<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py>=plat[j]->platItem->y()&&py<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height()) {

                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        if(px>400-plat[j]->platItem->pixmap().width())
                        {
                            px = qrand()%(400-QPixmap(":/res/p-green.png").width());
                            repeat=true;
                        }
                    }
                }while(repeat);
                scene->removeItem(plat[i]->platItem);
                delete plat[i];
                if(render >= 1 && render <= 10)
                {
                    plat[i] = new brownplat({px, 0});

                }
                else {
                    plat[i] = new Normalplat({px, 0});

                }
                scene->addItem(plat[i]->platItem);
            }
            }
        }
    }
    if(rocketpropcount>400)
    {
        scoreup = false;
        if(equiperocket)
        {
            connect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
        }
        equiperocket = false;
        rocket->setPixmap(QPixmap(":/res/rocket/rocket.png").scaled(QPixmap(":/res/rocket/rocket.png").size()*0.5));
        rocket->setPos(rocket->x()+1, rocket->y()+3);
        if(rocket->y()>500)
        {
            rocketpropcount = 0;
            isrocket = false;
            scene->removeItem(rocket);
            delete rocket;
            rocket = nullptr;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(rocketfly()));
        }
    }
    }
}
void MainWindow::capfly()
{
    if(!gamepause)
    {
    propcount++;

    if(propcount<200)
    {
        doodleplayer->playerItem->setPos(doodleplayer->playerItem->x(), doodleplayer->playerItem->y()-5);
        cap->setY(cap->y()-5);
        if(propcount%30==0)
        {
            cap->setPixmap(QPixmap(":/res/cap/cap3.png").scaled(50,30));
        }
        else if(propcount%20==0){
            cap->setPixmap(QPixmap(":/res/cap/cap2.png").scaled(50,30));

        }
        else if(propcount%10==0){
            cap->setPixmap(QPixmap(":/res/cap/cap1.png").scaled(50,30));

        }
        if(scoreup)
        {
            doodleplayer->score += 5;
            scoreItem->setPlainText(QString::number(doodleplayer->score));
        }
    }
    if(doodleplayer->playerItem->x()<0-doodleplayer->playerItem->pixmap().width())
    {
        doodleplayer->playerItem->setX(400);

    }
    if(doodleplayer->playerItem->x()>400)
    {
        doodleplayer->playerItem->setX(0-doodleplayer->playerItem->pixmap().width());
    }
    if(doodleplayer->playerItem->y()<250)
    {
        doodleplayer->playerItem->setY(250);
        cap->setY(270-cap->pixmap().height());
        int dice = qrand()%1000;
        if(dice == 40)
        {
            if(!ismonster)
            {
                ismonster = true;
                monsterItem = new QGraphicsPixmapItem;
                monsterItem->setPixmap(QPixmap(":/res/monsters/bat1.png").scaled(QPixmap(":/res/monsters/bat1.png").size()*0.8));
                monsterItem->setPos(qrand()%(400-monsterItem->pixmap().width()), 0);
                monsterItem->setZValue(10);
                scene->addItem(monsterItem);
                connect(timer, SIGNAL(timeout()), this, SLOT(monstermoveright()));
            }
        }
        else if(dice == 50)
        {
            if(!ishole)
            {
                ishole = true;
                holeItem = new QGraphicsPixmapItem;
                holeItem->setPixmap(QPixmap(":/res/hole.png").scaled(QPixmap(":/res/hole.png").size()*0.6));
                holeItem->setPos(qrand()%(400-holeItem->pixmap().width()), 0);
                holeItem->setZValue(5);
                scene->addItem(holeItem);
            }
        }

        if(ismonster)
        {
            monsterItem->setY(monsterItem->y()+5);
            if(monsterItem->y()>500)
            {
                scene->removeItem(monsterItem);
                delete monsterItem;
                monsterItem = nullptr;
                ismonster = false;
            }
        }
        if(ishole)
        {
            holeItem->setY(holeItem->y()+5);
            if(holeItem->y()>500)
            {
                scene->removeItem(holeItem);
                delete holeItem;
                holeItem = nullptr;
                ishole = false;
            }
        }
        if(isrocket)
        {
            rocket->setY(rocket->y()+8);
            if(rocket->y()>500)
            {
                scene->removeItem(rocket);
                delete rocket;
                rocket = nullptr;
                isrocket = false;
            }
        }
        int render;
        for(int i = 0; i < PLATSIZE; i++)
        {

            plat[i]->platItem->setY(plat[i]->platItem->y()+5);
            if(plat[i]->plattype == 0 || (plat[i]->plattype == 1 && plat[i]->state == 0))
            {
            if(plat[i]->platItem->y()>500)
            {
                render = qrand()%50 + 1;
                bool repeat = false;
                int px, py;
                px = qrand()%(400-QPixmap(":/res/p-green.png").width());
                py = 0;
                do
                {
                    repeat = false;

                    for(int j = 0; j < PLATSIZE; j++)
                    {
                        if(j == i)
                            continue;
                        if(px>=plat[j]->platItem->x()&&px<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py>=plat[j]->platItem->y()&&py<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height())
                        {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if(px+plat[j]->platItem->pixmap().width()>=plat[j]->platItem->x()&&px+plat[j]->platItem->pixmap().width()<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py+plat[j]->platItem->pixmap().height()>=plat[j]->platItem->y()&&py+plat[j]->platItem->pixmap().height()<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height())
                        {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if (px>=plat[j]->platItem->x()&&px<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py+plat[j]->platItem->pixmap().height()>=plat[j]->platItem->y()&&py+plat[j]->platItem->pixmap().height()<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height()) {
                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        else if (px+plat[j]->platItem->pixmap().width()>=plat[j]->platItem->x()&&px+plat[j]->platItem->pixmap().width()<=plat[j]->platItem->x()+plat[j]->platItem->pixmap().width()&&py>=plat[j]->platItem->y()&&py<=plat[j]->platItem->y()+plat[j]->platItem->pixmap().height()) {

                            px+=plat[j]->platItem->pixmap().width();
                            repeat=true;
                        }
                        if(px>400-plat[j]->platItem->pixmap().width())
                        {
                            px = qrand()%(400-QPixmap(":/res/p-green.png").width());
                            repeat=true;
                        }
                    }
                }while(repeat);
                scene->removeItem(plat[i]->platItem);
                delete plat[i];
                if(render >= 1 && render <= 10)
                {
                    plat[i] = new brownplat({px, 0});

                }
                else {
                    plat[i] = new Normalplat({px, 0});

                }
                scene->addItem(plat[i]->platItem);
            }
            }
        }
    }
    if(propcount>200)
    {
        scoreup = false;
        if(equipecap)
        {
            connect(timer, SIGNAL(timeout()), this, SLOT(doodlefly()));
        }
        equipecap = false;
        cap->setPixmap(QPixmap(":/res/cap/cap.png").scaled(QPixmap(":/res/cap/cap.png").size()*0.5));
        cap->setPos(cap->x()+1, cap->y()+3);
        if(cap->y()>500)
        {
            propcount = 0;
            iscap = false;
            scene->removeItem(cap);
            delete cap;
            cap = nullptr;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(capfly()));
        }
    }
    }
}
void MainWindow::monstermoveright()
{
    if(!gamepause)
    {
        if(ismonster)
        {
            monsterItem->setX(monsterItem->x()+1);
            if(monsterItem->x() >= 400 - monsterItem->pixmap().width())
            {
                disconnect(timer, SIGNAL(timeout()), this, SLOT(monstermoveright()));
                connect(timer, SIGNAL(timeout()), this, SLOT(monstermoveleft()));
            }
        }
        else {
            disconnect(timer, SIGNAL(timeout()), this, SLOT(monstermoveright()));
        }
    }
}
void MainWindow::monstermoveleft()
{
    if(!gamepause)
    {
        if(ismonster)
        {
            monsterItem->setX(monsterItem->x()-1);
            if(monsterItem->x() <= 0)
            {
                disconnect(timer, SIGNAL(timeout()), this, SLOT(monstermoveleft()));
                connect(timer, SIGNAL(timeout()), this, SLOT(monstermoveright()));
            }
        }
        else {
            disconnect(timer, SIGNAL(timeout()), this, SLOT(monstermoveleft()));
        }
    }
}
void MainWindow::holegameover()
{
    timecount++;
    if(timecount%5 == 0)
    {
        if(timecount < 200)
        {
            doodleplayer->playerItem->setPixmap(doodleplayer->playerItem->pixmap().scaled(doodleplayer->playerItem->pixmap().size()*0.9));
            double disx = doodleplayer->playerItem->x() + doodleplayer->playerItem->pixmap().width()/2 - (holeItem->x() + holeItem->pixmap().width()/2);
            double disy = doodleplayer->playerItem->y() + doodleplayer->playerItem->pixmap().height()/2 - (holeItem->y() + holeItem->pixmap().height()/2);
            if(timecount < 25 &&(disx != 0.0 || disy != 0.0))
            {
                doodleplayer->playerItem->setPos(doodleplayer->playerItem->x()-disx/5, doodleplayer->playerItem->y()-disy/5);
            }
            else
            {
                doodleplayer->playerItem->setPos(holeItem->x()+holeItem->pixmap().width()/2-doodleplayer->playerItem->pixmap().width()/2, holeItem->y()+holeItem->pixmap().height()/2-doodleplayer->playerItem->pixmap().height()/2);

            }
        }
        else {
            timecount = 0;
            falltimecount = 80;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(holegameover()));
            for(int i = 0; i < PLATSIZE; i++)
            {
                scene->removeItem(plat[i]->platItem);
            }
            if(ismonster)
            {
                ismonster = false;
                scene->removeItem(monsterItem);
            }
            if(ishole)
            {
                ishole = false;
                scene->removeItem(holeItem);
            }
            if(isrocket)
            {
                isrocket = false;
                scene->removeItem(rocket);
            }
            if(iscap)
            {
                iscap = false;
                scene->removeItem(cap);
            }
            gameoverItem = new QGraphicsPixmapItem;
            gameoverItem->setPixmap(QPixmap(":/res/menu/gameover0.png").scaled(400,533));
            scene->addItem(gameoverItem);
            gameoverItem->setY(533);
            finalscore = new QGraphicsTextItem;
            finalscore->setPlainText(QString::number(doodleplayer->score));
            finalscore->setFont(QFont("Doodle Jump", 30, 75));
            finalscore->setPos(210, 860);
            scene->addItem(finalscore);
            connect(timer, SIGNAL(timeout()), this, SLOT(gameoverfall()));

        }
    }
}
void MainWindow::doodleleft()
{
    if(!gamepause)
    {
        lcount++;
        doodleplayer->playerItem->setX(doodleplayer->playerItem->x() - 3);
        if(equipecap)
        {
            cap->setX(doodleplayer->playerItem->x() + doodleplayer->playerItem->pixmap().width() + 5 - cap->pixmap().width());
        }
        else if(equiperocket)
        {
            rocket->setX(doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()-15);

        }
        if(!iskey)
        {
            if(lcount >= 1)
            {
                lcount = 0;
                if(!iskey)
                    disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleleft()));
            }
        }
    }
}
void MainWindow::doodleright()
{
    if(!gamepause)
    {
        rcount++;
        doodleplayer->playerItem->setX(doodleplayer->playerItem->x() + 3);
        if(equipecap)
        {
            cap->setX(doodleplayer->playerItem->x()-5);
        }
        else if(equiperocket)
        {
            rocket->setX(doodleplayer->playerItem->x()-doodleplayer->playerItem->pixmap().width()+15);
        }
        if(!iskey)
        {
            if(rcount >= 1)
            {
                rcount = 0;
                if(!iskey)
                    disconnect(timer, SIGNAL(timeout()), this, SLOT(doodleright()));
            }
        }
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(!gameover && !gamepause)
    {
        switch(e->key())
        {
        case Qt::Key_A:
        case Qt::Key_Left:
            iskey = false;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            iskey = false;
            break;
        case Qt::Key_Space:
        case Qt::Key_Up:
            break;
        default:
            break;
        }
    }
}
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(!gameover && !iskey && gameopen && !gamepause)
    {
        switch(e->key())
        {
        case Qt::Key_A:
        case Qt::Key_Left:

            if(lcount == 0 && !iskey)
            {
                doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodle_left.png").scaled(QPixmap(":/res/doodleR.png").size()));
                if(equiperocket)
                {
                    rocket->setPixmap(QPixmap(":/res/rocket/rrightbig.png").scaled(56,100));
                }
                face = 1;
                connect(timer, SIGNAL(timeout()), this, SLOT(doodleleft()));
            }
            iskey = true;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            if(rcount == 0 && !iskey)
            {
                doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodle_right.png").scaled(QPixmap(":/res/doodleR.png").size()));
                if(equiperocket)
                {
                    rocket->setPixmap(QPixmap(":/res/rocket/rleft.png").scaled(56,100));
                }
                face = 0;
                connect(timer, SIGNAL(timeout()), this, SLOT(doodleright()));
            }
            iskey = true;
            break;
        default:
            break;
        }
    }
    if(!gameover && !isshoot && gameopen && !gamepause && !equipecap && !equiperocket)
    {
        switch(e->key())
        {
        case Qt::Key_Space:
        case Qt::Key_Up:
            if(!isshoot)
            {
                doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodleshoot.png").scaled(40, QPixmap(":/res/doodleR.png").height()));
                doodleplayer->bullet->setPos(doodleplayer->playerItem->x()+doodleplayer->playerItem->pixmap().width()/2-doodleplayer->bullet->pixmap().width()/2, doodleplayer->playerItem->y());
                scene->addItem(doodleplayer->bullet);
                connect(timer, SIGNAL(timeout()), this, SLOT(shoot()));
            }
            isshoot = true;
            break;
        default:
            break;
        }
    }
}
void MainWindow::shoot()
{
    if(!gamepause)
    {
        doodleplayer->bullet->setY(doodleplayer->bullet->y()-10);
        if(doodleplayer->bullet->y()<0)
        {
            doodleplayer->playerItem->setPixmap(QPixmap(":/res/doodle_right.png").scaled(QPixmap(":/res/doodleR.png").size()));
            scene->removeItem(doodleplayer->bullet);
            disconnect(timer, SIGNAL(timeout()), this, SLOT(shoot()));
            isshoot = false;
        }
        if(ismonster)
        {
            if((doodleplayer->bullet->x()+doodleplayer->bullet->pixmap().width()-5>monsterItem->x())&&
                    (doodleplayer->bullet->x()<(monsterItem->x()+monsterItem->pixmap().width()))&&
                    (doodleplayer->bullet->y()+doodleplayer->bullet->pixmap().height()>monsterItem->y())&&
                    (doodleplayer->bullet->y()+doodleplayer->bullet->pixmap().height()<(monsterItem->y()+monsterItem->pixmap().height())))
            {
                scene->removeItem(monsterItem);
                delete monsterItem;
                monsterItem=nullptr;
                ismonster = false;
            }
        }
    }
}
