#ifndef SNAKE_H
#define SNAKE_H

#include "constant.h"
#include <QGraphicsItem>

class snake:public QGraphicsItem
{

public:
    enum movingDirection{
        up,down,left,right,
    };
    snake();
    snake(Pii head,Pii body);
    Pii img_to_view(Pii origin) const {
        return qMakePair(origin.first*TILE_WIDTH-1*TILE_WIDTH
                         ,origin.second*TILE_WIDTH-1*TILE_WIDTH);
    }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void advance(int phase) override;
    void setDirection(movingDirection a);
    void move();
    void handleItems();
    QVector<Pii> getBodyPos(){
        QVector<Pii> tmp = body;
        tmp.push_back(head);
        return tmp;
    }
    //void keyPressEvent(QKeyEvent *event) override;
private:
    Pii head;
    QVector<Pii> body;
    movingDirection direction = up;

    int toGrow = 0;
};

#endif // SNAKE_H
