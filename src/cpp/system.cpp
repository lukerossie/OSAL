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
#include <cstdio>
#include <random>

#include "../hpp/system.hpp"

#include "../hpp/graphics.hpp"
#include "../hpp/util.hpp"
#include <string>
using namespace std;

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

void print(const string &str)
{
	printf("%s",str.c_str());
}
char *new_file(char const *path)
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
	string finalpathadj=adjpathstr+("/")+(path);
	path=finalpathadj.c_str();
#endif

    FILE* fp=fopen(path, "rb");
    if(!fp)
    {
        print("Could not open file.");
        return NULL;
    }
	
	fseek(fp, 0L, SEEK_END);
	u32 sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char *buff=new char[sz];
	fread(buff, 1, sz, fp);
	
    return buff;
}
void write_file(char const *path, char const *data, u32 bytes)
{
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
	string finalpathadj=adjpathstr+("/")+(path);
	path=finalpathadj.c_str();
#endif

    FILE* fp=fopen(path, "wb");
    if(!fp)
    {
        print((string("Could not open file: ")+string(path)).c_str());
		return;
    }

	fwrite(data, bytes, 1, fp);
    fclose(fp);
}
void write_file_cstr(char const *path, char const *data)
{
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
	string finalpathadj=adjpathstr+("/")+(path);
	path=finalpathadj.c_str();
#endif

    FILE* fp=fopen(path, "w");
    if(!fp)
    {
        print((string("Could not open file: ")+string(path)).c_str());
		return;
    }

    fprintf(fp,"%s",data);
    fclose(fp);
}
long long milli_current_time()
{
	using namespace std::chrono;
	auto now=high_resolution_clock::now();
	auto mill=time_point_cast<milliseconds>(now);
	return mill.time_since_epoch().count();
}
