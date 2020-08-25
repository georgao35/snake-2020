#ifndef CONSTANT_H
#define CONSTANT_H

#include "universal_functions.h"
#include <QPair>
#include <QDebug>

typedef QPair<int,int> Pii;

const int TILE_WIDTH = 30;

const int FOOD_R = 25;

const int row = 20;
const int column = 20;

const Pii startHead = qMakePair(5,5);
const Pii startBody = qMakePair(5,6);
/*


Pii view_to_img(Pii view){
    return qMakePair(view.first/TILE_WIDTH+1,view.second/TILE_WIDTH+1);
}*/

#endif // CONSTANT_H
