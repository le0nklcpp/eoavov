#include "debris.h"
VARP(debris_remove_time,0,0,300000); // 0 = do not remove
VARP(debris_kick_normal,0,90,2000);
#define kickvec vec(debris_kick_normal,debris_kick_normal,debris_kick_normal)

debrisEnt::debrisEnt(vec pos,vec deviation,vec dir,vec v,string model):fpsEntity()
{
    setpos(vec(pos).add(game::randomspread(deviation)));
    vel = game::randomspread(v);
    setangle(dir.x,dir.y,dir.z);
    setmodel(model);
    movable = true;
    if(!debris_remove_time)state = CS_ALIVE;
    nextthink = lastmillis + debris_remove_time;
    type = E_FRAGMENT;
}
inline void debrisEnt::move()
{
    if(bounce(this,0.6,0.5,1)&&collideplayer) // bounce returns true when it collides
    {
        vel.add(kickvec.rotate_around_z(sincosmod360(collideplayer->yaw)));
    }
    if(!onfloor())
    {
        vec oa(yaw,pitch,roll);
        vec sp = game::randomspread(vec(30,30,30));
        setangle(oa.x+sp.x,oa.y+sp.y,oa.z+sp.z);
    }
    else
    {
        pitch = 90;
        roll = 0;
    }
}
void add_debris(char*p,char*e,char*d,char*v,char*s,int* count)
{
    if(!isconnected())return;
    int pos[3],dev[3],dir[3],vel[3];
    parsearray(p,pos);
    parsearray(e,dev);
    parsearray(d,dir);
    parsearray(v,vel);
    loopi(*count)game::fpsents.add(new debrisEnt(vec(pos),vec(dev),vec(dir),vec(vel),s));
}
COMMANDN(create_debris,add_debris,"sssssi");