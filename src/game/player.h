#ifndef __GAME__PLAYER_H
#define __GAME__PLAYER_H
#include "eoavov.h"
extern float PLAYER_DEFAULT_SPEED,PLAYER_WALK_SPEED,MAX_INTERACT_DIST;
extern int NEXT_USE_DELAY;

struct playerEnt:fpsEntity,RPGObject{
 string vmodel; // HUD model
 fpsEntity*carries;
 bool light; // toggle flashlight
 int lastvanimtime,lastattacktime,lastdamagetime,nextattacktime,nextinteracttime; // nextinteracttime is for "use" command
 int vanim; // HUD model animation sequence
 vec muzzle; // weapon muzzle for rendering
 playerEnt():light(false),vanim(0),vmodel(""),carries(NULL),lastvanimtime(0),lastdamagetime(0),nextattacktime(0),lastattacktime(-1),nextinteracttime(0),muzzle(-1,-1,-1),fpsEntity(100){type = E_PLAYER;inv.maxweight = 80;movable = true;}
 void reset();
 void setvmodel(const char*path,int anim,int animtime);
 void moveitem();
 void move();
 void dropent();
 void grabent(fpsEntity*e);
 void killed(fpsEntity*killer);
 void quickswitch();
 void think();
 void attack(bool down);
 bool setev(int attr,const char*val);
 invItem* draw(int index);
};
void fixcarrydist();
#endif
