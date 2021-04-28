#ifndef __GAME_GAMEZONES_
#define __GAME_GAMEZONES_

#include "eoavov.h"
enum{
ZONE_ACOUSTIC=0,
ZONE_UNARMED,
ZONE_TRIGGER
};
struct envZone{
vec pos;
float radius;
uchar type;
string arg; // command to execute
bool active; // player already activated zone by being in it
envZone(vec o,float r,uchar z,const char*a){pos = o;radius = r;type = z;copystring(arg,a);active = false;}
~envZone(){}
};
namespace game{
extern vector <envZone*>gamezones;
#define inAcousticZone(l) inzone(l,ZONE_ACOUSTIC)
#define inUnarmedZone(l) inzone(l,ZONE_UNARMED)
extern void cleargamezones();
extern bool inzone(vec pos,int ztype);
extern void zonetriggers(vec pos);
};
#endif
