#include "eoavov.h"
#define ACOUSTIC_FIRST_DELAY 300
#define ACOUSTIC_SECOND_DELAY 700
#define ACOUSTIC_FIRST_VOLUME 45
#define ACOUSTIC_SECOND_VOLUME 15
struct deferredsound
{
string file;
int vol;
int timestamp;
deferredsound(const char*f,int v,int t):vol(v),timestamp(t){copystring(file,f);}
~deferredsound(){}
bool emit()
 {
  if(lastmillis >= timestamp)
   {
   playsoundname(file,NULL,vol);
   return true;
   }
  return false;
 }
};
vector<deferredsound*>sounds;
void playacoustic(const char*file,const vec *pos)
{
 playsoundname(file,pos);
 sounds.add(new deferredsound(file,ACOUSTIC_FIRST_VOLUME,lastmillis+ACOUSTIC_FIRST_DELAY));
 sounds.add(new deferredsound(file,ACOUSTIC_SECOND_VOLUME,lastmillis+ACOUSTIC_SECOND_DELAY));
}
void checkacoustic()
{
 loopv(sounds)
  {
   if(sounds[i]->emit())delete sounds.remove(i--);
  }
}
void clearacoustic()
{
 sounds.deletecontents();
}
GMCMD(playacoustic,"s",(const char*file),playacoustic(file,NULL));
