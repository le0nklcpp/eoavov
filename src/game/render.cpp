#include "eoavov.h"
namespace game
{
  float swaydist = 2.0;
  vec swaydir(0, 0, 0);
  VAR(hudgun, 0, 1, 1);
  VAR(hudgunsway, 0, 1, 1);
  FVAR(swaystep, 1, 35.0f, 100);
  FVAR(swayside, 0, 0.10f, 1);
  FVAR(swayup, -1, 0.15f, 1);
  VAR(showhud,0,1,1);
  VAR(showhudmsg,0,1,1);
  VAR(showwaypoints,0,0,1);
  void renderwaypoints()
  {
      int len = ai::waypoints.length();
      loopi(len)
      {
          loopj(ai::MAXWAYPOINTLINKS)
          {
              int link = ai::waypoints[i].links[j];
              if(!link) break;
              particle_flare(ai::waypoints[i].o, ai::waypoints[link].o, 1, PART_STREAK, 0x0000FF);
          }
      }
  }
  void rendergame()
  {
   renderfpsents();
   renderplayer();
   if(showwaypoints)renderwaypoints();
  }
  void renderfpsents()
  {
   loopv(fpsents)
    {
     fpsEntity &e = *fpsents[i];
     if(!e.visible)continue;
     int anim = (e.animsequence!=-1?e.animsequence+ANIM_GAMESPECIFIC:ANIM_MAPMODEL)|ANIM_LOOP;
     rendermodel(e.model,anim,e.feetpos(),e.yaw,e.pitch,e.roll,MDL_CULL_VFC | MDL_CULL_OCCLUDED | MDL_CULL_QUERY,NULL,NULL,e.lastanim?e.lastanim:lastmillis);
    }
  }
  void renderplayer()
  {
  }
  void drawhudmodel()
  {
	if(!showhud||player1->vmodel[0]=='\0')return; // Nothing to draw
	vec sway;
	vecfromyawpitch(player1->yaw, 0, 0, 1, sway);
	float steps = swaydist/swaystep*M_PI;
	if(!hudgunsway)sway = player1->o;
        else
         {
	 sway.mul(swayside*cosf(steps));
	 sway.z = swayup*(fabs(sinf(steps)) - 1);
	 sway.add(swaydir).add(player1->o);
         }
	//modelattach a[2];
	//player1->muzzle = vec(-1, -1, -1);
	//a[0] = modelattach("tag_muzzle", &player1->muzzle);
	rendermodel(player1->vmodel, player1->vanim, sway, player1->yaw, player1->pitch, 0, MDL_NOBATCH, NULL, NULL/*a*/, player1->lastvanimtime, 0, 1);
	if(player1->muzzle.x >= 0) player1->muzzle = calcavatarpos(player1->muzzle, 12);
  }
  void findanims(const char *pattern, vector<int> &anims)
  {
     loopi(sizeof(animnames)/sizeof(animnames[0])) if(matchanim(animnames[i], pattern)) anims.add(i);
  }
  void renderavatar()
  {
    drawhudmodel();
  }
  void renderplayerpreview(int model, int color, int team, int weap)
  {
   return;
  }
  int numanims() { return NUMANIMS; }

};
