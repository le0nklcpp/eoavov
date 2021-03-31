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
string model,vmodel;
int weight,type;
float vol; // what part of the inventory capacity does this item take.Max 1.0
RPGitem():weight(0),vol(0.0),name("empty"),model(""),vmodel(""){}
RPGitem(char *,char*,char*,char*,int,int,float);
void set(char *,char*,char*,char*,int,int,float);
virtual void draw(fpsEntity*user,invItem*i);
virtual void use(fpsEntity*user,invItem*i,bool release){}
virtual void alt(fpsEntity*user,invItem*i,bool release){}
virtual ~RPGitem(){}
};
struct RPGWeapon:RPGitem
{
int clip,clipitemid;
float minrange,maxrange; // for AI
bool onehanded;
RPGWeapon(char *,char*,char*,char*,int,int,float,int,int,bool,float,float);
virtual void reload(fpsEntity*user,invItem*i){}
virtual int getammo(fpsEntity*user,invItem*i){return 0;}
virtual void setammo(invItem*i,int ammo){}
};
struct invItem{
 RPGitem*parent;
 int ivar1,ivar2,ivar3; // custom variables for some scripts
 float fvar1,fvar2,fvar3;
 invItem(RPGitem*item):parent(item),ivar1(0),ivar2(0),ivar3(0),fvar1(0),fvar2(0),fvar3(0){}
 ~invItem(){parent = NULL;}
 void getproperty(int prop,char*value);
 void setproperty(int prop,char*value);
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
 extern int registeritem(char * name,char*devname,char*vmodel,char*model,int weight,int type,float vol);
 extern int getitembyname(char * name);
 extern int registerweapon(RPGWeapon*weapon);
 extern bool itemexists(int index);
 extern RPGitem *retrpgitem(int index);
};
struct RPGItemEnt:fpsEntity
{
 invItem*item;
 RPGItemEnt(extentity &e):item(NULL)
  {
   RPGitem*t = RPG::retrpgitem(e.attr1);
   if(!t)
    {
     killed(this);
     return;
    }
   yaw = e.attr2;pitch = e.attr3;roll = e.attr4;tag = e.attr5;o = e.o;
   type = E_INVITEM;
   state = CS_ALIVE;
   setmodel(t->model);
   item = new invItem(t);
   movable = true;
  }
 bool setev(int attr,char*val);
 ~RPGItemEnt(){delete item;}
 void playerused();
};
#endif
