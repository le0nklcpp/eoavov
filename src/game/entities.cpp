#include "eoavov.h"
namespace entities
{
    using namespace game;
    
    int extraentinfosize() { return 0; }       // size in bytes of what the 2 methods below read/write... so it can be skipped by other games

    void writeent(entity &e, char *buf)   // write any additional data to disk (except for ET_ ents)
    {
    }

    void readent(entity &e, char *buf, int ver)     // read from disk, and init
    {

    }
#ifndef STANDALONE
    vector<extentity *> ents;

    vector<extentity *> &getents() { return ents; }

    bool mayattach(extentity &e) { return false; }
    bool attachent(extentity &e, extentity &a) { return false; }

    int find_ent_by_tag(uchar type,short tag)
    {
     static int etag;
     loopv(ents)
      {
	etag = -1;
	extentity &e = *ents[i];
	if(e.type!=type)continue;
	switch(type)
	 {
	 case(ENT_MAPMODEL):etag = e.attr5;break;
	 }
	if(etag==tag)return i;
      }
     return -1;
    }
    const char *entmodel(const entity &e)
    {
        return NULL;
    }
    #ifdef __CUBE_LEGACY__
    void resetspawns() { loopv(ents) ents[i]->spawned = false; }
    #else
    void resetspawns() { loopv(ents) { extentity *e = ents[i]; e->clearspawned(); e->clearnopickup(); } }
    #endif
    void spawnitems(bool force)
    {
        #define isFpsEnt(type) (type==ENT_CREATURE||type==ENT_ITEM||type==ENT_PROP)
	loopv(ents)
	 {
	  extentity &e = *ents[i];
          if(isFpsEnt(e.type))createfpsent(e.type,e.o,e.attr1,e.attr2,e.attr3,e.attr4,e.attr5);
	 }
    }
    #ifdef __CUBE_LEGACY__
    void setspawn(int i, bool on) { if(ents.inrange(i)) ents[i]->spawned = on; }
    #else
    void setspawn(int i, bool on) { if(ents.inrange(i)) { extentity *e = ents[i]; e->setspawned(on); e->clearnopickup(); } }
    #endif
    extentity *newentity() { return new extentity(); }
    void deleteentity(extentity *e) { delete (extentity*)e; }

    void clearents()
    {
        while(ents.length()) deleteentity(ents.pop());
    }
    void set_ent_attr(int index,int attr1,int attr2,int attr3,int attr4,int attr5)
    {
      if(!ents.inrange(index))return;
      extentity &e = *ents[index];
      e.attr1 = attr1;
      e.attr2 = attr2;
      e.attr3 = attr3;
      e.attr4 = attr4;
      e.attr5 = attr5;
    }
    GMCMD(set_ent_args,"iiiiii",(int*index,int*attr1,int*attr2,int*attr3,int*attr4,int*attr5),set_ent_attr(*index,*attr1,*attr2,*attr3,*attr4,*attr5));

    void animatemapmodel(const extentity &e, int &anim, int &basetime)
    {

    }
    bool printent(extentity &e, char *buf, int len)
    {
        return false;
    }
    void fixentity(extentity &e)
    {
     /*   switch(e.type)
        {
	    case ENT_PROP:
	    case ENT_VEHICLE:
                e.attr5 = e.attr4;
                e.attr4 = e.attr3;
            case ENT_TELEDEST:
                e.attr3 = e.attr2;
            case ENT_CREATURE:
                e.attr2 = e.attr1;
            case ENT_SPAWN:
                e.attr1 = (int)player1->yaw;
                break;
        }*/
    }

    void entradius(extentity &e, bool color)
    {
        switch(e.type)
        {
            case ENT_TELEPORT:
                loopv(ents) if(ents[i]->type == ENT_TELEDEST && e.attr1==ents[i]->attr2)
                {
                    renderentarrow(e, vec(ents[i]->o).sub(e.o).normalize(), e.o.dist(ents[i]->o));
                    break;
                }
                break;
            case ENT_PLAYERSTART:
            case ENT_TELEDEST:
            case ENT_ITEM:
            case ENT_CREATURE:
            case ENT_VEHICLE:
            {
                vec dir;
                vecfromyawpitch(e.attr1, 0, 1, 0, dir);
                renderentarrow(e, dir, 4);
                break;
            }
            case ENT_PROP:
                vec dir;
		vecfromyawpitch(e.attr2,0,1,0,dir);
		renderentarrow(e, dir, 4);
                break;
        }
    }

    bool printent(extentity &e, char *buf)
    {
        return false;
    }

    const char *entnameinfo(entity &e) { return ""; }
    const char *entname(int i)
    {
        static const char *entnames[] =
        {
             "none?", "light", "mapmodel", "playerstart", "envmap", "particles", "sound", "spotlight", "decal","prop",/*"trigger",*/"vehicle","creature","rpgitem","teleport","teledest","rail","custom"
        };
        return i>=0 && size_t(i)<sizeof(entnames)/sizeof(entnames[0]) ? entnames[i] : "";
    }

    void editent(int i, bool local)
    {
        //mpeditent(i,e.o,e.type,e.attr1,e.attr2,e.attr3,e.attr4,e.attr5,true);
    }

    float dropheight(entity &e)
    {
        return 4.0f;
    }
#endif
}

