#include "eoavov.h"
#define MAXHUDTEXTURES 12
struct hudtexture{
string image;
float endtime;
float x,y;
float xs,ys; // scale X, scale Y
hudtexture(){}
~hudtexture(){}
};
namespace game{
void clearhudtextures();
};