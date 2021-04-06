#include "eoavov.h"
void cubeevent(const char * str)
 {
 if(identexists(str)) execute(str);
 }
namespace game{
    void sound(const char *s, const vec *loc, int vol , int flags, int loops, int fade, int chanid, int radius, int expire)
    {
     if(loc&&inAcousticZone(*loc))playacoustic(s,loc);
     else playsoundname(s, loc, vol , flags , loops , fade , chanid , radius , expire);
    }
    void epressed() // E
    {
     if(player1->carries)
      {
       player1->dropent();
       return;
      }
     fpsEntity*ent = rayent(player1->o,worldpos,MAX_INTERACT_DIST,false);
     if(!ent)return;
     ent->playerused();
    }
    GMACMD(use,"",(),{if(player1->nextinteracttime<lastmillis){epressed();player1->nextinteracttime = lastmillis + NEXT_USE_DELAY;}});
    void recountspeed(bool togglewalk)
    {
     if(!isconnected())return;
     player1->maxspeed = togglewalk?PLAYER_WALK_SPEED:PLAYER_DEFAULT_SPEED;
    }
    const char *defaultcrosshair(int index)
    {	    
	return "media/interface/crosshair/default.png";
    }
    int selectcrosshair(vec &col)
    {
	return 0;
    } 
    float abovegameplayhud(int w, int h)
    {
	return 1650.0f/1800.0f;
    }
    FVAR(flash_radius,0.0,15.5,1000.0);
    VAR(flash_colorr,0,255,255);
    VAR(flash_colorg,0,255,255);
    VAR(flash_colorb,0,255,255);
    void adddynlights()
    {
        if(player1->light)adddynlight(vec(worldpos).sub(camdir),flash_radius,vec(flash_colorr,flash_colorg,flash_colorb));
    }
    float clipconsole(float w, float h)
    {
        return 0;
    }
    void dynentcollide(physent *d, physent *o, const vec &dir)
    {
 
    }
    bool canjump()
    {
     return true;
    }
    bool cancrouch()
    {
     return true;
    }
    bool allowmove(physent * d)
    {
	return true;
    }
    bool collidecamera()
    {
        return true;
    }
    bool needminimap()
    {
	return false;
    }
    void lightfire_player(vec &o,vec &hud)
    {
        if(player1->muzzle.x < 0 || player1->lastattacktime < 0) return;
        o = player1->muzzle;
        hud = vec(player1->o).add(vec(0, 0, 2));
    }
    void lightfire_creature(creatureEntity*owner, vec &o, vec &hud)
    {
      if(owner->muzzle.x<0||owner->lastattacktime < 0)return;
      o = hud = owner->muzzle;
    }
    void dynlighttrack(physent *owner, vec &o, vec &hud)
    {
        if(owner->type==E_PLAYER)lightfire_player(o,hud);
	if(owner->type==E_CREATURE)lightfire_creature((creatureEntity*)owner,o,hud);
	
    }
    void particletrack_player(vec &o,vec &d)
    {   
        if(player1->muzzle.x < 0 || player1->lastattacktime < 0) return;
        float dist = o.dist(d);
        o = player1->muzzle;
        if(dist <= 0) d = o;
        else
        {
            vecfromyawpitch(player1->yaw, player1->pitch, 1, 0, d);
            float newdist = raycube(player1->o, d, dist, RAY_CLIPMAT|RAY_ALPHAPOLY);
            d.mul(min(newdist, dist)).add(player1->o);
        }
    }
    void particletrack_creature(creatureEntity*owner,vec &o,vec &d)
    {   
        if(owner->muzzle.x < 0 || owner->lastattacktime < 0) return;
        float dist = o.dist(d);
        o = owner->muzzle;
        if(dist <= 0) d = o;
        else
        {
            vecfromyawpitch(owner->yaw, owner->pitch, 1, 0, d);
            float newdist = raycube(owner->o, d, dist, RAY_CLIPMAT|RAY_ALPHAPOLY);
            d.mul(min(newdist, dist)).add(owner->o);
        }
    }
    void particletrack(physent *owner, vec &o, vec &d)
    {
	if(owner->type==E_PLAYER)particletrack_player(o,d);
	if(owner->type==E_CREATURE)particletrack_creature((creatureEntity*)owner,o,d);
    }
    void bounced(physent *d, const vec &surface)
    {

    }
    void resetgamestate()
    {
    }
    void parseoptions(vector<const char *> &args)
    {
    }
    GMCMD(quickswitch,"",(),player1->quickswitch());
    GMCMD(set_hudmodel,"sii",(char*s,int*anim,int*time),player1->setvmodel(s,*anim,*time));
    ICOMMAND(precache_model,"s",(char*fpath),preloadmodel(fpath));
};
