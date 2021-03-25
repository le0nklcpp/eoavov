#include "hudmessage.h"
hudmessage::hudmessage()
{
text[0]='\0';
x=y=0.0;
enabled = false;
r=g=b=255;
}
hudmessage hmessages[MAXHUDCHANNELS];
void set_hudmsg(float posx,float posy,int startdelay,int stopdelay,const char*message,int channel=-1)
{
 if(channel<-1||channel>=MAXHUDCHANNELS)return;
 hudmessage * m = NULL;
 if(channel==-1)
  {
          for(int i=0;i<MAXHUDCHANNELS;i++)
    {
     if(hmessages[i].enabled)continue;
     m = &hmessages[i];
     break;
    }
	   if(!m)m = &hmessages[0]; // There's no free channels left,so we will use this one
  }
 else m = &hmessages[channel];
 m->enabled = true;
 m->x = posx;
 m->y = posy;
 m->starttime = lastmillis + startdelay;
 m->endtime = m->starttime + stopdelay;
 m->r = hudmessagecolors[0];
 m->g = hudmessagecolors[1];
 m->b = hudmessagecolors[2];
 copystring(m->text,message);
}
void clearhudmessages()
{
 for(int i=0;i<MAXHUDCHANNELS;i++)
 {
 hmessages[i].enabled = false;
 }
}
void clearhudmessage(int msgid)
{
 if(msgid<0||msgid>=MAXHUDCHANNELS)return;
 hmessages[msgid].enabled = false;
}
int hudmessagecolors[3]={255,255,255};
void set_hudmsgcolors(int r,int g,int b)
{
 if(r>255||r<0||g>255||g<0||b>255||b<0)return;
 hudmessagecolors[0] = r;
 hudmessagecolors[1] = g;
 hudmessagecolors[2] = b;
}

ICOMMAND(set_hudmessage_color,"iii",(int*r,int*g,int*b),set_hudmsgcolors(*r,*g,*b));
ICOMMAND(set_hudmessage,"ffiisi",(float *posx,float *posy,int *startdelay,int *stopdelay,char*message,int *channel),set_hudmsg(*posx,*posy,*startdelay,*stopdelay,message,*channel));
ICOMMAND(clearhudmessage,"i",(int*index),clearhudmessage(*index));
ICOMMAND(clearhudmessages,"",(),clearhudmessages());