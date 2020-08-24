#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "constant.h"
#include <QGraphicsItem>

class obstacles:public QGraphicsItem
{

public:
    obstacles(int x,int y);
    obstacles(Pii x);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    Pii img_to_view(Pii origin){
        return qMakePair(origin.first*TILE_WIDTH-0.5*TILE_WIDTH
                         ,origin.second*TILE_WIDTH-0.5*TILE_WIDTH);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // OBSTACLES_H
