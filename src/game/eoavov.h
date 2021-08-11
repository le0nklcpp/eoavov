#ifndef __EOAMAIN_H
#define __EOAMAIN_H

#include "cube.h"
#include "fpsentity.h"
#include "hudmessage.h"
#include "acoustic.h"
#include "envzones.h"
#include "task.h"
#include "hudtexture.h"

#define GMCMD(a,b,c,d) ICOMMAND(a,b,c,{if(isconnected()){d;}})
#define GMACMD(a,b,c,d) GMCMD(a,b,c,{if(!lock_control)d;})

enum
{

 ENT_LIGHT=ET_LIGHT, // attr1 = radius, attr2 = intensity if attr3==attr4==0 or attr2=red,attr3=green,attr4=blue
 ENT_MAPMODEL=ET_MAPMODEL, // attr1 = model,attr2 = yaw,attr3 = pitch,attr4 = roll,attr5 = scale
 ENT_PLAYERSTART=ET_PLAYERSTART,
 ENT_ENVMAP=ET_ENVMAP,
 ENT_PARTICLES=ET_PARTICLES,
 ENT_SOUND=ET_SOUND,
 ENT_SPOTLIGHT=ET_SPOTLIGHT,
 ENT_DECAL=ET_DECAL,
 ENT_PROP, // attr1 = propid,attr2 = yaw,attr3 = pitch, attr4 = roll,attr5 = tag
// ENT_TRIGGER, // attr1 = flags, attr2 = tag
 ENT_VEHICLE,
 ENT_CREATURE,
 ENT_ITEM, // attr1 = id, attr2 = yaw, attr3 = pitch, attr4 = roll, attr5 = tag
 ENT_TELEPORT,
 ENT_TELEDEST,
 ENT_RAIL,
 ENT_CUSTOM
};
enum 
{
 E_PLAYER = ENT_PLAYER,
 E_MOVABLE = 4,
 E_VEHICLE = 5,
 E_CREATURE = 6,
 E_INVITEM = 7
};

#include "inventory.h"
#include "rpgobject.h"
#include "player.h"
#include "creatures.h"
#include "prop.h"

static const char * const animnames[] =
{
    "mapmodel",
    "dead", "dying",
    "idle", "run N", "run NE", "run E", "run SE", "run S", "run SW", "run W", "run NW",
    "jump", "jump N", "jump NE", "jump E", "jump SE", "jump S", "jump SW", "jump W", "jump NW",
    "sink", "swim", "reload",
    "crouch", "crouch N", "crouch NE", "crouch E", "crouch SE", "crouch S", "crouch SW", "crouch W", "crouch NW",
    "crouch jump", "crouch jump N", "crouch jump NE", "crouch jump E", "crouch jump SE", "crouch jump S", "crouch jump SW", "crouch jump W", "crouch jump NW",
    "crouch sink", "crouch swim", "crouch reload"
    "shoot", "melee",
    "pain", "taunt",
    "gun idle", "gun shoot", "gun shoot2", "gun reload",
    "vwep idle", "vwep shoot","ywepp shoot2", "ywep reload",
    "kick"
};
enum{
    ANIM_DEAD = ANIM_GAMESPECIFIC, ANIM_DYING,
    ANIM_IDLE, ANIM_RUN_N, ANIM_RUN_NE, ANIM_RUN_E, ANIM_RUN_SE, ANIM_RUN_S, ANIM_RUN_SW, ANIM_RUN_W, ANIM_RUN_NW,
    ANIM_JUMP, ANIM_JUMP_N, ANIM_JUMP_NE, ANIM_JUMP_E, ANIM_JUMP_SE, ANIM_JUMP_S, ANIM_JUMP_SW, ANIM_JUMP_W, ANIM_JUMP_NW,
    ANIM_SINK, ANIM_SWIM, ANIM_RELOAD,
    ANIM_CROUCH, ANIM_CROUCH_N, ANIM_CROUCH_NE, ANIM_CROUCH_E, ANIM_CROUCH_SE, ANIM_CROUCH_S, ANIM_CROUCH_SW, ANIM_CROUCH_W, ANIM_CROUCH_NW,
    ANIM_CROUCH_JUMP, ANIM_CROUCH_JUMP_N, ANIM_CROUCH_JUMP_NE, ANIM_CROUCH_JUMP_E, ANIM_CROUCH_JUMP_SE, ANIM_CROUCH_JUMP_S, ANIM_CROUCH_JUMP_SW, ANIM_CROUCH_JUMP_W, ANIM_CROUCH_JUMP_NW,
    ANIM_CROUCH_SINK, ANIM_CROUCH_SWIM,
    ANIM_SHOOT, ANIM_MELEE,
    ANIM_PAIN,
    ANIM_EDIT, ANIM_LAG, ANIM_TAUNT, 
    ANIM_GUN_IDLE, ANIM_GUN_SHOOT, ANIM_GUN_MELEE,
    ANIM_VWEP_IDLE, ANIM_VWEP_SHOOT, ANIM_VWEP_MELEE,
    NUMANIMS
};
extern playerEnt*player1;

extern int lock_control;
namespace game
{
 /*
  render functions
 */
 extern void cubeevent(const char*str);
 extern void rendergame();
 extern void renderfpsents();
 extern void renderplayer();
 extern void createfpsent(int type,vec pos,int attr1,int attr2,int attr3,int attr4,int attr5);
 /*

 */
 extern void recountspeed(bool togglewalk);
};
namespace entities
{
 extern void resetspawns();
 extern void spawnitems(bool force);
};
#endif
