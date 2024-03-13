#include "cube.h"
int exectask(int fn(void*),const char*name,void*args)
{
 SDL_Thread*thread = SDL_CreateThread(fn,name,args);
 if(!thread)return -1;
 SDL_DetachThread(thread);
 return 0;
}
namespace game::threads{
 static inline void lock(SDL_mutex*mutex)
 {
  SDL_LockMutex(mutex);
 }
 static inline void unlock(SDL_mutex*mutex)
 {
  SDL_UnlockMutex(mutex);
 }
};