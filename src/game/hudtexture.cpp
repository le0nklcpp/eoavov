#include "hudtexture.h"
namespace game
{
  hudtexture hudtextures[MAXHUDTEXTURES];
  void renderhudtextures(int w,int h)
  {
   loopi(MAXHUDTEXTURES)
    {
     if(hudtextures[i].endtime<=lastmillis)continue;
     hudtexture &t = hudtextures[i];
     renderhudtexture(t.image,t.x,t.y,w,h,t.xs,t.ys);
    }
  }
  void clearhudtextures()
  {
   loopi(MAXHUDTEXTURES)hudtextures[i].endtime=0;
  }
};