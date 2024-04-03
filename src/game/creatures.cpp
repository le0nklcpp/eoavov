#include "eoavov.h"
vector <creatureType*>creatureClasses;
int register_creature_class(int maxhealth,int mindamage,int maxdamage,int minskill,int maxskill,int minmoral,int maxmoral,int minstr,int maxstr,int team)
{
 creatureClasses.add(new creatureType(maxhealth,mindamage,maxdamage,minskill,maxskill,minmoral,maxmoral,minstr,maxstr,team));
 return creatureClasses.length()-1;
}
