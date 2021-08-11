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
 static inline void drawtexture(const char*img,float x,float y,int sw,int sh,float imw,float imh)
  {
   #define kx sw*1800/sh
   #define ky 1650
   float ix = kx*x,iy=ky*y;
   float ix2 = ix+kx*imw;
   float iy2 = iy+ky*imh;
   settexture(img);
   gle::defvertex(2);
   gle::deftexcoord0();
   gle::begin(GL_TRIANGLE_STRIP);
   gle::attribf(ix,iy); gle::attribf(0,0);
   gle::attribf(ix2,iy); gle::attribf(1,0);
   gle::attribf(ix,iy2); gle::attribf(0,1);
   gle::attribf(ix2,iy2); gle::attribf(1,1);
   gle::end();
  }
 // x,y - coordinates on the screen from 0 to 1, sw,sh - screen width and screen height, imw,imh - width and height on the screen
 void clearhudtextures();
};