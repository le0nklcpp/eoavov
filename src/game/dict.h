#ifndef __DICT_H__
#define __DICT_H__
#include "eoavov.h"
namespace dict
{
 class dictionary // Time to shoot myself in the foot
 {
   struct dnode
   {
    string key;
    string item;
    struct dnode*next;
    dnode(const char*k,const char*i):next(NULL){copystring(key,k);copystring(item,i);}
    ~dnode(){}
   };
   private:
   int items,limit;
   dnode*nodes;
   dnode*last;
   public:
   int size()
   {
    return items;
   }
   bool setlimit(int v)
   {
    limit = max(v,0);
    return v==limit;
   }
   int add(const char*key,const char*item)
   {
    if(limit&&items>=limit)return -1;
    dnode* node = new dnode(key,item);
    if(!nodes) // it is the first item ever
      nodes = node;
    else
      last->next = node;
    last = node;
    items++;
    return items-1;
   }
   #define loopnodes() for(dnode*i = nodes;i;i = i->next)
   void clear()
   {
    while(nodes)
     {
      dnode * bin = nodes;
      nodes = nodes->next;
      delete bin;
     }
    last = NULL;
    items = 0;
   }
   bool get(const char*key,string&item)
   {
    loopnodes()
     if(!strcmp(i->key,key))
      {
       copystring(item,i->item);
       return true;
      }
    return false;
   }
   bool get(int index,string&key,string&item)
   {
    int c = 0;
    loopnodes()
     {
      if(c==index)
       {
        copystring(item,i->item);
        copystring(key,i->key);
        return true;
       }
      c++;
     }
    return false;
   }
   #undef loopnodes
   dictionary():items(0),limit(0),last(NULL),nodes(NULL){}
   ~dictionary(){clear();}
 };
 enum
  {
   DICT_NOERR=0,
   DICT_TOOMANY, // Too many items in dictionary
   DICT_UEOF, // Unexpected end of file
   DICT_FOPEN, // Failed to open
   DICT_FWRITE, // Failed to write to file
   DICT_OOPS // Surprice behaviour
  };
 void write(dictionary&d,const char*path,int& error); // if something went wrong it will write error code into this variable
 void read(dictionary*result,const char*path,int& error);
};
#endif