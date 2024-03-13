#ifndef __eoavov_TASK__
#define __eoavov_TASK__
extern int exectask(int fn(void*),const char*name,void*args);
namespace game{
 namespace threads{
  extern void lock(SDL_mutex*mutex);
  extern void unlock(SDL_mutex*mutex);
 };
};
#endif