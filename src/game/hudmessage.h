#ifndef __GAME_HUDMSG_H
#define __GAME_HUDMSG_H
#include "eoavov.h"
#define MAXHUDCHANNELS 12
struct hudmessage{
int starttime,endtime;
float x,y;
string text;
Uint8 r,g,b,alpha;
hudmessage();
 // posx,posy - position,(start/stop)delay - delay before the message shown and  the message will disappear after being shown in milliseconds
};
namespace game
{
extern Uint8 hudmessagecolors[3];
extern hudmessage hmessages[MAXHUDCHANNELS];
extern void renderhudmsg(int w,int h);
extern void clearhudmessages();
extern void disableoldhudmessages();
};
#endif