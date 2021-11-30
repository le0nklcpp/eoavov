#include "eoavov.h"
namespace game
{
 vector<rail*>rails;
 rail*getrailent(int tag)
 {
  if(tag==-1)return NULL;
  loopv(rails)
  {
   if(rails[i]->tag==tag)return rails[i];
  }
  return NULL;
 }
 rail::rail(extentity&e)
 {
  prev = getrailent(e.attr2);
  next = getrailent(e.attr3);
  tag = e.attr1;
  revert = e.attr4;
  arrivetime = e.attr5;
  o = e.o;
 }
 bool routemanager::end()
  {
   return !next;
  }
 bool routemanager::finished()
  {
   return ent->o == next->o;
  }
 void routemanager::move()
  {
   int deltatime = lastmillis - timestamp;
   float step = cur->o.dist(next->o)/next->arrivetime;
   ent->setpos(vec(cur->o).add(vec(dir).mul(step*deltatime)));
   if(finished()&&next)
    {
    cur = next;
    next = revert?next->prev:next->next;
    }
  }
 void clearroutes()
  {
  rails.deletecontents();
  }
}