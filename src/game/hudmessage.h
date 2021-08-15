#ifndef __GAME_HUDMSG_H
#define __GAME_HUDMSG_H
#include "eoavov.h"
#define MAXHUDCHANNELS 12
struct hudmessage{
bool enabled;
int starttime,endtime;
float x,y;
string text;
int r,g,b;
hudmessage();
 // posx,posy - position,(start/stop)delay - delay before the message shown and  the message will disappear after being shown in milliseconds
};
namespace game
{
extern int hudmessagecolors[3];
extern hudmessage hmessages[MAXHUDCHANNELS];

extern void clearhudmessages();
extern void disableoldhudmessages();
};
#endif