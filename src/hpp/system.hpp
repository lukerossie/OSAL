#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "../hpp/util.hpp"
#include <string>

char *new_file(char const *path);//file path to string of contents
void write_file(char const *path, char const *data, u32 bytes);
void write_file_cstr(char const *path, char const *data);

void print(const std::string &str);

int rand_num(s32 min, s32 max);
void delay(u32 milliseconds);
long long milli_current_time();

struct raw_thread;
void wait_thread(raw_thread *t,int *status_out);
raw_thread *ctor_raw_thread(int(*func)(void*),char const *name,void *data);

struct mutex;//
mutex *ctor_mutex();
void dtor_mutex(mutex* m);
void lock_mutex(mutex *m);
void unlock_mutex(mutex *m);

#endif