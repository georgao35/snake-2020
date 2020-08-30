#ifndef SNAKE_H
#define SNAKE_H

#include "constant.h"
#include <QGraphicsItem>

class snake:public QGraphicsItem
{

public:
    enum movingDirection{
        up,down,left,right,null,
    };

    snake();
    snake(Pii head,Pii body);
    snake(QString head, QStringList body, QString direction);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void advance(int phase) override;
    void setDirection(movingDirection a);
    void move();
    void eatFood();
    //void handleItems();

    QVector<Pii> getBodyPos(){
        QVector<Pii> tmp = body;
        tmp.push_back(head);
        return tmp;
    }
    Pii img_to_view(Pii origin) const {
        return qMakePair(origin.first*TILE_WIDTH-1*TILE_WIDTH
                         ,origin.second*TILE_WIDTH-1*TILE_WIDTH);
    }
    //将字符串形式的坐标转换为数字
    Pii string_to_pii(QString origin){
        origin = origin.split(QLatin1Char('('),Qt::SkipEmptyParts)[0];
        origin = origin.split(QLatin1Char(')'),Qt::SkipEmptyParts)[0];
        int num1 = origin.split(QLatin1Char(','))[0].toInt();
        int num2 = origin.split(QLatin1Char(','))[1].toInt();
        return qMakePair(num1,num2);
    }

    //void keyPressEvent(QKeyEvent *event) override;
    /*声明友元类*/
    friend class gamecontroller;
private:
    Pii head;
    QVector<Pii> body;
    movingDirection direction = null;

    int toGrow = 0;
    bool turned = false;
};

#endif // SNAKE_H
