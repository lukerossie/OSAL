#ifndef SYSTEM_HPP
#define SYSTEM_HPP
/*
this covers any other OS features such as time, files, threads, mutex, hardware information, checking what features are supported, etc.
this is the only module that can be included in the implementation of other modules. the module headers should include _nothing_.
*/
void *amem(unsigned int size);//malloc
void fmem(void *mem);//free

char *amem_file(char const *path);//file path to string of contents

void print(char *str);

int rand_num(int min, int max);

long long milli_current_time();

#endif