#include "dict.h"
/*

Dictionary file structure is pretty simple:
first byte is a number of records stored in this file
followed by, of course, records
records structure:
1.Key length - byte
2.Key string - array of bytes
3.Value length - byte
4.Value string - array of bytes

Limits:
1.All lengths are stored as unsigned chars |=> dictionary can not contain more than 255 records
2.Item Key and Value fields are stored as strings(defined in tools.h it is just char[260]).
*/
namespace dict
{
 void read(dictionary*result,const char*path,int &error)
 {
  #define failwith(e) {error = e;delete f;return;}
  #define readchar(c) uchar c;{c = f->getchar();if(f->end())failwith(DICT_UEOF);}
  #define loadstr(s)\
  {\
    readchar(c);\
    if(f->read(s,c)!=c)failwith(DICT_UEOF);\
    s[c] = 0;\
  }
  error = DICT_NOERR;
  stream *f = openrawfile(path,"r");
  if(!f)failwith(DICT_FOPEN);
  readchar(len);
  loopi(len)
   {
    string key,item;
    loadstr(key);
    loadstr(item);
    result->add(key,item);
   }
  delete f;
 }
 void write(dictionary&d,const char*path,int &error)
 {
  #define trywrite(c) {if(!f->putchar((char)c))failwith(DICT_FWRITE);}
  #define writestr(s)\
  {\
   uchar len = strlen(s);\
   trywrite(len); \
   if(!f->putstring(s))failwith(DICT_FWRITE); \
  }
  error = DICT_NOERR;
  int sz = d.size();
  if(sz>=255){error = DICT_TOOMANY;return;}
  stream *f = openrawfile(path,"w");
  if(!f)failwith(DICT_FOPEN);
  trywrite(sz);
  loopi(sz)
   {
    string key,str;
    if(!d.get(i,key,str))failwith(DICT_OOPS);
    writestr(key);
    writestr(str);
   }
  delete f;
 }
};