#ifndef __INVENTORY_RPG_H
#define __INVENTORY_RPG_H

#include "eoavov.h"

enum // for scripting - get/set invItem variables
{
 inv_ivar1=0,
 inv_ivar2,
 inv_ivar3,
 inv_fvar1,
 inv_fvar2,
 inv_fvar3,
 inv_itemid
};

#define TYPE_WEAPON 1
#define TYPE_TOOL 2
#define TYPE_ARMOR 3
#define TYPE_CUSTOM 4

struct RPGitem
{
char name[48],devname[48]; // devname used in scripts and for getitembyname
string model,vmodel;
int weight,type;
float vol; // what part of the inventory capacity does this item take.Max 1.0
RPGitem():weight(0),vol(0.0),name("empty"),model(""),vmodel(""){}
RPGitem(char *,char*,char*,char*,int,int,float);
void set(char *,char*,char*,char*,int,int,float);
~RPGitem(){}
};

struct invItem
{
 int itemid;
 int ivar1,ivar2,ivar3; // custom variables for some scripts
 float fvar1,fvar2,fvar3;
 invItem(int iid):itemid(iid),ivar1(0),ivar2(0),ivar3(0),fvar1(0),fvar2(0),fvar3(0){}
 void getproperty(int prop,char*value);
 void setproperty(int prop,char*value);
};

struct RPGWeapon:RPGitem
{
int clip,clipitemid;
float minrange,maxrange; // for AI
bool onehanded;
RPGWeapon(char *,char*,char*,char*,int,int,float,int,int,bool,float,float);
virtual ~RPGWeapon(){}
virtual void doattack(fpsEntity*user,invItem*i){}
virtual void doattack2(fpsEntity*user,invItem*i){}
virtual void reload(fpsEntity*user,invItem*i){}
virtual int getammo(fpsEntity*user,invItem*i){return 0;}
virtual void setammo(invItem*i,int ammo){}
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
 RPGitem *getrpgitem(invItem i);
 extern int registeritem(char * name,char*devname,char*vmodel,char*model,int weight,int type,float vol);
 extern int getitembyname(char * name);
 extern int registerweapon(RPGWeapon*weapon);
 extern bool itemexists(int index);
};
struct RPGItemEnt:fpsEntity
{
 invItem*item;
 RPGItemEnt(extentity &e):item(NULL)
  {
   int itemid = e.attr1;yaw = e.attr2;pitch = e.attr3;roll = e.attr4;tag = e.attr5;o = e.o;
   type = E_INVITEM;
   state = CS_ALIVE;
   if(RPG::itemexists(itemid))
   {
    setmodel(RPG::itemlist[itemid]->model);
    item = new invItem(itemid);
    movable = true;
   }
  }
 ~RPGItemEnt(){delete item;}
 void playerused();
};
#endif
