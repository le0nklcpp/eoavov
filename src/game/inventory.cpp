#include "eoavov.h"
void RPGItemEnt::playerused()
{
 if(item)
  {
  if(player1->inv.additem(item)<0)return;
  item = NULL;
  }
 killed(this);
}
bool RPGItemEnt::setev(int attr,const char*val)
{
 if(!fpsEntity::setev(attr,val))
  {
   if(attr>=inv_itemid&&attr<=inv_fvar3)item->setproperty(attr,val);
   else return false;
  }
 return true;
}
RPGitem::RPGitem(const char*n,const char*dn,const char*vmdl,const char*mdl,int w,int i,float v)
{
 set(n,dn,vmdl,mdl,w,i,v);
}
void RPGitem::draw(fpsEntity*user,invItem*i)
{
if(user==player1)player1->setvmodel(vmodel,0,0);
}
void RPGitem::set(const char*n,const char*dn,const char*vmdl,const char*mdl,int w,int i,float v)
{
 copystring(name,n);
 copystring(devname,dn);
 if(mdl)copystring(model,mdl);
 else model[0] = '\0';
 if(vmdl)copystring(vmodel,vmdl);
 else vmodel[0]='\0';
 weight = w;
 type = i;
 vol = v;
}
RPGWeapon::RPGWeapon(const char*n,const char*dn,const char*vmdl,const char*mdl,int w,int i,float v,int c,int cid,bool oh,float minar,float maxar,int am)
{
 set(n,dn,vmdl,mdl,w,i,v);
 clip = c;
 clipitemid = cid;
 onehanded = oh;
 minrange = minar;
 maxrange = maxar;
 maxammo = am;
}
/*



*/
invItem*inventory::getitem(int index)
{
 return items.inrange(index)?items[index]:NULL;
}
/*

Returns summary weight of all items

*/
int inventory::getweight()
{
 int w = 0;
 loopv(items)
 {
  w += items[i]->parent->weight;
 }
 return w;
}
/*

Returns summary volume of all items

*/
float inventory::getvolume()
{
 float v = 0.0;
 loopv(items)
 {
  v += items[i]->parent->vol;
 }
 return v;
}
/*

Returns -1 if too much weight, -2 if too much volume, item index on success

*/
int inventory::additem(invItem *i)
{
 RPGitem * item = i->parent;
 if(!item)return -3;
 if(getweight()+item->weight>maxweight)return -1;
 if(getvolume()+item->vol>1.0)return -2;
 items.add(i);
 return items.length()-1;
}
/*

Id - item index in inventory

*/
void invItem::setproperty(int prop,const char*value) // there was mistake with variable names
{
 int n = atof(value);
 switch(prop)
  {
  case(inv_ivar1):ivar1 = n;break;
  case(inv_ivar2):ivar2 = n;break;
  case(inv_ivar3):ivar3 = n;break;
  case(inv_fvar1):fvar1 = n;break;
  case(inv_fvar2):fvar2 = n;break;
  case(inv_fvar3):fvar3 = n;break;
  case(inv_itemid):if(RPG::itemexists(n))parent = RPG::itemlist[n];break;
  }
}
void invItem::getproperty(int prop,const char * var)
{
 switch(prop)
 {
 case(inv_ivar1):setvar(var,ivar1,false);break;
 case(inv_ivar2):setvar(var,ivar2,false);break;
 case(inv_ivar3):setvar(var,ivar3,false);break;
 case(inv_fvar1):setfvar(var,fvar1,false);break;
 case(inv_fvar2):setfvar(var,fvar2,false);break;
 case(inv_fvar3):setfvar(var,fvar3,false);break;
 case(inv_itemid):setsvar(var,parent->devname,false);break;
 }
}
void inventory::removeitem(int id)
{
 if(items.inrange(id))delete items.remove(id);
}
void inventory::clear()
{
 items.deletecontents();
}
namespace RPG
{
 vector<RPGitem*>itemlist;
 int getitembyname(const char * name)
 {
  loopv(itemlist)
   {
    if(!strncmp(itemlist[i]->devname,name,47))return i;
   }
  return -1;
 }
 int registeritem(const char*name,const char*devname,const char*vmodel,const char*model,int weight,int type,float vol)
 {
  itemlist.add(new RPGitem(name,devname,vmodel,model,weight,type,vol));
  return itemlist.length()-1;
 }
 int registerweapon(RPGWeapon*weapon)
 {
  itemlist.add((RPGitem*)weapon);
  return itemlist.length()-1;
 }
 void Player1InvItemOperations(int itemid,bool set,int prop,const char* val)
 {
   if(!player1)return;
   invItem*item = player1->inv.getitem(itemid);
   if(!item)return;
   if(set)item->setproperty(prop,val);
   else item->getproperty(prop,val);
 }
 bool itemexists(int index)
 {
  return itemlist.inrange(index);
 }
 RPGitem* retrpgitem(int index)
 {
  return itemexists(index)?itemlist[index]:NULL;
 }
GMCMD(rpg_register_item,"ssssiif",(char*n,char*dn,char*vmdl,char*mdl,int*w,int*i,float*v),registeritem(n,dn,vmdl,mdl,*w,*i,*v))
GMCMD(rpg_get_item_by_name,"s",(const char*s),{intret(getitembyname(s));});
GMCMD(player_inv_length,"",(),{intret(player1->inv.items.length());});
GMCMD(player_inv_operation,"iiis",(int*index,int*s,int*p,const char*v),{Player1InvItemOperations(*index,(bool)*s,*p,v);});
GMCMD(player_inv_pick,"ii",(int*item,int*clearholster),{if(*item==-1)player1->hands = NULL;else player1->draw(*item);if(*clearholster)player1->holster = NULL;});
GMCMD(player_inv_remove,"i",(int*i),{player1->inv.removeitem(*i);});
GMCMD(player_add_item,"i",(int*i),{if(itemexists(*i))intret(player1->inv.additem(new invItem(itemlist[*i])));});
};
