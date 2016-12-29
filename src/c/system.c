#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "../h/system.h"
#include "../h/graphics.h"
#include "../h/util.h"


int rand_num(s32 min, s32 max);

void sleep(u32 milliseconds)
{
	SDL_Delay(milliseconds);
}

s64 milli_current_time();
/*
struct raw_thread;
void wait_thread(raw_thread *t,int *status_out);
raw_thread *ctor_raw_thread(int(*func)(void*),char const *name,void *data);

struct mutex;
mutex *ctor_mutex();
void dtor_mutex(mutex* m);
void lock_mutex(mutex *m);
void unlock_mutex(mutex *m);
*/