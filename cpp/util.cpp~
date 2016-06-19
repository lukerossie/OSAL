#include "../include/util.hpp"
#include <cstdlib>
#include <cstdio>
#include <random>
#include "assert.h"
#include <iostream>
#include <math.h>
#include <vector>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

namespace
{
	std::random_device rd;
    std::mt19937 gen(rd());
}

std::string gettag(std::string s, int tagnumber, int index)
{
	if(index>=s.length()-1)
	{
		return "";
	}
	if(s=="")
	{
		return "";
	}

	int end=0;
	for(int i=index; i<s.length(); i++)
	{
		if(s[i]=='>')
		{
			end=i;
			break;
		}

	}

	if(tagnumber==0)
	{
		if(end-index<=0)
		{
			return "";
		}
		return s.substr(index,end-index);
	}
	else return gettag(s,tagnumber-1,end+2);
};
std::string gettags(std::string s, int begin, int end)//0 end signifies to end of string
{
	int count=0;
	int startindex=0;
	for(int i=0; i<s.length(); i++)
	{
		if(s[i]=='>')count++;
		if(count>begin)
		{
			startindex=i+1;
			break;
		}
	}

	count=0;
	int lastindex=0;
	for(int i=0; i<s.length(); i++)
	{
		if(s[i]=='>')count++;
		if((end&&count>end)||i==s.length()-1)
		{
			lastindex=i+1;
			break;
		}
	}
	return s.substr(startindex,lastindex);
};
std::map<std::string,std::string> parseConfig(char const *path)//using gettags
{
	using namespace std;
    string s=loadFile(path);

    int i=0;
    int index=0;
    int lineEnd=0;
    vector<string> lines;

    while(1)
    {
        if(i==s.length())
        {//on last character just push index to last regardless (end of file is removed)
            lines.push_back(s.substr(index,i));
            index=i+1;
            break;
        }
        if(s[i]=='\n')
        {
            lines.push_back(s.substr(index,i));
            index=i+1;
        }
        i++;
    }

    map<string,string> retval;
    for(int i=0; i<lines.size(); i++)
    {
        retval[gettag(lines[i],0,1)]=gettag(lines[i],1,1);
    }
    return retval;
}
std::pair<std::vector<std::string>,std::string> separate(char c,std::string stream,bool exclude)
{
    using namespace std;
    std::pair<vector<string>,string> retval;

    int i=0;
    while(stream[i])
    {
        if(stream[i]==c)
        {
            int secondSubstringBegin=exclude?i+1:i;
            retval.first.push_back(stream.substr(0,i));
            stream=stream.substr(secondSubstringBegin,stream.size());

            i=0;
            continue;
        }
        i++;
    }
    retval.second=stream;
    return retval;
}

std::string loadFile(char const *path)
{
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
        cout<<"\n"<<("Could not open file: "+string(path)).c_str()<<endl;
        return("Error");
    }

    string s;
    int c;
    while ((c = fgetc(fp)) != EOF)
    {
       s+=c;
    }
    fclose(fp);
    return s;
}

/*
TODO: Add collision for things other than rectangles
*/
bool insec( SDL_Rect a, SDL_Rect b )
{
	//if not not intersecting
	return !(b.x > a.x+a.w ||
			b.x+b.w < a.x ||
			b.y > a.y+a.h ||
			b.y+b.h < a.y);
}
int randNum(int min, int max)
{
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

Vec2 angleToVec2(double angle)
{
	auto radians=angle*M_PI/180;
	return Vec2(cos(radians),sin(radians));
}
Vec2::Vec2() : x{0}, y{0}
{}
Vec2::Vec2(double xy) : x{xy}, y{xy}
{}
Vec2::Vec2( double _x, double _y ) : x{_x}, y{_y}
{}
Vec2 Vec2::operator-()
{
	return Vec2(-x,-y);
}
Vec2 Vec2::operator+(Vec2 const &other)
{
	return Vec2(x+other.x, y+other.y);
}
Vec2 Vec2::operator-(Vec2 const &other)
{
	return Vec2(x-other.x, y-other.y);
}
Vec2 Vec2::operator*( Vec2 const &other )
{
	return Vec2(x*other.x, y*other.y);
}
Vec2 Vec2::operator*( double factor )
{
	return Vec2(x*factor, y*factor);
}
Vec2 Vec2::operator/( Vec2 const &other )
{
	return Vec2(x/other.x, y/other.y);
}
Vec2 Vec2::operator/( double factor )
{
	return Vec2(x/factor, y/factor);
}
Vec2 &Vec2::operator+=( Vec2 const &other )
{
	x+=other.x;
	y+=other.y;
	return *this;
}
Vec2 &Vec2::operator-=( Vec2 const &other )
{
	x-=other.x;
	y-=other.y;
	return *this;
}
Vec2 &Vec2::operator*=( Vec2 const &other )
{
	x*=other.x;
	y*=other.y;
	return *this;
}
Vec2 &Vec2::operator*=( double factor )
{
	x*=factor;
	y*=factor;
	return *this;
}
Vec2 &Vec2::operator/=( Vec2 const &other )
{
	x/=other.x;
	y/=other.y;
	return *this;
}
Vec2 &Vec2::operator/=( double factor )
{
	x/=factor;
	y/=factor;
	return *this;
}
double &Vec2::operator[]( int index )
{
	if(index==0)
	{
		return x;
	}
	return y;//y will be returned for anything other than 0, probably not good
}
bool Vec2::operator<( double num )
{
	return x<num&&y<num;
}
bool Vec2::operator>( double num )
{
	return x>num&&y>num;
}
bool Vec2::significant()
{
    if(std::abs(x)<.00001&&std::abs(y)<.00001)
    {
        return false;
    }
    return true;
}
Vec2 Vec2::scale(double factor)
{
	x*=factor;
	y*=factor;
	return *this;
}
double Vec2::toangle()//degrees
{
	return atan2(y, x)*180/M_PI;
}
double Vec2::len()
{
	return std::sqrt(std::pow(x,2)+std::pow(y,2));
}
Vec2 Vec2::unit()
{
    if(!significant())return {0,0};
	auto lenvalue=len();
	return {x/lenvalue,y/lenvalue};
}
Vec2 Vec2::abs()
{
	return {(double)::abs(x),(double)::abs(y)};
}
bool Vec2::sameSign(Vec2 const &other)
{
	bool o_neg_x=other.x<0;
	bool o_neg_y=other.y<0;
	bool neg_x=x<0;
	bool neg_y=y<0;
	return o_neg_x==neg_x && o_neg_y==neg_y;
}
Rect::Rect() : Vec2(0, 0), w{0}, h{0}
{

}
Rect::Rect( double _x, double _y, double _w, double _h ) : Vec2( _x, _y ), w{_w}, h{_h}
{

}

Clock::Clock()
{
	milliSinceConstruction = 0;
	deltaMilli = 0;
	start = getNow();
	oldclock = getNow();
}
long long Clock::getmillis(std::chrono::time_point<std::chrono::high_resolution_clock> tp)
{
	using namespace std::chrono;
	return time_point_cast<milliseconds>(tp).time_since_epoch().count();
}
void Clock::update()
{
	using namespace std::chrono;
	long long now=getNow();
	auto dclock = now - oldclock;
	oldclock = getNow();

	milliSinceConstruction = now - start;

	deltaMilli = dclock;
	//@bug deltamilli can be zero
	if(deltaMilli)FPS=1000/deltaMilli;

	deltaSeconds = deltaMilli/1000.0;
}
void Clock::reset()
{
	start = getNow();
}
long long Clock::getNow()
{
	using namespace std::chrono;
	auto now=high_resolution_clock::now();
	auto mill=time_point_cast<milliseconds>(now);
	return mill.time_since_epoch().count();
}

#ifdef SG2D_TEST
void util_test()
{
	Vec2 v1={1,2};
	Vec2 v2={5,2};
	Vec2 v3=v1+v2;
	assert(v3.x==6);

	Vec2 v4(5,2);
	Vec2 v5(10,5);
	v5*=v4;
	assert(v5.x==50&&v5.y==10&&v4.x==5);

	Vec2 v6(5,5);
	assert(v6[0]==v6.x&&v6[1]==v6.y);

	SDL_Rect a={50,50,25,25};
	SDL_Rect b={76,76,25,25};
	assert(!insec(a,b));
	b.x=75;
	b.y=75;
	assert(insec(a,b));
}
#endif
