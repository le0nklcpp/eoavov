#include "eoavov.h"
vector <creatureType*>creatureClasses;
int register_creature_class(int maxhealth,int mindamage,int maxdamage,int minskill,int maxskill,int team)
{
 creatureClasses.add(new creatureType(maxhealth,mindamage,maxdamage,minskill,maxskill,team));
 return creatureClasses.length()-1;
}
