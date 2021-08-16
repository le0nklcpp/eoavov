#ifndef __HUDTEXTURES__H_
#define __HUDTEXTURES__H_

#include "eoavov.h"
#define MAXHUDTEXTURES 12
struct hudtexture{
string image;
int endtime;
float x,y;
float xs,ys; // width and height
hudtexture():endtime(0),image(""){}
~hudtexture(){}
void set(const char*file,int time,float x1,float y1,float w,float h)
 {
  copystring(image,file);
  endtime = time;
  x = x1;
  y = y1;
  xs = w;
  ys = h;
 }
};
namespace game{
 extern hudtexture hudtextures[MAXHUDTEXTURES];
 extern void clearhudtextures();

 /* x,y - coordinates on the screen (from 0 to 1)
    sw,sh - screen width and screen height
    imw,imh - width and height on the screen(from 0 to 1)
    srcx,srcy,srcw,srch - coordinates of the displayed texture fragment (from 0 to 1)
 */
 static inline void renderhudtexture(const char*img,float x,float y,int sw,int sh,float imw,float imh,float srcx=0.0,float srcy=0.0,float srcw=1.0,float srch=1.0)
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
   gle::attribf(ix,iy); gle::attribf(srcx,srcy);
   gle::attribf(ix2,iy); gle::attribf(srcw,srcy);
   gle::attribf(ix,iy2); gle::attribf(srcx,srch);
   gle::attribf(ix2,iy2); gle::attribf(srcw,srch);
   gle::end();
  }
 static inline void renderhudtextures(int w,int h)
 {
  loopi(MAXHUDTEXTURES)
   {
    if(hudtextures[i].endtime<=lastmillis)continue;
    hudtexture &t = hudtextures[i];
    renderhudtexture(t.image,t.x,t.y,w,h,t.xs,t.ys);
   }
 }
};

#endif