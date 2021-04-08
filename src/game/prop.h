#ifndef __GAME__PROPENTS
#define __GAME__PROPENTS
#include "eoavov.h" 
struct proptype
{
 int health;
 int id;
 float mass;
 bool movable,portable; // portable - player can take prop
 char movehook[48],breakhook[48],hithook[48],drophook[48],usehook[48];
 string model;
 proptype();
 ~proptype() {}
};
extern vector<proptype*>proptypes;
struct propEnt:fpsEntity
{
 int propid;
 propEnt(vec pos,int attr1,int attr2,int attr3,int attr4,int attr5): propid(attr1)
  {
   tag = attr5;
   if(!proptypes.inrange(propid))
    {
     killed(this);
     return;
    }
   state = CS_ALIVE;
   yaw = attr2;
   pitch = attr3;
   roll = attr4;
   proptype &p = *proptypes[propid];
   health = maxhealth = p.health;
   mass = p.mass;
   movable = p.movable;
   setmodel(p.model);
   type = E_MOVABLE;
   o=pos;
  }
 ~propEnt(){}
 void attacked(fpsEntity*attacker,float damage,uchar damagetype,vec hitpos);
 void killed(fpsEntity*killer);
 void playerused();
 void dropped(); // specific function
 void falldamage(int ftime);
};
#endif
