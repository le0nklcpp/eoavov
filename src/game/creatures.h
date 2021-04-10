#ifndef __EOAVOVCREATURES_H
#define __EOAVOVCREATURES_H
#include "eoavov.h"

enum { //aistate
CR_SCARED=1,
CR_ANGER,
CR_RAGE,
CR_FOLLOW,
CR_PROTECT,
CR_RUNAWAY,
CR_RETREAT
};
enum {
TEAM_PLAYER=0,
TEAM_NEUTRAL,
TEAM_ENEMY
};
struct AIAvoidZone:envZone
{
fpsEntity*attached;
AIAvoidZone(vec o,float rad,fpsEntity*ent):envZone(pos,rad,0,(char*)"\0"){attached = ent;} // zone will move with attached entity if any.
~AIAvoidZone(){}
};
struct creatureType{
int maxhealth,mindamage,maxdamage,minskill,maxskill; // minskill,maxskill - in percent
int team;
creatureType(int mh,int mind,int maxd,int mins,int maxs,int t):maxhealth(mh),mindamage(mind),maxdamage(maxd),minskill(mins),maxskill(maxs),team(t){}
};

extern vector<creatureType*>creatureClasses;
struct creatureEntity:fpsEntity{
int crtype;
bool warned; // knows that player is somewhere here
bool controlled; // by scripts
vec vangles; // view angles for entity
vec muzzle;
int lastattacktime,lastdamagetime,lastsuccessattacktime,nextattacktime;
uchar cclass,team,aistate;
fpsEntity * target;
inventory inv;
vec lastknownenemypos[4];
creatureEntity(vec pos,int attr1,int attr2,int attr3,int attr4,int attr5):crtype(attr2),controlled(false),vangles(0,0,0),muzzle(0,0,0),team(0),warned(false),lastattacktime(-1),lastdamagetime(0),lastsuccessattacktime(0),nextattacktime(0){setpos(pos);roll = attr1;type = E_CREATURE;}
~creatureEntity(){}
};
#endif
