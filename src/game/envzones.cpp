#include "eoavov.h"
namespace game{
vector <envZone*>gamezones;
 void cleargamezones()
 {
  gamezones.deletecontents();
 }
#define loopzones(p,zt,action,outsideaction) \
 loopv(gamezones) \
 { \
  envZone &z = *gamezones[i]; \
  if(z.type!=zt)continue; \
  if(z.pos.dist(p)<=z.radius){action;} \
  else {outsideaction;}\
 }
 bool inzone(vec pos,int ztype)
 {
  loopzones(pos,ztype,return true,);
  return false;
 }
 void zonetriggers(vec pos)
 {
  loopzones(pos,ZONE_TRIGGER,if(!z.active){z.active = true;cubeevent(z.arg);},z.active = false);
 }
};
ICOMMAND(set_envzone,"ffffis",(float*x,float*y,float*z,float*r,int*zt,const char*arg),{game::gamezones.add(new envZone(vec(*x,*y,*z),*r,*zt,arg));});
