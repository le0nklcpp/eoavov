#include "hudtexture.h"
namespace game
{
  hudtexture hudtextures[MAXHUDTEXTURES];
  void clearhudtextures()
  {
   loopi(MAXHUDTEXTURES)hudtextures[i].endtime=0;
  }
  ICOMMAND(set_hudtexture,"isiffff",(int*c,const char*file,int *t,float*x,float*y,float*w,float*h),{
   int channel = *c;
   if(channel>=MAXHUDTEXTURES)return;
   if(channel<0)
    {
     channel = 0;
     loopi(MAXHUDTEXTURES)
      {
       if(hudtextures[i].endtime<=lastmillis){channel = i;break;}
      }
    }
   hudtexture *tx = &hudtextures[channel];
   tx->set(file,*t,*x,*y,*w,*h);
  });
};