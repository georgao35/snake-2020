#include "gamecontroller.h"
#include "mainwindow.h"

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

/*gamecontroller::gamecontroller(QObject *parent,QGraphicsScene* _scene) :
    QObject(parent),
    scene(_scene)
{
    timer = new QTimer(this);
}*/

gamecontroller::gamecontroller(QObject *parent,QGraphicsScene* _scene
        ,MainWindow* _father):
    QObject(parent),
    scene(_scene),
    father(_father)
{
    timer = new QTimer(this);
    scene->installEventFilter(this);
    apple = new food(3,3);
    scene->addItem(apple);
}

void gamecontroller::pause(){
    status = gameStatus::paused;
    father->setButtonsStatus();
    qDebug()<<"entering pause\n";
}

void gamecontroller::start(){
    status = gameStatus::gaming;
    father->setButtonsStatus();
    qDebug()<<"entering start\n";
}

void gamecontroller::load(){
    status = gameStatus::paused;
    father->setButtonsStatus();
    qDebug()<<"loading a saved game\n";
}

void gamecontroller::save(){
    qDebug()<<"saving the game\n";
}

void gamecontroller::restart(){
    status = initialized;
    father->setButtonsStatus();
    qDebug()<<"restart a new game";
}

void gamecontroller::resume(){
    status = gameStatus::gaming;
    father->setButtonsStatus();
    qDebug()<<"resuming a game\n";
}


void gamecontroller::handleClick(Pii a)
{
    Pii img_coordinate = view_to_img(a);
    if(barrier.contains(img_coordinate)){
        scene->removeItem(barrier[img_coordinate]);
        barrier.remove(img_coordinate);
    }else{
        obstacles* ob = new obstacles(img_coordinate);
        barrier.insert(img_coordinate,ob);
        scene->addItem(ob);
    }
}
