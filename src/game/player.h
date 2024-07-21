#ifndef __GAME__PLAYER_H
#define __GAME__PLAYER_H
#include "eoavov.h"
extern float PLAYER_DEFAULT_SPEED,PLAYER_WALK_SPEED,MAX_INTERACT_DIST;
extern int NEXT_USE_DELAY;

struct playerEnt:fpsEntity,RPGObject{
 string vmodel; // HUD model
 fpsEntity*carries;
 bool light; // toggle flashlight
 bool doublejump;
 int lastvanimtime,lastattacktime,lastdamagetime,nextattacktime,nextinteracttime; // nextinteracttime is for "use" command
 int vanim; // HUD model animation sequence
 vec muzzle; // weapon muzzle for rendering
 int lastnode;
 int prevnodes[6]; // waypoints generation
 playerEnt():light(false),lastnode(-1),doublejump(false),vanim(0),vmodel(""),carries(NULL),lastvanimtime(0),lastdamagetime(0),nextattacktime(0),lastattacktime(0),nextinteracttime(0),muzzle(-1,-1,-1),fpsEntity(100){tag = PLAYER_ENT_TAG;type = E_PLAYER;inv.maxweight = 80;movable = true;}
 void reset();
 void setvmodel(const char*path,int anim,int animtime);
 void removeitem(int index);
 void dropitem(int index);
 void moveitem();
 void move();
 void falldamage(int ftime);
 void dropent();
 void grabent(fpsEntity*e);
 void killed(fpsEntity*killer);
 void quickswitch();
 void think();
 void attack(bool down);
 bool setev(int attr,const char*val);
 bool getev(int attr);
 invItem* draw(int index);
};
void fixcarrydist();
#endif
