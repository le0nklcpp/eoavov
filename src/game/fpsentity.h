#ifndef __FPSENTITY__
#define __FPSENTITY__
#include "eoavov.h"

enum{
EV_HEALTH=0,
EV_MAXHEALTH,
EV_VISIBLE,
EV_MOVABLE,
EV_NEXTTHINK,
EV_MODEL,
EV_TEAM,
EV_ANIMSEQUENCE,
EV_FRAMERATE,
EV_LASTANIM,
EV_MASS,
EV_STATE,
EV_NEXTATTACKTIME,
EV_NEXTINTERACTTIME,
EV_ITEMPTR,
EV_IVAR1,
EV_IVAR2,
EV_IVAR3,
EV_IFVAR1,
EV_IFVAR2,
EV_IFVAR3,
EV_VEL,
EV_ATPOS,
EV_POS,
EV_ANGLES,
EV_WATERPHYS
};
struct fpsEntity;
namespace game{
 extern vector<fpsEntity*>fpsents;
 extern void fpsthink();
 extern void clearfpsents();
 extern void clearfpsroutes();
 extern void preparemovables();
 extern int fpsfindent(int tag,int type=-1);
 extern fpsEntity* getfpsent(int tag,int type=-1); // don't specify type to search through all entities
 extern void fpsremove(int index);
 extern void fpsremove(fpsEntity*e);
 extern void fpsremovebytag(int tag);
 extern bool fpsintersect(fpsEntity &e, const vec &o, const vec &ray,float maxdist);
 extern void fpsdetach(fpsEntity *e);
 extern fpsEntity* rayfpsent(const vec& from, const vec& to,float maxdist,bool thruwalls);
 extern vector<fpsEntity*>&rayfpsentchain(const vec & from, const vec & to, float maxdist, bool thruwalls);// WARNING: Never clear this using deletecontents()
 namespace threads
 {
  extern SDL_mutex*entities;
 }
};
struct fpsEntity:dynent{ // dynamic entity
 bool waterphys;
 int tag;
 float health,maxhealth;
 bool visible,movable;
 int nextthink;
 string model;
 uchar team;
 int animsequence; // animsequence - index of model animation
 short framerate;  // frames per second
 int lastanim;
 float mass;
 fpsEntity*attached;
 vec atpos; // attachedpos - relative
 routeManager route;
 fpsEntity(int hp):attached(NULL),waterphys(false),movable(false),mass(0.),maxhealth(hp),health(hp),tag(-1),animsequence(-1),framerate(0),lastanim(0),team(0),nextthink(0),visible(true),model(""){state = CS_DEAD;type=-1;}
 fpsEntity():fpsEntity(100){}
 virtual ~fpsEntity(){detach();}
 void turnto(const vec &d);
 virtual bool draggable();
 virtual bool headinwater();
 virtual bool onfloor();
 virtual vec cameradir();
 virtual vec worlddir();
 virtual void mirror(short axis); // 1 - X, 2 - Y, 3 - Z
 virtual void think();
 virtual void move();
 virtual void setmodel(const char*modelname);
 virtual void touched(fpsEntity*ent,vec side);
 virtual void hit(fpsEntity*attacker,float damage,uchar damagetype);
 virtual void attacked(fpsEntity*attacker,float damage,uchar damagetype,vec hitpos);
 virtual void falldamage(int ftime);
 virtual void killed(fpsEntity*killer);
 virtual void playerused(); // playerEnt * player1
 virtual void attach(fpsEntity*ent,vec pos);
 virtual void detach();
 virtual void setpos(vec o);
 virtual void setangle(int y=-1,int p=-1,int r=-1); // -1 = don't change
 virtual bool setev(int attr,const char*val); // for scripting purposes.Returns false if it didnt find value to set.
 virtual bool getev(int attr);
 virtual void setroute(rail*r,bool revert=false);
 virtual void stoproute();
 virtual void reset();
};
#endif
