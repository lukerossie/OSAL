#include <cmath>
#include "../hpp/util.hpp"
#include "../hpp/system.hpp"
#include <cstdio>
#include <chrono>

bool intersects( rect a, rect b )
{
	//if not not intersecting
	return !(b.x > a.x+a.w ||
			b.x+b.w < a.x ||
			b.y > a.y+a.h ||
			b.y+b.h < a.y);
}

namespace
{
	class clock
	{
		public:

		int FPS;
		long long start;
		long long oldclock;
		long long dclock;
		long long milliSinceConstruction;
		long long deltaMilli;
		float deltaSeconds;

		clock()
		{
			milliSinceConstruction = 0;
			deltaMilli = 0;
			start = getNow();
			oldclock = getNow();
		}
		long long getmillis(std::chrono::time_point<std::chrono::high_resolution_clock> tp)
		{
			using namespace std::chrono;
			return time_point_cast<milliseconds>(tp).time_since_epoch().count();
		}
		void update()
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
		void reset()
		{
			start = getNow();
		}
		long long getNow()
		{
			using namespace std::chrono;
			auto now=high_resolution_clock::now();
			auto mill=time_point_cast<milliseconds>(now);
			return mill.time_since_epoch().count();
		}
	}internal_clock;
}
long long get_time_millis()
{
	return internal_clock.getNow();
}

char *cstr_copy(char const *src, char *dest)
{
	u32 i=0;
	while(src[i])
	{
		dest[i]=src[i];
		++i;
	}
	return dest;
}
u32 cstr_len(char const *cstr)
{
	u32 size=0;
	for(; cstr[size]; ++size);
	return size;
}
char *new_cstr_concat(char const *stra, char const *strb)
{//untested
	u32 lena=cstr_len(stra);
	u32 lenb=cstr_len(strb);
	char *retval=new char[lena+lenb+1];
	for(u32 a=0; a<lena; ++a)
	{
		retval[a]=stra[a];
	}
	for(u32 a=lena; a<lena+lenb; ++a)
	{
		retval[a]=strb[a];
	}
	retval[lena+lenb]=0;
	return retval;
}

vec2::vec2() : x{0}, y{0}
{}
vec2::vec2(f64 angle)
{
	auto radians=angle*PI/180;
	x=cos(radians);
	y=sin(radians);
}
vec2::vec2( f64 _x, f64 _y ) : x{_x}, y{_y}
{}
vec2 vec2::operator-()
{
	return vec2(-x,-y);
}
vec2 vec2::operator+(vec2 const &other)
{
	return vec2(x+other.x, y+other.y);
}
vec2 vec2::operator-(vec2 const &other)
{
	return vec2(x-other.x, y-other.y);
}
vec2 vec2::operator*( vec2 const &other )
{
	return vec2(x*other.x, y*other.y);
}
vec2 vec2::operator*( f64 factor )
{
	return vec2(x*factor, y*factor);
}
vec2 vec2::operator/( vec2 const &other )
{
	return vec2(x/other.x, y/other.y);
}
vec2 vec2::operator/( f64 factor )
{
	return vec2(x/factor, y/factor);
}
vec2 &vec2::operator+=( vec2 const &other )
{
	x+=other.x;
	y+=other.y;
	return *this;
}
vec2 &vec2::operator-=( vec2 const &other )
{
	x-=other.x;
	y-=other.y;
	return *this;
}
vec2 &vec2::operator*=( vec2 const &other )
{
	x*=other.x;
	y*=other.y;
	return *this;
}
vec2 &vec2::operator*=( f64 factor )
{
	x*=factor;
	y*=factor;
	return *this;
}
vec2 &vec2::operator/=( vec2 const &other )
{
	x/=other.x;
	y/=other.y;
	return *this;
}
vec2 &vec2::operator/=( f64 factor )
{
	x/=factor;
	y/=factor;
	return *this;
}
f64 &vec2::operator[]( u32 index )
{
	if(index==0)
	{
		return x;
	}
	return y;//y will be returned for anything other than 0, probably not good
}
bool vec2::significant()
{
    if(std::abs(x)<.00001&&std::abs(y)<.00001)
    {
        return false;
    }
    return true;
}
vec2 vec2::scale(f64 factor)
{
	x*=factor;
	y*=factor;
	return *this;
}
f64 vec2::to_angle()//degrees
{
	return atan2(y, x)*180/PI;
}
f64 vec2::len()
{
	return sqrt(pow(x,2)+pow(y,2));
}
vec2 vec2::unit()
{
    if(!significant())return {0,0};
	auto lenvalue=len();
	return {x/lenvalue,y/lenvalue};
}
vec2 vec2::abs()
{
	return {(f64)std::abs(x),(f64)std::abs(y)};
}