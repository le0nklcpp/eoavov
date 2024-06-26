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
RPGitem::RPGitem(const char*n,const char*dn,const char*vmdl,const char*mdl,int w,int t,float v)
{
 copystring(name,n);
 copystring(devname,dn);
 copystring(model,mdl);
 copystring(vmodel,vmdl);
 weight = w;
 type = t;
 vol = v;
}
void RPGitem::draw(fpsEntity*user,invItem*i)
{
 if(user==player1)player1->setvmodel(vmodel,0,0);
}
RPGWeapon::RPGWeapon(const char*n,const char*dn,const char*vmdl,const char*mdl,int w,int i,float v,int c,int cid,bool oh,float minar,float maxar,int am):RPGitem(n,dn,vmdl,mdl,w,i,v)
{
 clip = c;
 clipitemid = cid;
 onehanded = oh;
 minrange = minar;
 maxrange = maxar;
 maxammo = am;
}
ETAGVAR(item_owner_tag);
VAR(item_release,0,0,1);
void RPGscriptedItem::event(const char*name,fpsEntity*user)
{
 string funcname;
 formatstring(funcname,"%s_%s",devname,name);
 item_owner_tag = user->tag;
 game::cubeevent(funcname);
}
void RPGscriptedItem::draw(fpsEntity*user,invItem*i)
{
 event("draw",user);
}
void RPGscriptedItem::use(fpsEntity*user,invItem*i,bool release)
{
 item_release = release;
 event("use",user);
}
void RPGscriptedItem::alt(fpsEntity*user,invItem*i,bool release)
{
 item_release = release;
 event("alt",user);
}
/*



*/
invItem*inventory::getitem(int index)
{
 return findone(items,index);
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
 #define sf atof(value);
 #define si atoi(value);
 switch(prop)
  {
  case(inv_ivar1):ivar1 = si;break;
  case(inv_ivar2):ivar2 = si;break;
  case(inv_ivar3):ivar3 = si;break;
  case(inv_fvar1):fvar1 = sf;break;
  case(inv_fvar2):fvar2 = sf;break;
  case(inv_fvar3):fvar3 = sf;break;
  case(inv_itemid):int n = si;if(RPG::itemexists(n))parent = RPG::itemlist[n];break;
  }
}
void invItem::getproperty(int prop)
{
 switch(prop)
 {
 case(inv_ivar1):intret(ivar1);break;
 case(inv_ivar2):intret(ivar2);break;
 case(inv_ivar3):intret(ivar3);break;
 case(inv_fvar1):floatret(fvar1);break;
 case(inv_fvar2):floatret(fvar2);break;
 case(inv_fvar3):floatret(fvar3);break;
 case(inv_itemid):result(parent->devname);break;
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
 void Player1InvItemOperations(playerEnt*ent,int itemid,bool set,int prop,const char* val)
 {
   if(!ent)return;
   invItem*item = ent->inv.getitem(itemid);
   if(!item)return;
   if(set)item->setproperty(prop,val);
   else item->getproperty(prop);
 }
 bool itemexists(int index)
 {
  return itemlist.inrange(index);
 }
 RPGitem* retrpgitem(int index)
 {
  return findone(itemlist,index);
 }
#define retplent playerEnt*ent = *tag==PLAYER_ENT_TAG?player1:(playerEnt*)game::getfpsent(*tag,E_CREATURE);if(!ent)return;
GMCMD(rpg_register_item,"ssssiif",(char*n,char*dn,char*vmdl,char*mdl,int*w,int*i,float*v),registeritem(n,dn,vmdl,mdl,*w,*i,*v))
GMCMD(rpg_get_item_by_name,"s",(const char*s),{intret(getitembyname(s));});
GMCMD(player_inv_length,"i",(int*tag),{retplent;intret(ent->inv.items.length());});
GMCMD(player_inv_operation,"iiiis",(int*tag,int*index,int*s,int*p,const char*v),{retplent;Player1InvItemOperations(ent,*index,(bool)*s,*p,v);});
GMCMD(player_inv_pick,"ii",(int*tag,int*item),{retplent;ent->draw(*item);});
GMCMD(player_inv_remove,"ii",(int*tag,int*i),{retplent;ent->removeitem(*i);});
GMCMD(player_inv_drop,"ii",(int*tag,int*i),{retplent;ent->dropitem(*i);});
GMCMD(player_add_item,"ii",(int*tag,int*i),{retplent;if(itemexists(*i))intret(ent->inv.additem(new invItem(itemlist[*i])));});
};
