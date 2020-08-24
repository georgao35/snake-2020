#include "obstacles.h"

#include <QPainter>
#include <QDebug>

obstacles::obstacles(int x,int y)
{
    setPos(img_to_view(qMakePair(x,y)).first,img_to_view(qMakePair(x,y)).second);
}

obstacles::obstacles(Pii origin)
{
    //setPos(origin.first*TILE_WIDTH-0.5*TILE_WIDTH
    //       ,origin.second*TILE_WIDTH-0.5*TILE_WIDTH);
    setPos((origin.first-1)*TILE_WIDTH,(origin.second-1)*TILE_WIDTH);
    //qDebug()<<origin.first<<origin.second;
}

QRectF obstacles::boundingRect() const
{
    return QRectF(0,0,TILE_WIDTH,TILE_WIDTH);
}

QPainterPath obstacles::shape() const
{
    QPainterPath path;
    path.addRect(0,0,TILE_WIDTH,TILE_WIDTH);
    return path;
}

void obstacles::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setPen(Qt::black);
    painter->setBrush(QBrush(Qt::darkGray));
    painter->drawPath(shape());

    painter->restore();
}
