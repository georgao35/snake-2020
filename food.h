#ifndef FOOD_H
#define FOOD_H

#include "constant.h"
#include <QGraphicsItem>

class food:public QGraphicsItem
{
    Q_CLASSINFO("author","John Doe");
public:
    food()=delete ;
    food(Pii pos);
    food(int x,int y);//行、列数，未转换
    Pii img_to_view(Pii origin){
        return qMakePair(origin.first*TILE_WIDTH-TILE_WIDTH
                         ,origin.second*TILE_WIDTH-TILE_WIDTH);
    }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

};

#endif // FOOD_H
