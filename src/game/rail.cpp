#include "eoavov.h"
namespace game
{
 vector<rail*>rails;
 void clearroutes()
 {
  rails.deletecontents();
  clearfpsroutes();
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
 prevtag = e.attr2;
 nexttag = e.attr3;
 tag = e.attr1;
 revert = e.attr4;
 arrivetime = e.attr5;
 o = e.o;
}
void rail::update()
{
 using namespace game;
 prev = getrailent(prevtag);
 next = getrailent(nexttag);
}
void routeManager::set(rail*c,bool r)
{
 if(c)
  {
  cur = c;
  revert = r;
  timestamp = lastmillis;
  next = revert?c->prev:c->next;
  if(next)
   {
   if(!next->arrivetime)
    {
     next = NULL;
     return;
    }
   dir = vec(next->o).sub(c->o);
   }
  }
 else next = NULL;
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
 if(deltatime>next->arrivetime)deltatime = next->arrivetime;
 vec result = vec(cur->o).add(vec(dir).mul(float(deltatime)/next->arrivetime));
 if(finished(result)&&next)
  {
  set(next,revert);
  }
 return result;
}