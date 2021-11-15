#ifndef __INVENTORY_RPG_H
#define __INVENTORY_RPG_H

#include "eoavov.h"

enum // for scripting - get/set invItem variables
{
 inv_itemid=EV_ITEMPTR,
 inv_ivar1,
 inv_ivar2,
 inv_ivar3,
 inv_fvar1,
 inv_fvar2,
 inv_fvar3
};

#define TYPE_WEAPON 1
#define TYPE_TOOL 2
#define TYPE_ARMOR 3
#define TYPE_CUSTOM 4
struct invItem;
struct RPGitem
{
 char name[48],devname[48]; // devname used in scripts and for getitembyname
 string vmodel,model;
 int weight,type;
 float vol; // what part of the inventory capacity does this item take.Max 1.0
 RPGitem(const char *n,const char*dn,const char*vmdl,const char*mdl,int w,int t,float v);
 virtual void draw(fpsEntity*user,invItem*i);
 virtual void use(fpsEntity*user,invItem*i,bool release){}
 virtual void alt(fpsEntity*user,invItem*i,bool release){}
 virtual ~RPGitem(){}
};
struct RPGscriptedItem:RPGitem
{
 void event(const char*name,fpsEntity*user);
 void draw(fpsEntity*user,invItem*i);
 void use(fpsEntity*user,invItem*i,bool release);
 void alt(fpsEntity*user,invItem*i,bool release);
 RPGscriptedItem(const char *n,const char*dn,const char*vmdl,const char*mdl,int w,int t,float v):RPGitem(n,dn,vmdl,mdl,w,t,v){}
 ~RPGscriptedItem(){}
};
struct RPGWeapon:RPGitem
{
 int clip,clipitemid;
 bool onehanded;
 float minrange,maxrange; // for AI
 int maxammo;
 RPGWeapon(const char*n,const char*dn,const char*vmdl,const char*mdl,int w,int i,float v,int c,int cid,bool oh,float minar,float maxar,int am);
 ~RPGWeapon(){}
 virtual void reload(fpsEntity*user,invItem*i){}
 virtual int getammo(fpsEntity*user,invItem*i){return 0;}
 virtual void setammo(invItem*i,int ammo){}
};
struct invItem{
 RPGitem*parent;
 int ivar1,ivar2,ivar3; // custom variables for some scripts
 float fvar1,fvar2,fvar3;
 invItem(RPGitem*item):parent(item),ivar1(0),ivar2(0),ivar3(0),fvar1(0),fvar2(0),fvar3(0){}
 ~invItem(){}
 void getproperty(int prop);
 void setproperty(int prop,const char*value);
};
struct inventory
{
 int maxweight;
 vector<invItem*>items;
 int getweight();
 float getvolume();
 invItem*getitem(int index);
 int additem(invItem *i);
 void removeitem(int id);
 void clear();
 inventory():maxweight(0){}
 ~inventory(){items.deletecontents();}
};
namespace RPG
{
 extern vector <RPGitem*> itemlist;
 extern int registeritem(const char*name,const char*devname,const char*vmodel,const char*model,int weight,int type,float vol);
 extern int getitembyname(const char*name);
 extern int registerweapon(RPGWeapon*weapon);
 extern bool itemexists(int index);
 extern RPGitem *retrpgitem(int index);
};
struct RPGItemEnt:fpsEntity
{
 invItem*item;
 RPGItemEnt(vec pos,int attr1,int attr2,int attr3,int attr4,int attr5):item(NULL)
  {
   RPGitem*t = RPG::retrpgitem(attr1);
   if(!t)
    {
     killed(this);
     return;
    }
   yaw = attr2;pitch = attr3;roll = attr4;tag = attr5;
   type = E_INVITEM;
   state = CS_ALIVE;
   setmodel(t->model);
   setpos(pos);
   item = new invItem(t);
   movable = true;
  }
 bool setev(int attr,const char*val);
 ~RPGItemEnt(){delete item;}
 void playerused();
};
#endif
