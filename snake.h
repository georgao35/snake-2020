#ifndef SNAKE_H
#define SNAKE_H

#include "constant.h"
#include <QGraphicsItem>

class snake:public QGraphicsItem
{

public:
    snake();
    Pii img_to_view(Pii origin){
        return qMakePair(origin.first*TILE_WIDTH-0.5*TILE_WIDTH
                         ,origin.second*TILE_WIDTH-0.5*TILE_WIDTH);
    }
};

#endif // SNAKE_H
