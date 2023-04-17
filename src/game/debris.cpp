#include "debris.h"
VARP(debris_remove_time,0,30000,0); // 0 = do not remove

debrisEnt::debrisEnt(vec pos,vec deviation,vec dir,vec v,string model):fpsEntity()
{
 o = vec(pos).add(game::randomspread(deviation));
 vel = game::randomspread(v);
 setangle(dir.x,dir.y,dir.z);
 setmodel(model);
 if(!debris_remove_time)state = CS_ALIVE;
 nextthink = lastmillis + debris_remove_time;
 type = E_FRAGMENT;
}
inline void debrisEnt::move()
{
 if(bounce(this,0.6,0.5,1))
 {
  vec sp = game::randomspread(vec(90,90,90));
  setangle(sp.x,sp.y,sp.z);
 }
}
void add_debris(char*p,char*e,char*d,char*v,char*s,int count)
{
 int pos[3],dev[3],dir[3],vel[3];
 parsearray(p,pos);
 parsearray(e,dev);
 parsearray(d,dir);
 parsearray(v,vel);
 loopi(count)game::fpsents.add(new debrisEnt(vec(pos),vec(dev),vec(dir),vec(vel),s));
}
COMMANDN(create_debris,add_debris,"sssssi");