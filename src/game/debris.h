#if !defined __DEBRIS_H
#define __DEBRIS_H

#include "eoavov.h"
struct debrisEnt:fpsEntity{
 debrisEnt(vec pos,vec deviation,vec dir,vec v,string model);
 ~debrisEnt(){}
 void move();
};

#endif