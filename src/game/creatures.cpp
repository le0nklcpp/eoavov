#include "eoavov.h"
vector <creatureType*>creatureClasses;
int register_creature_class(int maxhealth,int mindamage,int maxdamage,int minskill,int maxskill,int minmoral,int maxmoral,int minstr,int maxstr,int team,const char *manid)
{
 creatureClasses.add(new creatureType(maxhealth,mindamage,maxdamage,minskill,maxskill,minmoral,maxmoral,minstr,maxstr,team,manid));
 return creatureClasses.length()-1;
}
aiManager *findManager(const char*name)
{
    loopv(creatureManagers)
    {
        if(!strcmp(creatureManagers[i]->name,name))return creatureManagers[i];
    }
    return NULL;
}
#define aimanexists (crtype&&crtype->manager)
void creatureEntity::think()
{
    if(aimanexists)crtype->manager->think(this);
}
void creatureEntity::attacked(fpsEntity *attacker,uchar damagetype,vec hitpos)
{
    if(aimanexists)crtype->manager->attacked(this,attacker,damagetype,hitpos);
}
void creatureEntity::touched(fpsEntity *ent)
{
    if(aimanexists)crtype->manager->touched(this,ent);
}
vector<aiManager*>creatureManagers;