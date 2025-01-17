#include "eoavov.h"
aiManager *findManager(const char*name)
{
    return *game::creatureManagers.access(name);
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
bool creatureEntity::knows(fpsEntity *ent)
{
    if(aimanexists)return crtype->manager->knows(this,ent);
    return false;
}
inline void creatureEntity::findnearesttarget()
{
    target = NULL;
    float closest = FLT_MAX;
    loopv(game::fpsents)
    {
        fpsEntity &e = *game::fpsents[i];
        if((e.type!=E_CREATURE&&e.type!=E_PLAYER)||(e.team&team)||!knows(&e))continue;
        float dist = vec(o).dist(e.o);
        if(dist<closest)
        {
            closest = dist;
            target = &e;
        }
    }
}
void creatureEntity::updateenemypos(const vec &o)
{
    loopi(knownpositions)
    {
        if(lastknownenemypos[i].dist(o)<KNOWN_POS_THRESHOLD)
        {
            lastknownenemypos[i] = o;
            return;
        }
    }
    if(knownpositions<(MAX_KNOWN_POSITIONS-1))
    {
        knownpositions++;
        lastknownenemypos[knownpositions] = o;
    }
}
extern const float JUMPVEL;
inline bool creatureEntity::avoid()
{
    int mat = lookupmaterial(o);
    // right now there are no things to avoid, but later we need to count our chances on everything at this pos in this moment
    return isdeadly(mat)||(crtype&&crtype->avoidwater&&isliquid(mat));
}
enum
{
 AI_AVOID_FORWARD=0,
 AI_AVOID_BACKWARD,
 AI_AVOID_LEFT,
 AI_AVOID_RIGHT,
 AI_AVOID_BOTTOM,
 AI_AVOID_TOP
};
bool aiManager::avoid(creatureEntity*parent,int avoiddir)
{
    vec pos = parent->o, m = vec(0,0,0);
    int move=0,strafe=0,pitch=0;
    switch(avoiddir)
    {
        case(AI_AVOID_FORWARD):
        move = 1;
        break;
        case(AI_AVOID_BACKWARD):
        move = -1;
        break;
        case(AI_AVOID_LEFT):
        strafe = 1;
        break;
        case(AI_AVOID_RIGHT):
        strafe = -1;
        break;
        case(AI_AVOID_BOTTOM):
        pitch = -1;
        break;
        case(AI_AVOID_TOP):
        pitch = 1;
        break;
    }
    vecfromyawpitch(parent->yaw,90*pitch,move,strafe,m);
    parent->o.add(m.mul(parent->maxspeed));
    bool collided = collide(parent,vec(0,0,0),0);
    bool avoids = parent->avoid();
    parent->o = pos;
    return (collided||avoids);
}

inline void aiManager::move(creatureEntity*parent)
{
    if(ai::waypoints.inrange(parent->lastnode))
    {
        ai::waypoint *w = &ai::waypoints[parent->lastnode];
        if(w&&w->o.dist(parent->o)<ai::CLOSEDIST)
        {
            parent->turnto(w->o);
            parent->physent::move = 1;
        }
    }
}
inline bool aiManager::visible(fpsEntity *parent,fpsEntity *ent)
{
    // To be sure you need to always hit the center, so set it to even values
    #define VIS_SCAN_W_DEPTH 4
    #define VIS_SCAN_H_DEPTH 4

    vec a(parent->worlddir());
    vec b(ent->o);
    a.sub(parent->o);
    b.sub(parent->o);
    a.z = b.z = 0;
    int angle = acos(a.dot(b)/(a.magnitude()*b.magnitude()))/RAD;
    if(angle>90)return false; // right behind you
    static float i,j;
    for(i=0.;i<VIS_SCAN_W_DEPTH;i++)
    {
        for(j=0.;j<VIS_SCAN_H_DEPTH;j++)
        {   // physics.cpp: get direction
            vec dir(ent->o);
            vec offs((-(ent->xradius))+(i*ent->xradius*2/VIS_SCAN_W_DEPTH),0,(-(ent->zmargin))+(i*ent->zmargin*2/VIS_SCAN_H_DEPTH));
            offs.rotate_around_z(ent->yaw*RAD);
            offs.rotate_around_x(ent->pitch*RAD);
            dir.add(offs);
            dir.sub(parent->o);
            dir.div(dir.magnitude());
            // TODO: that may cause problems with stealth. As an alternative to artifical stupidity it would be an artifical cheater
            //if(game::rayfpsent(parent->o,dir,1e16f,false)==ent)return true;
            // in case we will see multiple objects.
            vector <fpsEntity*>&rayresult = game::rayfpsentchain(parent->o,dir,1e16f,false); // rayresult will be cleared next time the function is called
            loopv(rayresult)
            {
                if(rayresult[i]==ent)return true;
            }
        }
    }
    return false;
}
inline bool aiManager::knows(creatureEntity*parent,fpsEntity*ent)
{
    if(visible(parent,ent))
    {
        if(ent->type==E_CREATURE)
        {
            if(!(parent->team&ent->team))
            {
                parent->updateenemypos(ent->o);
            }
        }
        return true;
    }
    loopi(parent->knownpositions)
    {
        if(ent->o.dist(parent->lastknownenemypos[i])<KNOWN_POS_THRESHOLD)return true;
    }
    return false;
}
void aiManager::think(creatureEntity *parent)
{
    if(!parent->target||!parent->knows(parent->target))parent->findnearesttarget();
    parent->nextthink = lastmillis + 1400;
}
struct flyingAI : aiManager
{
    #define FLIGHT_TRACK_TIME 800
    flyingAI():aiManager("flyingAI")
    {
        
    }
    void init(creatureEntity *parent)
    {
        creatureEntity &p = *parent;
        p["flightdir"].setValue(0);
        p["flighttime"].setValue(lastmillis + FLIGHT_TRACK_TIME);
    }
    void move(creatureEntity *parent)
    {
        creatureEntity &p = *parent;
        p.physstate = PHYS_FLIGHT;
        if(p["flighttime"].intValue()>lastmillis)
        {
            p["flighttime"].setValue(lastmillis + FLIGHT_TRACK_TIME);
            p["flightdir"].setValue(rnd(63));
            if(p.target)p.turnto(p.target->o);
            else if(p.knownpositions)p.turnto(p.lastknownenemypos[0]);
            else p.yaw += rndscale(90)-45;
        }
        int dir = (p["flightdir"].intValue());
        p.physent::move = ((dir&(1<<1)&&(!avoid(parent,AI_AVOID_FORWARD)))||parent->target)?1:((dir&(1<<2)&&!p.k_up&&!avoid(parent,AI_AVOID_BACKWARD))?-1:0);
        p.strafe = (dir&(1<<3)&&(!avoid(parent,AI_AVOID_LEFT)))?1:((dir&(1<<4)&&!avoid(parent,AI_AVOID_RIGHT))?-1:0);
        if(((!(dir&(1<<5)))||p.o.z<p.target->o.z)&&!avoid(parent,AI_AVOID_TOP))p.vel.add(vec(0,0,(int)p.maxspeed/(rnd(4)+1)));
        if(((!p.jumping&&(dir&1<<6))||p.o.z>p.target->o.z)&&!avoid(parent,AI_AVOID_BOTTOM))p.vel.add(vec(0,0,-(int)p.maxspeed/(rnd(4)+1)));
        p.move();
    }
};
struct genericAI:aiManager
{
    genericAI():aiManager("genericAI")
    {
    }
    void init(creatureEntity *parent)
    {
        parent->nextthink = lastmillis + 1400;
    }
    void move(creatureEntity *parent)
    {
        parent->turnto(parent->target->o);
        parent->physent::move = 1;
        parent->move();
    }
};
namespace game
{
   vector <creatureType*>creatureClasses;
   hashnameset<aiManager*>creatureManagers;
   int register_creature_class(int maxhealth,int mindamage,int maxdamage,int minskill,int maxskill,int minmoral,int maxmoral,int minstr,int maxstr,int team,const char *manid,bool avw)
    {
     creatureClasses.add(new creatureType(maxhealth,mindamage,maxdamage,minskill,maxskill,minmoral,maxmoral,minstr,maxstr,team,manid,avw));
     return creatureClasses.length()-1;
    }
    void initai()
    {
     clearai();
     creatureManagers.add(new genericAI());
     creatureManagers.add(new flyingAI());
    }
    void clearai()
    {
     creatureManagers.clear();
     creatureClasses.deletecontents();
    }
};