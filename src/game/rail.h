#ifndef __GAME_RAIL_H_
#define __GAME_RAIL_H_

#include "eoavov.h"
/* This is entity for creating paths
   tag - entity tag
   fromtag - previous rail entity tag
   totag - next rail entity tag
   revert - if set to 1, then the entity moving along this route will be sent back after reaching this entity, also it marks entity route as reverted
   arrivaltime - time(in millis) moving from the previous rail to this rail takes
   /newent rail tag fromtag totag revert arrivaltime

   It works like that:
   railent1 --------- railent2 ---------- railent3
   entity1^
   -after setting entity1 route entity1 will move from railent1 to railent2, from railent2 to railent3
   -moving between the rails entity1 will use the shortest path
*/
struct rail;
namespace game{
 #define validrail(a) (a->arrivetime>0&&(a->revert?a->prev:a->next))
 extern vector<rail*>rails;
 void clearroutes();
 rail*getrailent(int tag);
};
struct rail{
 vec o;
 rail*prev,*next;
 int prevtag,nexttag;
 int arrivetime,tag;
 bool revert;
 void update();
 rail(extentity&e);
 ~rail(){}
};
struct routeManager{
 bool revert;
 int timestamp;
 vec dir;
 rail*cur,*next;
 routeManager():cur(NULL),next(NULL){}
 void set(rail*c,bool r=false);
 bool end();
 bool finished(const vec o);
 vec move();
};
#endif