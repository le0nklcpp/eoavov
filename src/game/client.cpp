#include "eoavov.h"

playerEnt * player1 = NULL;
namespace game
{
    string clientmap = "";
    const char *gameident() {return "eoavov";}
    void initclient()
    {
    player1 = new playerEnt();
    }
    void gameconnect(bool _remote)
    {
     player1->reset();
    }
    #define clearmodels() {cleanupmodels();clear_models();int dummy=0;mapmodelreset(&dummy);}
    void gamedisconnect(bool cleanup)
    {
     RPG::itemlist.deletecontents();
     proptypes.deletecontents();
     clearacoustic();
     cleargamezones();
     clearfpsents();
     clearmodels();
     clearroutes();
    }
    void changemap(const char *name)
    {
     cubeevent("map_loading");
     if(editmode) toggleedit();
     if(!isconnected()) localconnect();
     if(!load_world(name)) emptymap(0, true, name);
    }
    void forceedit(const char *name)
    {
        changemap(name);
    }
    const char* getclientmap(){return clientmap;}
    ICOMMAND(map, "s", (char *s), changemap(s));
    void physicstrigger(physent *d, bool local, int floorlevel, int waterlevel, int material,int timeinair)
    {
        if     (waterlevel>0) sound("splash1", d==player1 ? NULL : &d->o);
        else if(waterlevel<0) sound("splash2", d==player1 ? NULL : &d->o);
        if     (floorlevel>0) {}
        else if(floorlevel<0) {((fpsEntity*)d)->falldamage(timeinair);}
    }

    void updateworld()        // main game update loop
    {
	if(!isconnected())return;
	checkacoustic();
	physicsframe();
	fpsthink();
	player1->think();
	zonetriggers(player1->o);
    }

    void suicide(physent *d)
    {
	fpsEntity*cast = (fpsEntity*)d;
        cast->killed(cast);
    }
    GMCMD(kill,"",(),{player1->killed(NULL);});
    bool allowedittoggle()
    {
        if(editmode) return true;
        return execidentbool("allowedittoggle", true);
    }
    void renderhudmsg(int w,int h)
    {
	for(int i=0;i<MAXHUDCHANNELS;i++)
	 {
	  if(!hmessages[i].enabled)continue;
	  if(hmessages[i].endtime<lastmillis)
	   {
	   hmessages[i].enabled = false;
	   continue;
	   }
	  if(hmessages[i].starttime>lastmillis)continue;
	  draw_text(hmessages[i].text,w*1800/h*hmessages[i].x,1650*hmessages[i].y,hmessages[i].r,hmessages[i].g,hmessages[i].b);
	 }
    }
    void startmap(const char *name)   // called just after a map load
    {
	player1->reset();
	clearroutes();
	clearfpsents();
	findplayerspawn(player1);
	entities::resetspawns();
	entities::spawnitems(true);
	cubeevent("map_loaded");
	copystring(clientmap, name ? name : "");
	preparemovables();
    }
    void newmap(int size)
    {
    }
    void gameplayhud(int w,int h)
    {
	pushhudscale(h/1800.0f);
	renderhudmsg(w,h);
	renderhudtextures(w,h);
	pophudmatrix();
    }
    const char *getscreenshotinfo()
    {
	return "eoavov_";
    }
    void edittoggled(bool on,bool post) 
    {
	if(!post)
	 {
	 player1->reset();
	 return;
	 }
	if(on)
         {
         clearfpsents();
         clearroutes();
         }
	else entities::spawnitems(true);
    }
    const char *gameconfig() { return "config/game.cfg"; }
    const char *savedconfig() { return "config/saved.cfg"; }
    const char *restoreconfig() { return "config/restore.cfg"; }
    const char *defaultconfig() { return "config/default.cfg"; }
    const char *autoexec() { return "config/autoexec.cfg"; }
    const char *savedservers() { return "config/servers.cfg"; }
    void loadconfigs()
    {
        execfile("config/auth.cfg", false);
    }
    int maxsoundradius(int n)
    {
	return 500;
    }
    GMACMD(attack1,"D",(int*down),player1->attack(*down));
    void connectfail(){}
    void connectattempt(const char *name, const char *password, const ENetAddress &address) {}
    const char *getmapinfo() { return NULL; }
    void preload() {}
    void parsepacketclient(int chan, packetbuf &p) {}
    bool allowmouselook() { return !lock_control; }
    void writeclientinfo(stream *f) {}
    bool ispaused() { return false; }
    int scaletime(int t) { return t*100; }
    void vartrigger(ident *id) {}
    void toserver(char *text) {}
    void setupcamera() {}
    bool allowthirdperson() {return true;}
    bool detachcamera()
    {
        return player1->state==CS_DEAD;
    }
    void writegamedata(vector<char> &extras) {}
    void readgamedata (vector<char> &extras) {}
    void edittrigger(const selinfo &sel, int op, int arg1, int arg2, int arg3, const VSlot *vs){}
};
namespace server
{
    void *newclientinfo() { return NULL; }
    void deleteclientinfo(void *ci) {}
    void serverinit() {}
    int reserveclients() { return 0; }
    int numchannels() { return 0; }
    void clientdisconnect(int n) {}
    int clientconnect(int n, uint ip) { return DISC_NONE; }
    void localdisconnect(int n) {}
    void localconnect(int n) {}
    bool allowbroadcast(int n) { return true; }
    void recordpacket(int chan, void *data, int len) {}
    void parsepacket(int sender, int chan, packetbuf &p) {}
    void sendservmsg(const char *s) {}
    bool sendpackets(bool force) { return false; }
    void serverinforeply(ucharbuf &req, ucharbuf &p) {}
    void serverupdate() {}
    bool servercompatible(char *name, char *sdec, char *map, int ping, const vector<int> &attr, int np) { return true; }
    int serverinfoport() { return 0; }
    int serverport() { return 0; }
    const char *defaultmaster() { return ""; }
    int masterport() { return 0; }
    int laninfoport() { return 0; }
    void processmasterinput(const char *cmd, int cmdlen, const char *args) {}
    bool ispaused() { return false; }
    int scaletime(int t) { return t*100; }
    void masterdisconnected(){}
    void masterconnected(){}
    int protocolversion() {return 0;}
}
