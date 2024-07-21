#ifndef __EOAVOVCREATURES_H
#define __EOAVOVCREATURES_H
#include "eoavov.h"
#include "arrayassoc.h"
enum { //aistate
CR_SCARED=1,
CR_ANGER,
CR_RAGE,
CR_FOLLOW,
CR_PROTECT,
CR_RUNAWAY,
CR_RETREAT
};
struct creatureEntity;
struct AIAvoidZone:envZone
{
fpsEntity*attached;
AIAvoidZone(vec o,float rad,fpsEntity*ent):envZone(pos,rad,0,(const char*)"\0"){attached = ent;} // zone will move with attached entity if any.
~AIAvoidZone(){}
};
#define MAX_KNOWN_POSITIONS 4
class aiManager
{
    public:
    int enemypositions;
    string name;
    virtual bool avoid(creatureEntity *parent,int avoiddir);
    virtual void init(creatureEntity *parent) {}
    virtual void move(creatureEntity *parent) {}
    virtual void think(creatureEntity *parent);
    virtual void attacked(creatureEntity *parent,fpsEntity *attacker, uchar damagetype,vec hitpos) {}
    virtual void touched(creatureEntity *parent,fpsEntity *ent){}
    virtual bool knows(fpsEntity *parent,fpsEntity *ent);
    aiManager(const char*n){copystring(name,n);enemypositions = MAX_KNOWN_POSITIONS;}
    ~aiManager(){}
};
aiManager* findManager(const char *name);
struct creatureType
{
 int maxhealth,mindamage,maxdamage,minskill,maxskill; // minskill,maxskill - in percent
 int minstrength,maxstrength,minmoral,maxmoral;
 int team;
 bool avoidwater;
 aiManager * manager;
 creatureType(int mh,int mind,int maxd,int mins,int maxs,int minm,int maxm,int minstr,int maxstr,int t,const char *manid,bool avw):maxhealth(mh),mindamage(mind),maxdamage(maxd),minskill(mins),maxskill(maxs),team(t),minmoral(minm),maxmoral(maxm),minstrength(minstr),maxstrength(maxstr),avoidwater(avw){manager = findManager(manid);if(!manager)conoutf(CON_ERROR,"Failed to find AI manager named \"%s\"",manid);}
};

namespace game{
extern hashnameset<aiManager*>creatureManagers;
extern vector<creatureType*>creatureClasses;
void initai();
void clearai();
};
struct creatureEntity:playerEnt,arrayAssoc{
creatureType *crtype;
bool warned; // knows that player is somewhere here
bool controlled; // by scripts
vec vangles; // view angles for entity
int goodattacktime;
uchar cclass,aistate;
int team; // flags
fpsEntity * target;
inventory inv;
vec lastknownenemypos[MAX_KNOWN_POSITIONS];
uchar knownpositions;
creatureEntity(vec pos,int attr1,int attr2,int attr3,int attr4,int attr5):playerEnt(),controlled(false),vangles(0,0,0),team(0),warned(false),goodattacktime(-1),target(NULL),knownpositions(0){setpos(pos);roll = attr1;type = E_CREATURE;crtype = findone(game::creatureClasses,attr2);}
~creatureEntity(){}
void reset(){playerEnt::reset();target = NULL;}
bool avoid();
void findnearesttarget();
bool knows(fpsEntity *target);
void think();
void attacked(fpsEntity *attacker,uchar damagetype,vec hitpos);
void touched(fpsEntity *ent);
};
#include "waypoint.h"
#endif
