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
    father(_father),
    apple(nullptr)
{
    timer = new QTimer(this);
    timer->start(1000);
    //connect(time,&QTimer::timeout,father,&MainWindow::setDisplayTime);
    //scene->installEventFilter(this);
    //apple = new food(3,3);
    //scene->addItem(apple);
    Snake = new snake(qMakePair(4,4),qMakePair(4,5));
    scene->addItem(Snake);
    scene->advance();
}

void gamecontroller::pause(){
    status = gameStatus::paused;
    disconnect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    father->setButtonsStatus();
    qDebug()<<"entering pause\n";
}

void gamecontroller::start(){
    status = gameStatus::gaming;
    father->setButtonsStatus();
    connect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    qDebug()<<"start a game\n";
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
    scene->clear();
    if(apple!=nullptr)
        delete apple;
    Snake = new snake(startHead,startBody);
    if(!barrier.empty())
        barrier.clear();
    disconnect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    father->setDisplayTime();
    time = 0;

    scene->addItem(Snake);
    qDebug()<<"restart a new game";
}

void gamecontroller::resume(){
    status = gameStatus::gaming;
    father->setButtonsStatus();
    connect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    qDebug()<<"resuming a game\n";
}


void gamecontroller::handleClick(Pii a)
{
    Pii img_coordinate = view_to_img(a);
    if(barrier.contains(img_coordinate)){
        scene->removeItem(barrier[img_coordinate]);
        barrier.remove(img_coordinate);
    }else if(Snake->getBodyPos().contains(img_coordinate)){
        return;
    }
    else{
        obstacles* ob = new obstacles(img_coordinate);
        barrier.insert(img_coordinate,ob);
        scene->addItem(ob);
    }
}

void gamecontroller::handlePress(QKeyEvent * key)
{
    qDebug()<<"handle press";
    switch (key->key()) {
    case Qt::Key_Up:
        Snake->setDirection(snake::movingDirection::up);
        break;
    case Qt::Key_Down:
        Snake->setDirection(snake::movingDirection::down);
        break;
    case Qt::Key_Left:
        Snake->setDirection(snake::movingDirection::left);
        break;
    case Qt::Key_Right:
        Snake->setDirection(snake::movingDirection::right);
        break;
    default:
        break;
    }
}

void gamecontroller::advance()
{
    scene->advance();
    scene->update();
    father->setDisplayTime(++time);
}

void gamecontroller::handleSnakeCollide()
{

}
