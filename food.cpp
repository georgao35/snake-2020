#include "food.h"

#include <QString>
#include <QPainter>

food::food(Pii pos){
    setPos(pos.first*TILE_WIDTH-TILE_WIDTH,pos.second*TILE_WIDTH-TILE_WIDTH);
}

food::food(int x,int y){
    setPos((x-1)*TILE_WIDTH,(y-1)*TILE_WIDTH);
}

QRectF food::boundingRect() const
{
    return QRectF(0,0,TILE_WIDTH,TILE_WIDTH);
}

QPainterPath food::shape() const
{
    return QPainterPath();
}

void food::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pic;
    pic.load(":/pic/food.png");
    //QGraphicsPixmapItem picItem (pic,this);
    painter->save();
    QRectF source (0,0,200.0,200.0);
    painter->drawPixmap(boundingRect(),pic,source);

    painter->restore();
}
