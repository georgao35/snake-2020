#include "gamecontroller.h"
#include "mainwindow.h"

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QFileDialog>
#include <QTime>
#include <QDateTime>

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
    timer->start(speed);
    //connect(time,&QTimer::timeout,father,&MainWindow::setDisplayTime);
    //scene->installEventFilter(this);
    //apple = new food(3,3);
    //scene->addItem(apple);
    Snake = new snake(startHead,startBody);
    scene->addItem(Snake);
}

void gamecontroller::pause(){
    status = gameStatus::paused;
    disconnect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    father->setButtonsStatus();
    //qDebug()<<"entering pause\n";
}

void gamecontroller::start(){
    status = gameStatus::gaming;
    father->setButtonsStatus();
    setNewFood();
    connect(timer,&QTimer::timeout,this,&gamecontroller::advance);
    //qDebug()<<"start a game\n";
}
//文件读入输出
void gamecontroller::load(){
    QString fileName = QFileDialog::getOpenFileName();
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly |QIODevice::Text)){
            QMessageBox::warning(nullptr,tr("warning"),tr("无法打开"));
        }
        else{
            scene->clear();
            QTextStream in(&file);
            /*-蛇部分读取信息-*/
            in.readLine();
            QString head = in.readLine();//head
            head = head.split(QLatin1Char(' '))[1];
            QString body_buffer = in.readLine();//body
            QStringList body = body_buffer.split(QLatin1Char(' '),Qt::SkipEmptyParts);//没加要求使得最后多了一个空字符串，导致转换坐标时出现问题
            body.pop_front();
            QString direc = in.readLine();
            direc = direc.split(QLatin1Char(' '))[1];
            Snake = new snake(head,body,direc);
            scene->addItem(Snake);
            in.readLine();
             /*-食物部分-*/
            in.readLine();
            QString food_bf = in.readLine();
            food_bf = food_bf.split(QLatin1Char(' '),Qt::SkipEmptyParts)[1];
            apple = new food(food_bf);
            scene->addItem(apple);
            qDebug()<<apple->currentPos()<<apple->pos();
            in.readLine();
            /*-障碍部分-*/
            in.readLine();
            QString buffer_for_obstacles = in.readLine();
            QStringList buffer_list_bostacles = buffer_for_obstacles.split(QLatin1Char(' '),Qt::SkipEmptyParts);
            buffer_list_bostacles.pop_front();
            if(!buffer_list_bostacles.isEmpty()){
                foreach(QString obstacle, buffer_list_bostacles){
                    obstacles* ob = new obstacles(obstacle);
                    barrier.insert(qMakePair(ob->currentPos().first,ob->currentPos().second),ob);
                    scene->addItem(ob);
                }
            }
            in.readLine();
            /*-时间部分-*/
            QString time1 = in.readLine();
            time1 = time1.split(QLatin1Char(' '))[1];
            time = time1.toInt();
            father->setDisplayTime(time);
        }
        status = gameStatus::paused;
        father->setButtonsStatus();
        scene->update();
        //qDebug()<<"loading a saved game\n";
    }else{
        QMessageBox::warning(nullptr,tr("warning"),tr("未选择文件"));
    }
}

void gamecontroller::save(){
    QString fileName = QFileDialog::getSaveFileName();
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
            QMessageBox::warning(nullptr,tr("warning"),tr("无法打开"));
            return ;
        }
        else{
            QTextStream out(&file);
            QDataStream out1(&file);
            out<<"Snake:"<<Qt::endl<<"head@ ("<<Snake->head.first<<","<<Snake->head.second<<")"<<Qt::endl<<"body@ ";
            foreach(Pii point, Snake->body){
                out<<"("<<point.first<<","<<point.second<<") ";
            }
            out<<Qt::endl<<"direction: "<<Snake->direction<<Qt::endl;
            out<<Qt::endl<<"Food:"<<Qt::endl<<"food@ ("<<apple->currentPos().first<<","<<apple->currentPos().second<<")"<<Qt::endl;
            out<<Qt::endl<<"Obstacles:"<<Qt::endl<<"obstacles@ ";
            foreach(obstacles* obstacle, barrier){
                out<<"("<<obstacle->currentPos().first<<","<<obstacle->currentPos().second<<") ";
            }
            out<<Qt::endl<<Qt::endl<<"Time "<<time;
        }
    }else{
        QMessageBox::warning(nullptr,tr("warning"),tr("未选择保存文件"));
    }
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
    if(Snake->direction != snake::movingDirection::null)
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
        QTimer::singleShot(0,this,&gamecontroller::gamelost);//碰到自己，报废
    }
    else if(barrier.contains(Snake->head)){
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
    while(barrier.contains(qMakePair(x,y)) or Snake->getBodyPos().contains(qMakePair(x,y))){
        x = (QRandomGenerator::global()->generate())%column+1;
        y = (QRandomGenerator::global()->generate())%row+1;
    }
    qDebug()<<x<<y;
    if(apple != nullptr)
        scene->removeItem(apple);
    apple = new food(x,y);
    scene->addItem(apple);
}
