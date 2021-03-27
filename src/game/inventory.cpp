#include "eoavov.h"
void RPGItemEnt::playerused()
{
 if(item)
  {
  if(player1->inv.additem(item)<0)return;
  item = NULL;
  }
 nextthink = lastmillis;
 state = CS_DEAD; // entity will be removed in next cycle
}
RPGitem::RPGitem(char* n,char * dn,char*vmdl,char*mdl,int w,int i,float v)
{
 set(n,dn,vmdl,mdl,w,i,v);
}
void RPGitem::draw(fpsEntity*user,invItem*i)
{
if(user==player1)player1->setvmodel(vmodel,0,0);
}
void RPGitem::set(char* n,char * dn,char*vmdl,char*mdl,int w,int i,float v)
{
 copystring(name,n);
 copystring(devname,dn);
 if(mdl)copystring(model,mdl);
 else mdl[0] = '\0';
 if(vmdl)copystring(vmodel,vmdl);
 else vmodel[0]='\0';
 weight = w;
 type = i;
 vol = v;
}
RPGWeapon::RPGWeapon(char* n,char * dn,char*vmdl,char*mdl,int w,int i,float v,int c,int cid,bool oh,float minar,float maxar)
{
 set(n,dn,vmdl,mdl,w,i,v);
 clip = c;
 clipitemid = cid;
 onehanded = oh;
 minrange = minar;
 maxrange = maxar;
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
  RPGitem* item = RPG::getrpgitem(*items[i]);
  if(item)w += item->weight;
  else {delete items.remove(i--);}
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
  RPGitem* item = RPG::getrpgitem(*items[i]);
  if(item)v += item->vol;
  else {delete items.remove(i--);}
 }
 return v;
}
/*

Returns -1 if too much weight, -2 if too much volume, item index on success

*/
int inventory::additem(invItem *i)
{
 RPGitem * item = RPG::getrpgitem(*i);
 if(!item)return -3;
 if(getweight()+item->weight>maxweight)return -1;
 if(getvolume()+item->vol>1.0)return -2;
 items.add(i);
 return items.length()-1;
}
/*

Id - item index in inventory

*/
void invItem::setproperty(int prop,char*value) // there was mistake with variable names
{
 switch(prop)
  {
  case(inv_ivar1):ivar1 = atoi(value);break;
  case(inv_ivar2):ivar2 = atoi(value);break;
  case(inv_ivar3):ivar3 = atoi(value);break;
  case(inv_fvar1):fvar1 = atof(value);break;
  case(inv_fvar2):fvar2 = atof(value);break;
  case(inv_fvar3):fvar3 = atof(value);break;
  case(inv_itemid):itemid = atoi(value);break;

  }
}
void invItem::getproperty(int prop,char * var)
{
 switch(prop)
 {
 case(inv_ivar1):setvar(var,ivar1);break;
 case(inv_ivar2):setvar(var,ivar2);break;
 case(inv_ivar3):setvar(var,ivar3);break;
 case(inv_fvar1):setfvar(var,fvar1);break;
 case(inv_fvar2):setfvar(var,fvar2);break;
 case(inv_fvar3):setfvar(var,fvar3);break;
 case(inv_itemid):setvar(var,itemid);break;
 }
 touchvar(var);
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
 RPGitem *getrpgitem(invItem i)
 {
  if(i.itemid<0||i.itemid>=itemlist.length())return NULL;
  return itemlist[i.itemid];
 }
 int getitembyname(char * name)
 {
  loopv(itemlist)
   {
    if(!strcmp(name,itemlist[i]->devname))return i;
   }
  return -1;
 }
 int registeritem(char * name,char*devname,char*vmodel,char*model,int weight,int type,float vol)
 {
  itemlist.add(new RPGitem(name,devname,vmodel,model,weight,type,vol));
  return itemlist.length()-1;
 }
 int registerweapon(RPGWeapon*weapon)
 {
  itemlist.add((RPGitem*)weapon);
  return itemlist.length()-1;
 }
 void Player1InvItemOperations(int itemid,bool set,int prop,char* val)
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
ICOMMAND(rpg_register_item,"ssssiif",(char*n,char*dn,char*vmdl,char*mdl,int*w,int*i,float*v),registeritem(n,dn,vmdl,mdl,*w,*i,*v))
ICOMMAND(rpg_get_item_by_name,"s",(char*s),{intret(getitembyname(s));});
ICOMMAND(player_inv_length,"",(),{intret(player1?player1->inv.items.length():0);});
ICOMMAND(player_inv_operation,"iiis",(int*index,int*s,int*p,char*v),{Player1InvItemOperations(*index,(bool)*s,*p,v);});
ICOMMAND(player_inv_remove,"i",(int*i),{if(player1)player1->inv.removeitem(*i);});
ICOMMAND(player_add_item,"i",(int*i),{if(player1&&RPG::itemexists(*i))intret(player1->inv.additem(new invItem(*i)));})
};
