#include <cmath>
#include "../hpp/util.hpp"
#include "../hpp/system.hpp"
#include <cstdio>

bool intersects( rect a, rect b )
{
	//if not not intersecting
	return !(b.x > a.x+a.w ||
			b.x+b.w < a.x ||
			b.y > a.y+a.h ||
			b.y+b.h < a.y);
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
char *amem_cstr_concat(char const *stra, char const *strb)
{//untested
	u32 lena=cstr_len(stra);
	u32 lenb=cstr_len(strb);
	char *retval=(char*)amem(lena+lenb+1);
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
u32 cstr_len(char const *cstr)
{
	u32 size=0;
	for(; cstr[size]; ++size);
	return size;
}

string::string()
{
	cstr=(char*)amem(1);
	cstr[0]=0;
}
string::string(char const *str):string()
{
	fmem(cstr);
	u32 len=cstr_len(str)+1;
	cstr=(char*)amem(len);
	for(u32 a=0; a<len; ++a)
	{
		cstr[a]=str[a];
	}
	cstr[len-1]=0;
}
string::string(char c):string()
{
	fmem(cstr);
	cstr=(char*)amem(2);
	cstr[0]=c;
	cstr[1]=0;
}
/*
t2 = t1; calls assignment operator - t2.operator=(t1)
Test t3 = t1; calls copy constructor - Test t3(t1)
*/
string::string(string const &other):string()
{
	fmem(cstr);
	u32 size=other.size();
	cstr=(char*)amem(size+1);
	for(u32 a=0; a<size; ++a)
	{
		cstr[a]=other.cstr[a];
	}
	cstr[size]=0;
}
string &string::operator=(string const &other)
{
	fmem(cstr);
	u32 size=other.size();
	cstr=(char*)amem(size+1);
	for(u32 a=0; a<size; ++a)
	{
		cstr[a]=other.cstr[a];
	}
	cstr[size]=0;
	return *this;
}
string::~string()
{
	fmem(cstr);
}
string string::slice(u32 start, u32 end)
{
	string s;
	for(u32 i=start; i<=end; i++)
	{
		s=s.concat(cstr[i]);
	}
	return s;
}
string string::concat(string const &str)
{
	u32 lena=cstr_len(cstr);
	u32 lenb=cstr_len(str.cstr);
	char *retval=(char*)amem(lena+lenb+1);
	for(u32 a=0; a<lena; ++a)
	{
		retval[a]=cstr[a];
	}
	for(u32 a=lena; a<lena+lenb; ++a)
	{
		retval[a]=str.cstr[a-lena];
	}
	retval[lena+lenb]=0;
	string s=retval;

	fmem(retval);
	return s;
}
bool string::equals(string const &str) const
{
	for(u32 a=0; a<cstr_len(str.cstr); ++a)
	{
		if(cstr[a]!=str.cstr[a])
		{
			return false;
		}
	}
	return true;
}
u32 string::size() const
{
	return cstr_len(cstr);
}
char &string::operator[](u32 index)
{
	return cstr[index];
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