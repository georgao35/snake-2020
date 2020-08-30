#include "snake.h"

#include <QPainter>
#include <algorithm>

snake::snake()
{

}

snake::snake(Pii _head,Pii _body){
    if(qAbs(_head.first-_body.first)==1 xor qAbs(_body.second-_head.second)==1)
    {
        head = _head;
        body.push_back(_body);
    }
}

snake::snake(QString _head, QStringList _body, QString _direction){
    head = string_to_pii(_head);
    foreach(QString _bodyPart,_body){
        body.push_back(string_to_pii(_bodyPart));
    }
    switch (_direction.toInt()) {
    case 0:
        direction = up;
        break;
    case 1:
        direction = down;
        break;
    case 2:
        direction = left;
        break;
    case 3:
        direction = right;
        break;
    default:
        direction = null;
        break;
    }
}

QRectF snake::boundingRect() const
{
    int minX = head.first,minY = head.second,maxX = head.first,maxY = head.second;
    foreach(Pii a, body){
        minX = std::min(minX,a.first);
        minY = std::min(minY,a.second);
        maxX = std::max(maxX,a.first);
        maxY = std::max(maxY,a.second);
    }
    return QRectF((minX-1)*TILE_WIDTH,(minY-1)*TILE_WIDTH,
                  (maxX-minX+1)*TILE_WIDTH,(maxY-minY+1)*TILE_WIDTH);
}

QPainterPath snake::shape() const
{
    QPainterPath path;
    path.addRect(img_to_view(head).first,img_to_view(head).second,TILE_WIDTH,TILE_WIDTH);
    foreach(Pii a, body){
        path.addRect(img_to_view(a).first,img_to_view(a).second,TILE_WIDTH,TILE_WIDTH);
    }
    return path;
}

void snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setPen(Qt::white);
    painter->setBrush(QBrush(Qt::green));
    painter->drawPath(shape());

    painter->setPen(Qt::darkGreen);
    painter->setBrush(QBrush(Qt::darkGreen));
    painter->drawRect(img_to_view(head).first,img_to_view(head).second,TILE_WIDTH,TILE_WIDTH);

    painter->restore();
}

void snake::setDirection(movingDirection a)
{
    if(a == left and direction == right)
        return;
    else if(a == right and direction == left)
        return;
    else if(a == up and direction == down)
        return ;
    else if(a == down and direction == up)
        return ;
    else if(turned)
        return;
    direction = a;
    turned = true;
}

void snake::advance(int phase)
{
    turned = false;
    if(phase == 0)
        return ;
    else if(toGrow  > 0){//一步长一个单位，而不是一次性全长完
        toGrow -- ;
        body.push_back(head);
        switch (direction) {
        case up:
            head.second--;
            break;
        case down:
            head.second++;
            break;
        case left:
            head.first--;
            break;
        case right:
            head.first++;
            break;
        default:
            break;
        }
    }
    else{
        move();
    }

}

void snake::move()
{
    switch (direction) {
    case up:
        body.push_back(head);
        head.second--;
        body.pop_front();
        break;
    case down:
        body.push_back(head);
        head.second++;
        body.pop_front();
        break;
    case left:
        body.push_back(head);
        head.first--;
        body.pop_front();
        break;
    case right:
        body.push_back(head);
        head.first++;
        body.pop_front();
        break;
    default:
        break;
    }
}
/*
void snake::handleItems()
{
    if(head.first < 1 or head.first > column
            or head.second < 1 or head.second > row)
    {
        //出界，报废

    }
    else{
        auto items = collidingItems();
        foreach(QGraphicsItem* item, items){
            food* apple = dynamic_cast<food*>(item);
            obstacles* obstacle = dynamic_cast<obstacles*>(item);
            if(apple!=nullptr)
                ;
            else if(obstacle!=nullptr)
                ;//报废
            else{

            }
        }
    }
}
*/

void snake::eatFood(){
    toGrow = 3;
}
