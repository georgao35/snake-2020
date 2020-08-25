#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolBar>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))

{
    /*--初始化界面和view组件的大小和背景--
        若要关闭scroll bar，可以调用scrollbarpolicy这一个接口；
        同时可以用ensurevisible来确保一些东西可以被显示出来
    --*/
    ui->setupUi(this);
    view = ui->graphicsView;
    view->setScene(scene);
    setView();
    initBackground();

    game = new gamecontroller(this,scene,this);
/*--初始化按钮、菜单栏、工具栏--*/
{    start = new QAction(this);
    start -> setText(tr("&开始游戏"));
    QMenu* preparation = menuBar()->addMenu("操作");
    preparation->addAction(start);
    addToolBar(tr("start"))->addAction(start);
    connect(ui->startButton,&QPushButton::clicked,start,&QAction::trigger);
    connect(start,&QAction::triggered,game,&gamecontroller::start);
    /*--退出游戏--*/
    quit = new QAction(this);
    quit -> setText(tr("&退出游戏"));
    quit -> setShortcut(QKeySequence::Quit);
    preparation->addAction(quit);
    addToolBar(tr("quit"))->addAction(quit);
    connect(ui->quitButton,&QPushButton::clicked,quit,&QAction::trigger);
    connect(quit,&QAction::triggered,QApplication::instance(),&QApplication::quit);
    /*--载入游戏--*/
    load = new QAction(this);
    load -> setText(tr("载入游戏"));
    QMenu* saving = menuBar()->addMenu("保存/载入");
    saving->addAction(load);
    addToolBar(tr("load"))->addAction(load);
    connect(ui->loadButton,&QPushButton::clicked,load,&QAction::trigger);
    connect(load,&QAction::triggered,game,&gamecontroller::load);
    /*--保存游戏，输出文件--*/
    save = new QAction(this);
    save -> setText(tr("&保存游戏"));
    save ->setShortcut(QKeySequence::SaveAs);
    saving->addAction(save);
    addToolBar(tr("save"))->addAction(save);
    connect(ui->saveButton,&QPushButton::clicked,save,&QAction::trigger);
    connect(save,&QAction::triggered,game,&gamecontroller::save);
    /*--暂停重启游戏--*/
    pause = new QAction(this);
    pause->setText(tr("&暂停游戏"));
    QMenu* gaming = menuBar()->addMenu("游戏");
    gaming->addAction(pause);
    addToolBar("pause")->addAction(pause);
    connect(ui->pauseButton,&QPushButton::clicked,pause,&QAction::trigger);
    connect(pause,&QAction::triggered,game,&gamecontroller::pause);
    /*--继续游戏--*/
    unpause = new QAction(this);
    unpause->setText(tr("&继续游戏"));
    gaming->addAction(unpause);
    addToolBar("continue")->addAction(unpause);
    connect(ui->continueButton,&QPushButton::clicked,unpause,&QAction::trigger);
    connect(unpause,&QAction::triggered,game,&gamecontroller::resume);
    /*--新游戏--*/
    restart = new QAction(this);
    restart ->setText(tr("新游戏"));
    gaming ->addAction(restart);
    addToolBar(tr("new game"))->addAction(restart);
    connect(ui->restartButton,&QPushButton::clicked,restart,&QAction::trigger);
    //connect(restart,&QAction::triggered,game,&gamecontroller::restart);
    connect(restart,&QAction::triggered,game,&gamecontroller::restart);
    /*--设置动作状态--*/
    setButtonsStatus();
}
    /*--为view添加事件过滤器--*/
    view->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setButtonsStatus(){
    switch (game->getStatus()) {
    case gamecontroller::gameStatus::initialized:
        ui->pauseButton->setDisabled(true);
        pause->setDisabled(true);
        ui->continueButton->setDisabled(true);
        unpause->setDisabled(true);
        ui->restartButton->setDisabled(true);
        restart->setDisabled(true);
        ui->saveButton->setDisabled(true);
        save->setDisabled(true);
        ui->startButton->setDisabled(false);
        start->setDisabled(false);
        ui->quitButton->setDisabled(false);
        quit->setDisabled(false);
        ui->loadButton->setDisabled(false);
        load->setDisabled(false);
        break;
    case gamecontroller::gameStatus::gaming:
        ui->startButton->setDisabled(true);
        start->setDisabled(true);
        ui->loadButton->setDisabled(true);
        load->setDisabled(true);
        ui->continueButton->setDisabled(true);
        unpause->setDisabled(true);
        ui->restartButton->setDisabled(true);
        restart->setDisabled(true);
        ui->saveButton->setDisabled(true);
        save->setDisabled(true);
        ui->quitButton->setDisabled(false);
        quit->setDisabled(false);
        ui->pauseButton->setDisabled(false);
        pause->setDisabled(false);
        break;
    case gamecontroller::gameStatus::paused:
        ui->startButton->setDisabled(true);
        start->setDisabled(true);
        ui->pauseButton->setDisabled(true);
        pause->setDisabled(true);
        ui->loadButton->setDisabled(true);
        load->setDisabled(true);
        ui->quitButton->setDisabled(false);
        quit->setDisabled(false);
        ui->continueButton->setDisabled(false);
        unpause->setDisabled(false);
        ui->restartButton->setDisabled(false);
        restart->setDisabled(false);
        ui->saveButton->setDisabled(false);
        save->setDisabled(false);
        break;
    case gamecontroller::gameStatus::ended:
        ui->startButton->setDisabled(true);
        start->setDisabled(true);
        ui->pauseButton->setDisabled(true);
        pause->setDisabled(true);
        ui->loadButton->setDisabled(true);
        load->setDisabled(true);
        ui->quitButton->setDisabled(false);
        quit->setDisabled(false);
        ui->continueButton->setDisabled(true);
        unpause->setDisabled(true);
        ui->restartButton->setDisabled(false);
        restart->setDisabled(false);
        ui->saveButton->setDisabled(true);
        save->setDisabled(true);
        break;
    }
}

Pii view_to_img(Pii view){
    return qMakePair(1+view.first/TILE_WIDTH,1+view.second/TILE_WIDTH);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event){
    if(game->getStatus() == gamecontroller::gameStatus::initialized
            and object == view){
        QMouseEvent* mouse = dynamic_cast<QMouseEvent*>(event);
        if(mouse!=nullptr){
            //qDebug()<<"鼠标@"<<mouse->pos().rx()<<" "<<mouse->pos().ry();
            game->handleClick(qMakePair(mouse->pos().rx(),mouse->pos().ry()));
            return true;
        }
        else
            return QMainWindow::eventFilter(object, event);
    }
    else if(game->getStatus() == gamecontroller::gameStatus::gaming
            and object == view)
    {
        QKeyEvent* key = dynamic_cast<QKeyEvent*>(event);
        if(key!=nullptr and key->type()==QEvent::KeyPress)
        {
            game->handlePress(key);
            return true;
        }else
            return QMainWindow::eventFilter(object, event);
    }
    else
        return QMainWindow::eventFilter(object, event);
}

void MainWindow::initBackground(){
    QPixmap bg(TILE_WIDTH, TILE_WIDTH);
    QPainter p(&bg);
    p.setBrush(QBrush(Qt::white));
    p.drawRect(0, 0, TILE_WIDTH, TILE_WIDTH);

    view->setBackgroundBrush(QBrush(bg));
}

void MainWindow::setView(){
    view->resize((column)*TILE_WIDTH+1,(row)*TILE_WIDTH+1);
    scene->setSceneRect(0,0,(column)*TILE_WIDTH,(row)*TILE_WIDTH);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->ensureVisible(scene->sceneRect());
}

void MainWindow::setDisplayTime(int time)
{
    ui->timeDisplay->setNum(time);
}
