#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <chrono>
#include <cstdlib>
#include <random>

#include "../hpp/system.hpp"
#include "../hpp/util.hpp"

namespace
{
	std::random_device rd;
    std::mt19937 gen(rd());
}

int rand_num(s32 min, s32 max)
{
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void delay(u32 milliseconds)
{
	SDL_Delay(milliseconds);
}

void *amem(u32 size)
{
	return malloc(size);
}
void *rmem(void *block, u32 size)
{
	return realloc(block, size);
}
void fmem(void *mem)
{
	free(mem);
}

void print(char const *str)
{
	printf("%s",str);
}
char *amem_file(char const *path)
{//at least 2x slower than it should be

#ifdef  __APPLE__
	char adjpath[1024];
	uint32_t size = sizeof(adjpath);
	_NSGetExecutablePath(adjpath, &size);
	string adjpathstr=adjpath;
	for(int i=adjpathstr.size(); i>0; i--)
	{
		if(adjpathstr[i]=='/' || adjpathstr[i]=='\\')
		{
			adjpathstr=adjpathstr.slice(0,i-1);
			break;
		}
	}
	string finalpathadj=adjpathstr.concat("/").concat(path);
	path=finalpathadj.cstr;
#endif

    FILE* fp=fopen(path, "r");
    if(!fp)
    {
        print("Could not open file.");
        return NULL;
    }

    string s;
    int c;
    while ((c = fgetc(fp)) != EOF)
    {
       s=s.concat(c);
    }
    fclose(fp);
	char *retval=(char *)amem(s.size()+1);
	cstr_copy(s.cstr,retval);
    return retval;
}
long long milli_current_time()
{
	using namespace std::chrono;
	auto now=high_resolution_clock::now();
	auto mill=time_point_cast<milliseconds>(now);
	return mill.time_since_epoch().count();
}
