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
 propEnt(extentity &e): propid(e.attr1)
  {
   tag = e.attr5;
   if(!proptypes.inrange(propid))
    {
     killed(this);
     return;
    }
   state = CS_ALIVE;
   yaw = e.attr2;
   pitch = e.attr3;
   roll = e.attr4;
   proptype &p = *proptypes[propid];
   health = maxhealth = p.health;
   mass = p.mass;
   movable = p.movable;
   setmodel(p.model);
   type = E_MOVABLE;
   o=e.o;
  }
 ~propEnt(){}
 void attacked(fpsEntity*attacker,float damage,uchar damagetype,vec hitpos);
 void killed(fpsEntity*killer);
 void playerused();
 void dropped(); // specific function
 void falldamage(int ftime);
};
#endif
