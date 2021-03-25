#include "cube.h"
int exectask(int fn(void*),char*name,void*args)
{
 SDL_Thread*thread = SDL_CreateThread(fn,name,args);
 if(!thread)return -1;
 SDL_DetachThread(thread);
 return 0;
}
