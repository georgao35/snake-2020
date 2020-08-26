#ifndef FOOD_H
#define FOOD_H

#include "constant.h"
#include <QGraphicsItem>

class food:public QGraphicsItem
{
    Q_CLASSINFO("author","John Doe");
    int x,y;
public:
    food()=delete ;
    food(Pii pos);
    food(int x,int y);//行、列数，未转换
    food(QString src);
    Pii img_to_view(Pii origin){
        return qMakePair(origin.first*TILE_WIDTH-TILE_WIDTH
                         ,origin.second*TILE_WIDTH-TILE_WIDTH);
    }
    Pii string_to_pii(QString origin){
        origin = origin.split(QLatin1Char('('),Qt::SkipEmptyParts)[0];
        origin = origin.split(QLatin1Char(')'),Qt::SkipEmptyParts)[0];
        int num1 = origin.split(QLatin1Char(','))[0].toInt();
        int num2 = origin.split(QLatin1Char(','))[1].toInt();
        return qMakePair(num1,num2);
    }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    Pii currentPos(){
        return qMakePair(x,y);
    }
};

#endif // FOOD_H
