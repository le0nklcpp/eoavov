#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__
#include "eoavov.h"
namespace game
{
 struct projectileEnt;
 struct projectileType
 {
  projectileType(){}
  virtual ~projectileType(){}
  virtual void think(projectileEnt*ent){}
  virtual void onhit(projectileEnt*ent,vec hitpos,fpsEntity*hit){}
  virtual void oncreate(projectileEnt*ent){}
 };
 extern vector<projectileType*>projtypes;
 struct projectileEnt:fpsEntity
 {
  fpsEntity*owner;
  projectileType *projtype;
  projectileEnt(vec pos,vec dir,int t,int et):fpsEntity(0)
  {
   if(!projtypes.inrange(t))return;
   type = E_PROJECTILE;
   projtype = projtypes[t];
   movable = true;
   state = CS_ALIVE;
   setpos(pos);
   yaw = -atan((pos.x-dir.x)/(pos.y-dir.y))/RAD;
   pitch = -atan((pos.y-dir.y)/(pos.z-dir.z))/RAD;
   roll = atan((pos.x-dir.x)/(pos.z-dir.z))/RAD;
   projtype->oncreate(this);
   tag = et;
  }
  void think()
  {
   projtype->think(this);
  }
  void touched(fpsEntity*ent,vec side)
  {
   projtype->onhit(this,vec(o).add(vec(xradius,yradius,zmargin).mul(side)),ent);
  }
 };
 void clearprojtypes();
};
#endif