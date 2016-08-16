#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "../hpp/util.hpp"
/*
this covers any other OS features such as time, files, threads, mutex, hardware information, checking what features are supported, etc.
this is the only module that can be included in the implementation of other modules. the module headers should include _nothing_.
*/
void *amem(u32 size);//malloc
void *rmem(void *block, u32 size);//realloc
void fmem(void *mem);//free

char *amem_file(char const *path);//file path to string of contents
void write_file(char const *path, char const *data, u32 bytes);
void write_file_cstr(char const *path, char const *data);

void print(char const *str);

int rand_num(s32 min, s32 max);
void delay(u32 milliseconds);
long long milli_current_time();

#endif