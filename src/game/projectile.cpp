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
  void think(projectileEnt*ent)
  {
   if(!thinkhook[0])return;
   gmsetvar("projectile_ent",intval(ent->tag));
   cubeevent(thinkhook);
  }
  void onhit(projectileEnt*ent,vec hitpos,fpsEntity*hit)
  {
   if(!hithook[0])return;
   gmsetvar("projectile_ent",intval(ent->tag));
   gmsetvar("projectile_hit_x",floatval(hitpos.x));
   gmsetvar("projectile_hit_y",floatval(hitpos.y));
   gmsetvar("projectile_hit_z",floatval(hitpos.z));
   gmsetvar("projectile_target",intval(hit->tag));
   cubeevent(hithook);
  }
  void oncreate(projectileEnt*ent)
  {
   if(!createhook[0])return;
   gmsetvar("projectile_ent",intval(ent->tag));
   cubeevent(createhook);
  }
 };
 void clearprojtypes()
 {
  projtypes.deletecontents();
 }
 ICOMMAND(projtype_create,"sss",(char*th,char*hh,char*ch),{projtypes.add(new scriptedProjectileType(th,hh,ch));intret(projtypes.length()-1);});
 ICOMMAND(create_projectile,"iffffff",(int*index,float*mx,float*my,float*mz,float*tx,float*ty,float*tz),{fpsents.add(new projectileEnt(vec(*mx,*my,*mz),vec(*tx,*ty,*tz),*index));});
};