#include "eoavov.h"
namespace game{
 ICOMMAND(deletedynent,"i",(int*tag),fpsremovebytag(*tag));
 vector<fpsEntity*>fpsents;
 void createfpsent(int type,vec pos,int attr1,int attr2,int attr3,int attr4,int attr5)
 {
   #define fadd(e) fpsents.add(new e(pos,attr1,attr2,attr3,attr4,attr5))
   switch(type)
    {
     case(ENT_PROP):fadd(propEnt);break;
     case(ENT_CREATURE):fadd(creatureEntity);break;
     case(ENT_ITEM):fadd(RPGItemEnt);break;
    }
 }
 GMCMD(spawn_dynent,"ifffiiiii",(int*t,float*x,float*y,float*z,int*a1,int*a2,int*a3,int*a4,int*a5),createfpsent(*t,vec(*x,*y,*z),*a1,*a2,*a3,*a4,*a5));
 int fpsfindent(int tag,int type)
 {
  if(tag!=-1)loopv(fpsents)if(fpsents[i]->tag==tag&&(type==-1||fpsents[i]->type==type))return i;
  return -1;
 }
 fpsEntity*getfpsent(int tag,int type)
 {
  int result = fpsfindent(tag,type);
  return fpsents.inrange(result)?fpsents[result]:NULL;
 }
 void fpsremove(int index)
 {
  if(!fpsents.inrange(index))return;
  delete fpsents.remove(index);
 }
 void fpsremovebytag(int tag)
 {
  int found = fpsfindent(tag);
  if(!fpsents.inrange(found))return;
  delete fpsents.remove(found);
 }
 void fpsthink()
 {
 loopv(fpsents)
  {
   fpsEntity&e = *fpsents[i];
   e.move();
   if(e.nextthink<lastmillis)continue;
   if(e.state==CS_DEAD)
    {
    delete fpsents.remove(i--);
    }
   else e.think();
  }
 }
 void clearfpsents()
 {
  fpsents.deletecontents();
 }
 void clearfpsroutes()
 {
  loopv(fpsents)
  {
   fpsents[i]->setroute(NULL);
  }
 }
 void preparemovables()
 {
/*
  cleardynentcache();
  loopv(fpsents)
   {
   fpsEntity &e=*fpsents[i];
   if(e.movable)e.resetinterp();
   }
*/
 }
 dynent* iterdynents(int i)
 {
  if(!i)return player1;
  return fpsents[i-1];
 }
 int numdynents()
 {
  return fpsents.length()+1;
 }
 bool intersect(fpsEntity *d, const vec &from, const vec &to, float &dist)   // if lineseg hits entity bounding box
 {
  vec bottom(d->o), top(d->o);
  bottom.z -= d->eyeheight;
  top.z += d->aboveeye;
  return linecylinderintersect(from, to, bottom, top, d->radius, dist);
 }
 ICOMMAND(get_dynentsnum,"",(),{intret(fpsents.length());});
 #define PLAYER_ENT_TAG 1337
 #define returnfpsent(id,entname) \
 fpsEntity* entname;\
 if(id==PLAYER_ENT_TAG)entname = player1; \
 else entname = getfpsent(id); \
 if(!entname)return;
 GMCMD(set_dynent_pos,"ifff",(int*tag,float*fx,float*fy,float*fz),
 {
  returnfpsent(*tag,ent);
  ent->setpos(vec(*fx,*fy,*fz));  
 });
 GMCMD(attach_dynent,"iifff",(int*attached,int*tag,float*fx,float*fy,float*fz),
 {
  returnfpsent(*attached,ent);
  returnfpsent(*tag,target);
  if(target!=ent)ent->attach(target,vec(*fx,*fy,*fz)); 
 });
 GMCMD(set_dynent_angle,"iiii",(int*tag,int*yaw,int*pitch,int*roll),
 {
  returnfpsent(*tag,ent);
  ent->setangle(*yaw,*pitch,*roll);
 });
 GMCMD(detach_dynent,"i",(int*tag),
 {
  returnfpsent(*tag,ent);
  ent->detach();
 });
 GMCMD(set_dynent_vel,"ifff",(int*tag,float*vx,float*vy,float*vz),
 {
  returnfpsent(*tag,ent);
  ent->vel = vec(*vx,*vy,*vz);
 });
 GMCMD(remove_dynent,"i",(int*tag),{
  returnfpsent(*tag,ent);
  ent->state = CS_DEAD;
  ent->nextthink = 0;
 });
 fpsEntity* rayfpsent(const vec& from, const vec& to,float maxdist,bool thruwalls)
 {
     float newmaxdist = maxdist?maxdist:130.0;
     vec hitpos;
     if(!thruwalls)newmaxdist = raycubepos(from,to,hitpos,maxdist,RAY_CLIPMAT|RAY_ALPHAPOLY);
     fpsEntity* result = NULL;
     loopv(fpsents)
      {
       float dist;
       fpsEntity&e = *fpsents[i];
       if(e.state!=CS_ALIVE||!intersect(&e,from,(const vec&)hitpos,dist))continue;
       if(!result||dist<newmaxdist)
        {
        result = &e;
        newmaxdist = dist;
        }
      }
      return result;
 }
 GMCMD(set_ev,"iis",(int*tag,int*attr,const char*val),{returnfpsent(*tag,ent);ent->setev(*attr,val);});
 GMCMD(set_dynent_route,"iii",(int*tag,int*rtag,int*revert),{returnfpsent(*tag,ent);ent->setroute(getrailent(*rtag),*revert);});
};
void fpsEntity::mirror(short axis)
{
 switch(axis)
 {
  case(1):pitch = 360 - pitch;break; //X-axis
  case(2):roll = 360 - roll;break; //Y-axis
  case(3):yaw = 360 - yaw;break; //Z-axis
 }
}
void fpsEntity::think()
{
}
void fpsEntity::move()
{
 if(attached)
  {
   setpos(vec(attached->o).add(atpos));
  }
 else if(route.end())
  {
   if(movable)moveplayer(this,1,true);
  }
 else
  {
   setpos(route.move());
  }
}
void fpsEntity::setmodel(const char*modelname)
{
 if(!modelname)return;
 copystring(model,modelname);
 setbbfrommodel(this,model);
 updatedynentcache(this);
}
void fpsEntity::touched(fpsEntity*ent)
{
}
void fpsEntity::hit(fpsEntity*attacker,float damage,uchar damagetype)
{
 if(maxhealth)health-=damage;
 if(health<0)
  {
   state = CS_DEAD;
   nextthink = lastmillis + 40;
   killed(attacker);
  } 
}
void fpsEntity::falldamage(int ftime)
{
}
void fpsEntity::attacked(fpsEntity*attacker,float damage,uchar damagetype,vec hitpos)
{

}
void fpsEntity::killed(fpsEntity*killer)
{
 state = CS_DEAD;
 nextthink = lastmillis;
}
void fpsEntity::playerused()
{

}
void fpsEntity::reset()
{
 health = maxhealth;
 detach();
 setroute(NULL,0);
}
void fpsEntity::attach(fpsEntity*ent,vec pos)
{
 attached = ent;
 atpos = pos;
}
void fpsEntity::detach()
{
 attached = NULL;
}
void fpsEntity::setpos(vec newpos)
{
 o = newpos;
 resetinterp();
}
void fpsEntity::setangle(int y,int p,int r)
{
 if(y!=-1)yaw = y;
 if(p!=-1)pitch = p;
 if(r!=-1)roll = r;
}
bool fpsEntity::setev(int attr,const char*val)
{
 if(!val)return false;
 #define v atoi(val);
 #define fv atof(val);
 switch(attr)
  {
   case(EV_HEALTH):health = fv;break;
   case(EV_MAXHEALTH):maxhealth = fv;break;
   case(EV_VISIBLE):visible = v;break;
   case(EV_MOVABLE):movable = v;break;
   case(EV_NEXTTHINK):nextthink = lastmillis + v;break;
   case(EV_MODEL):setmodel(val);break;
   case(EV_TEAM):team = v;break;
   case(EV_ANIMSEQUENCE):animsequence = v;break;
   case(EV_FRAMERATE):framerate = v;break;
   case(EV_LASTANIM):lastanim = v;break;
   case(EV_MASS):mass = fv;break;
   case(EV_STATE):state = v;break;
   default:return false;break;
  }
 return true;
}
void fpsEntity::setroute(rail*r,bool revert)
{
 route.set(r,revert);
}
void fpsEntity::stoproute()
{
 route.next = NULL;
}