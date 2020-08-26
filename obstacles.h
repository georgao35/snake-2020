#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "constant.h"
#include <QGraphicsItem>

class obstacles:public QGraphicsItem
{
    int posX,posY;
public:
    obstacles(int x,int y);
    obstacles(Pii x);
    obstacles(QString src);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    Pii img_to_view(Pii origin){
        return qMakePair(origin.first*TILE_WIDTH-0.5*TILE_WIDTH
                         ,origin.second*TILE_WIDTH-0.5*TILE_WIDTH);
    }
    //将字符串形式的坐标转换为数字
    Pii string_to_pii(QString origin){
        origin = origin.split(QLatin1Char('('),Qt::SkipEmptyParts)[0];
        origin = origin.split(QLatin1Char(')'),Qt::SkipEmptyParts)[0];
        int num1 = origin.split(QLatin1Char(','))[0].toInt();
        int num2 = origin.split(QLatin1Char(','))[1].toInt();
        return qMakePair(num1,num2);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    Pii currentPos(){
        return qMakePair(posX,posY);
    }

};

#endif // OBSTACLES_H
