#include "gamecontroller.h"
#include "mainwindow.h"

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QRandomGenerator>

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
    apple(new food(5,5))
{
    /*--开始计时器，但尚未与其他的槽函数相连--*/
    timer = new QTimer(this);
    timer->start(1000/3);
    //connect(time,&QTimer::timeout,father,&MainWindow::setDisplayTime);
    //scene->installEventFilter(this);
    //apple = new food(3,3);
    //scene->addItem(apple);
    Snake = new snake(qMakePair(4,4),qMakePair(4,5));
    scene->addItem(Snake);
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
    setNewFood();
    connect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    qDebug()<<"start a game\n";
}
//文件读入输出
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
    Snake = new snake(startHead,startBody);
    apple = nullptr;
    if(!barrier.empty())
        barrier.clear();
    disconnect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    father->setDisplayTime();
    time = 0;

    scene->addItem(Snake);
    qDebug()<<"restart a new game";
}
//继续好像可以和start用同一个函数
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
//结束
void gamecontroller::gamelost(){
    /*--设置状态--*/
    status = ended;
    disconnect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    father->setButtonsStatus();
    //scene->clear();
    /*--调整时间--*/
    time = 0;
    father->setDisplayTime();
    /*--弹窗--*/
    if( QMessageBox::information(nullptr,tr("游戏失败"),
                         tr("游戏失败,是否直接重新开始"),QMessageBox::No|QMessageBox::Yes,
                                 QMessageBox::Yes) == QMessageBox::Yes)
    {
        QTimer::singleShot(0,this,&gamecontroller::restart);
    }
    //qDebug()<<"游戏结束";
}

void gamecontroller::advance()
{
    scene->advance();
    father->setDisplayTime(++time);
    handleSnakeCollide();
    scene->update();
}

void gamecontroller::handleSnakeCollide()
{
    if(Snake->head.first < 1 or Snake->head.first > column
            or Snake->head.second < 1 or Snake->head.second > row)
    {
        QTimer::singleShot(0,this,&gamecontroller::gamelost);
        //gamelost();出界，报废
    }
    else if(Snake->head == apple->currentPos()){
        QTimer::singleShot(0,this,&gamecontroller::handleSnakeEating);//换新的苹果，并且让蛇增长
    }
    else if(Snake->body.contains(Snake->head)){
        qDebug()<<"body crash";
        QTimer::singleShot(0,this,&gamecontroller::gamelost);//碰到自己，报废
    }
    else if(barrier.contains(Snake->head)){
        qDebug()<<"Wall crash";
        QTimer::singleShot(0,this,&gamecontroller::gamelost);
    }
}

void gamecontroller::handleSnakeEating(){
    Snake->eatFood();
    setNewFood();
}

void gamecontroller::setNewFood(){
    int x = (QRandomGenerator::global()->generate())%column+1;
    int y = (QRandomGenerator::global()->generate())%row+1;
    qDebug()<<x<<y;
    if(apple != nullptr)
        scene->removeItem(apple);
    apple = new food(x,y);
    scene->addItem(apple);
}
