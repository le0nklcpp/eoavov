#ifndef __RPGOBJECT_H__
#define __RPGOBJECT_H__
#include "eoavov.h"

struct healthSystem
{
 float * bodyparts; // array
 float pain; // 0..1
 ushort numparts;
 ushort moral;
 ushort maxmoral;
 healthSystem(ushort n,ushort m):pain(0.){bodyparts = new float[n];numparts = n;moral = maxmoral = m;reset();}
 ~healthSystem(){delete[](bodyparts);}
 float getpartdamage(int index)
 {
  if(index<0||index>=numparts)return 0.;
  return bodyparts[index];
 }
 void setpartdamage(int index,float damage)
 {
  if(index>=0&&index<numparts)bodyparts[index] = damage;
 }
 void reset()
 {
  for(int i=0;i<numparts;i++)bodyparts[i]=0.;
  moral = maxmoral;
 }
};

struct RPGObject
{
 inventory inv;
 healthSystem* organism; // Engrish
 invItem*hands,*holster;
 RPGObject():hands(NULL){organism = NULL;} // head, legs,hands, body
 virtual ~RPGObject() {delete organism;inv.clear();}
 void reset(){inv.clear();if(organism)organism->reset();holster = hands = NULL;}
 virtual invItem* draw(int index){invItem *item = inv.getitem(index);if(item){holster = hands;hands = item;}return item;}
 virtual void removeitem(int index)
 {
  invItem*item = inv.getitem(index);
  if(item==hands)
  {
   hands=NULL;
  }
  if(item==holster)holster=NULL;
  inv.removeitem(index);
 }
};
#endif
