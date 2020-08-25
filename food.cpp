#include "food.h"

#include <QString>
#include <QPainter>

food::food(Pii pos):x(pos.first),y(pos.second){
    setPos(pos.first*TILE_WIDTH-TILE_WIDTH,pos.second*TILE_WIDTH-TILE_WIDTH);
}

food::food(int x,int y):x(x),y(y){
    setPos((x-1)*TILE_WIDTH,(y-1)*TILE_WIDTH);
}

QRectF food::boundingRect() const
{
    return QRectF(0,0,TILE_WIDTH,TILE_WIDTH);
}

QPainterPath food::shape() const
{
    QPainterPath path;
    path.addRect(0,0,TILE_WIDTH,TILE_WIDTH);
    return path;
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
