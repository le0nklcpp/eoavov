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
struct AIAvoidZone:envZone
{
fpsEntity*attached;
AIAvoidZone(vec o,float rad,fpsEntity*ent):envZone(pos,rad,0,(const char*)"\0"){attached = ent;} // zone will move with attached entity if any.
~AIAvoidZone(){}
};
class aiManager
{
    public:
    string name;
    virtual void think(fpsEntity *parent) {}
    virtual void attacked(fpsEntity *parent,fpsEntity *attacker, uchar damagetype,vec hitpos) {}
    virtual void touched(fpsEntity *parent,fpsEntity *ent){}
    aiManager(const char*n){copystring(name,n);}
    ~aiManager(){}
};
extern vector<aiManager*>creatureManagers;
aiManager* findManager(const char *name);
struct creatureType{
int maxhealth,mindamage,maxdamage,minskill,maxskill; // minskill,maxskill - in percent
int minstrength,maxstrength,minmoral,maxmoral;
int team;
aiManager * manager;
creatureType(int mh,int mind,int maxd,int mins,int maxs,int minm,int maxm,int minstr,int maxstr,int t,const char *manid):maxhealth(mh),mindamage(mind),maxdamage(maxd),minskill(mins),maxskill(maxs),team(t),minmoral(minm),maxmoral(maxm),minstrength(minstr),maxstrength(maxstr){manager = findManager(manid);if(!manager)conoutf(CON_ERROR,"Failed to find AI manager named \"%s\"",manid);}
};
extern vector<creatureType*>creatureClasses;
struct creatureEntity:playerEnt{
creatureType *crtype;
bool warned; // knows that player is somewhere here
bool controlled; // by scripts
vec vangles; // view angles for entity
int goodattacktime;
uchar cclass,aistate;
int team; // flags
fpsEntity * target;
inventory inv;
vec lastknownenemypos[4];
creatureEntity(vec pos,int attr1,int attr2,int attr3,int attr4,int attr5):playerEnt(),controlled(false),vangles(0,0,0),team(0),warned(false),goodattacktime(-1)
{
setpos(pos);roll = attr1;type = E_CREATURE;crtype = findone(creatureClasses,attr2);}
~creatureEntity(){}
void think();
void attacked(fpsEntity *attacker,uchar damagetype,vec hitpos);
void touched(fpsEntity *ent);
};
#endif
