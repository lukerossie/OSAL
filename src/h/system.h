#ifndef SYSTEM_H
#define SYSTEM_H

#include "../h/util.h"

int rand_num(s32 min, s32 max);
void sleep(u32 milliseconds);
s64 milli_current_time();
char *malloc_file_cstr(char const *path);
void write_file_cstr(char const *path, char const *cstr);
int rand_num(s32 min, s32 max);
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
#endif