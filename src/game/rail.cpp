#include "eoavov.h"
namespace game
{
 vector<rail*>rails;
 void clearroutes()
 {
  rails.deletecontents();
 }
 rail*getrailent(int tag)
 {
  if(tag==-1)return NULL;
  loopv(rails)
  {
   if(rails[i]->tag==tag)return rails[i];
  }
  return NULL;
 }
}
rail::rail(extentity&e)
{
 using namespace game;
 prev = getrailent(e.attr2);
 next = getrailent(e.attr3);
 tag = e.attr1;
 revert = e.attr4;
 arrivetime = e.attr5;
 o = e.o;
}
void routeManager::set(rail*c,bool r)
{
 cur = c;
 revert = r;
 timestamp = lastmillis;
 next = revert?c->prev:c->next;
 dir = vec(next->o).sub(c->o);
}
bool routeManager::end()
{
 return !next;
}
bool routeManager::finished(const vec o)
{
 return o == next->o;
}
vec routeManager::move()
{
 int deltatime = lastmillis - timestamp;
 float step = cur->o.dist(next->o)/next->arrivetime;
 vec result = vec(cur->o).add(vec(dir).mul(step*deltatime));
 if(finished(result)&&next)
  {
  cur = next;
  next = revert?next->prev:next->next;
  }
 return result;
}