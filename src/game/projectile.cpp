#include "projectile.h"
namespace game
{
 vector<projectileType*>projtypes;
 struct scriptedProjectileType:projectileType
 {
  string thinkhook,hithook,createhook;
  scriptedProjectileType(char*th,char*hh,char*ch)
  {
   copystring(thinkhook,th);
   copystring(hithook,hh);
   copystring(createhook,ch);
  }
  ~scriptedProjectileType(){}
  VAR(projectile_ent,-32767,0,32767);
  VAR(projectile_target,-32767,0,32767);
  FVAR(projectile_hit_x,-FLT_MIN,0,FLT_MAX);
  FVAR(projectile_hit_y,-FLT_MIN,0,FLT_MAX);
  FVAR(projectile_hit_z,-FLT_MIN,0,FLT_MAX);
  void think(projectileEnt*ent)
  {
   if(!thinkhook[0])return;
   projectile_ent = ent->tag;
   cubeevent(thinkhook);
  }
  void onhit(projectileEnt*ent,vec hitpos,fpsEntity*hit)
  {
   if(!hithook[0])return;
   projectile_ent = ent->tag;
   projectile_hit_x = hitpos.x;
   projectile_hit_y = hitpos.y;
   projectile_hit_z = hitpos.z;
   projectile_target = hit->tag;
   cubeevent(hithook);
  }
  void oncreate(projectileEnt*ent)
  {
   if(!createhook[0])return;
   projectile_ent = ent->tag;
   cubeevent(createhook);
  }
 };
 void clearprojtypes()
 {
  projtypes.deletecontents();
 }
 ICOMMAND(projtype_create,"sss",(char*th,char*hh,char*ch),{projtypes.add(new scriptedProjectileType(th,hh,ch));intret(projtypes.length()-1);});
 ICOMMAND(create_projectile,"iiffffff",(int*index,int*tag,float*mx,float*my,float*mz,float*tx,float*ty,float*tz),{fpsents.add(new projectileEnt(vec(*mx,*my,*mz),vec(*tx,*ty,*tz),*index,*tag));});
};