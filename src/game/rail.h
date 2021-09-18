#include <eoavov>
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
 struct rail{
 rail*prev,*next;
 int arrivetime,tag;
 bool revert;
 rail(extentity&e)
  {
   prev = getfpsent(e.attr2,E_RAIL);
   next = getfpsent(e.attr3,E_RAIL);
   next = getfpsent(); 
   tag = e.attr1;
   revert = e.attr4;
   arrivetime = e.attr5;
  }
 };
 struct rroute{
  bool revert;
  int timestamp;
  vec dir;
  fpsEntity*ent;
  rail*cur,*next;
  rroute(fpsEntity*e,rail*c,bool r=false):revert(r),ent(e),cur(c),timestamp(lastmillis)
   {
    next = revert?c->prev:c->next;
    e->setpos(c->o);
    dir = vec(next->o).sub(c->o);
   }
  void move()
   {
    int deltatime = lastmillis - timestamp;
    float step = cur->o.dist(next->o)/next->arrivetime;
    ent->setpos(vec(cur->o).add(vec(dir).mul(step*deltatime)));
   }
 };
 void cleanroutes();
 void setroute(fpsEntity*e,rail*route);
};