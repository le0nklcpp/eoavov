#ifndef __GAME_RAIL_H_
#define __GAME_RAIL_H_

#include "eoavov.h"
/* This is entity for creating paths
   tag - entity tag
   fromtag - previous rail entity tag
   totag - next rail entity tag 
   revert - if set to 1, then the entity moving along this route will be sent back after reaching this entity, also it marks entity route as reverted
   arrivaltime - time(in millis) moving from this rail to next rail takes
   /newent rail tag fromtag totag revert arrivaltime

   It works like that:
   railent1 --------- railent2 ---------- railent3
   entity1^
   -after setting entity1 route entity1 will move from railent1 to railent2, from railent2 to railent3
   -moving between the rails entity1 will use the shortest path
*/
namespace game{
 #define validrail(a) (a->arrivetime>0&&(a->revert?a->prev:a->next))
 struct rail;
 extern vector<rail*>rails;
 struct rail{
 vec o;
 rail*prev,*next;
 int arrivetime,tag;
 bool revert;
 rail(extentity&e);
 ~rail(){}
 };
 struct routemanager{
  bool revert;
  int timestamp;
  vec dir;
  fpsEntity*ent;
  rail*cur,*next;
  routemanager(fpsEntity*e,rail*c,bool r=false):revert(r),ent(e),cur(c),timestamp(lastmillis)
   {
    next = revert?c->prev:c->next;
    e->movable = false;
    e->setpos(c->o);
    dir = vec(next->o).sub(c->o);
   }
  ~routemanager(){}
  bool finished();
  void move();
 };
 void cleanroutes();
};
#endif