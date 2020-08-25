#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "food.h"
#include "snake.h"
#include "obstacles.h"
#include <QObject>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMap>

class MainWindow;
class gamecontroller : public QObject
{
    Q_OBJECT
public:
    enum gameStatus{
        initialized,
        gaming,
        paused,
        ended,
    };
    explicit gamecontroller(QObject *parent = nullptr,QGraphicsScene* _scene = nullptr)=delete;
    explicit gamecontroller(QObject *parent = nullptr,QGraphicsScene* _scene = nullptr
            ,MainWindow* _father=nullptr);
    void pause();
    void start();
    void restart();
    void load();
    void save();
    void resume();
    void end();

    void handleClick(Pii a);
    void handlePress(QKeyEvent*);
    void handleSnakeCollide();

    //bool eventFilter(QObject *watched, QEvent *event) override;
    gamecontroller::gameStatus getStatus(){return status;}
    Pii view_to_img(Pii view){
        return qMakePair(1+view.first/TILE_WIDTH,1+view.second/TILE_WIDTH);
    }

public slots:
    void advance();

signals:

private:
    QGraphicsScene* scene;
    QTimer* timer;

    gameStatus status = gameStatus::initialized;

    MainWindow* father;
    food* apple;
    snake* Snake;
    QMap<Pii,obstacles*> barrier;
    int time = 0;
};

#endif // GAMECONTROLLER_H
