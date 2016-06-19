#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include <chrono>
#include <cstdlib>
#include <random>

#include "../hpp/system.hpp"
#include "../../IFUM/hpp/IFUM.hpp"

namespace
{
	std::random_device rd;
    std::mt19937 gen(rd());
}

int rand_num(int min, int max)
{
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void *amem(unsigned int size)
{
	return malloc(size);
}
void fmem(void *mem)
{
	free(mem);
}

void print(char *str)
{
	printf(str);
}
char *amem_file(char const *path)
{//at least 2x slower than it should be
	using namespace std;
#ifdef  __APPLE__
	char adjpath[1024];
	uint32_t size = sizeof(adjpath);
	_NSGetExecutablePath(adjpath, &size);
	string adjpathstr=adjpath;
	for(int i=adjpathstr.length(); i>0; i--)
	{
		if(adjpathstr[i]=='/' || adjpathstr[i]=='\\')
		{
			adjpathstr=adjpathstr.substr(0,i);
			break;
		}
	}
	string finalpathadj=adjpathstr+"/"+string(path);
	path=finalpathadj.c_str();
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
       s+=c;
    }
    fclose(fp);
	char *retval=(char *)amem(s.size()+1);
	copy_cstr(s.c_str(),retval);
    return retval;
}
long long milli_current_time()
{
	using namespace std::chrono;
	auto now=high_resolution_clock::now();
	auto mill=time_point_cast<milliseconds>(now);
	return mill.time_since_epoch().count();
}
