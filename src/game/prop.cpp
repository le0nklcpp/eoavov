#include "cube.h"
#include "prop.h"
vector<proptype*>proptypes;
VAR(prop_health,0,0,32767);
VAR(prop_id,0,0,32767);
FVAR(prop_mass,0.,1.,32767.);
VAR(prop_movable,0,1,1);
VAR(prop_portable,0,1,1);
SVAR(prop_movehook,"");
SVAR(prop_breakhook,"");
SVAR(prop_hithook,"");
SVAR(prop_drophook,"");
SVAR(prop_usehook,"");
SVAR(prop_touchhook,"");
SVAR(prop_model, "");
proptype::proptype()
{
  health = prop_health;
  id = prop_id;
  mass = prop_mass;
  movable = prop_movable;
  portable = prop_portable;
  copystring(movehook,prop_movehook); 
  copystring(breakhook,prop_breakhook);
  copystring(drophook,prop_drophook);
  copystring(usehook,prop_usehook);
  copystring(touchhook,prop_touchhook);
  copystring(model, prop_model);
}
ICOMMAND(push_prop,"",(),{proptypes.add(new proptype());});

VARP(prop_hk_tag, -32767,0,32767);
VARP(prop_hk_type,-32767,0,32767);
FVARP(prop_hk_arg1,-32767.,0.,32767.);
FVARP(prop_hk_arg2,-32767.,0.,32767.);
FVARP(prop_hk_arg3,-32767.,0.,32767.);
FVARP(prop_hk_arg4,-32767.,0.,32767.);
FVARP(prop_hk_arg5,-32767.,0.,32767.);
#define getpt if(!proptypes.inrange(propid))return;proptype &p = *proptypes[propid];
void propEnt::attacked(fpsEntity*attacker,float damage,uchar damagetype,vec hitpos)
{
  if(!attacker)return;
  getpt;
  prop_hk_tag =  attacker->tag;
  prop_hk_type = attacker->type;
  prop_hk_arg1 = damage;
  prop_hk_arg2 = damagetype;
  prop_hk_arg3 = hitpos.x;
  prop_hk_arg4 = hitpos.y;
  prop_hk_arg5 = hitpos.z;
  game::cubeevent(p.hithook);
}
void propEnt::killed(fpsEntity*killer)
{
  fpsEntity::killed(killer); // we mark this entity as dead
  if(!killer)return;
  getpt;
  prop_hk_tag =  killer->tag;
  prop_hk_type = killer->type;
  game::cubeevent(p.breakhook);
}
void propEnt::touched(fpsEntity*ent)
{
  getpt;
  if(!p.touchhook[0])return;
  prop_hk_tag = ent->tag;
  prop_hk_type = ent->type;
  game::cubeevent(p.touchhook);
}
void propEnt::playerused()
{
  getpt;
  if(p.portable)
   {
   player1->grabent(this);
   movable = false; // setting movable to false to exclude unnecessary calculations
   }
  game::cubeevent(p.usehook);
}
void propEnt::dropped()
{
 getpt;
 game::cubeevent(p.drophook);
 movable = p.movable;
 falling = player1->falling;
 vel = player1->vel;
 timeinair = player1->timeinair;
 physstate = PHYS_FALL;
}
void propEnt::falldamage(int ftime)
{

}