#ifndef IFUM_HPP
#define IFUM_HPP

//@todo actual proper crossplatform types
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef signed long long s64;
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

typedef double f64;
typedef float f32;

constexpr f64 PI=3.1415926535897;

//@todo math functions, or use just glm/cmath... ? might have to reimplement them for console etc. 

char *cstr_copy(char const *src, char *dest);
u32 cstr_len(char const *cstr);

struct string
{
	char *cstr;
	string();
	string(char const *str);
	string(char c);
	string(string const &other);//slow
	~string();
	string slice(u32 start, u32 end);
	string concat(string const &str);
	bool equals(string const &str) const;
	string &operator=(string const &other);
	u32 size() const;
	char &operator[](u32 index);
};
/*
@todo efficient string, s64_map, and list and replace the usage in put and system (if these are even necessary)
also string functions for printing bools, ints, floats, vec2s, etc.
*/
/*
template<typename b>
struct s64_map
{
	
};
template<typename t>
struct list
{
	t *block;
	t &operator[]( u32 index );
	t pop();
	void push(t);
	t remove(u32 index);
	void insert(u32 index);
};
*/

struct vec2
{
	f64 x, y;
	
	vec2();
	vec2( f64 angle );
	vec2( f64 x, f64 y );

	vec2 operator-();
	vec2 operator+( vec2 const &other );
	vec2 operator-( vec2 const &other );
	vec2 operator*( vec2 const &other );
	vec2 operator/( vec2 const &other );
	vec2 &operator+=( vec2 const &other );
	vec2 &operator-=( vec2 const &other );
	vec2 &operator*=( vec2 const &other );
	vec2 &operator/=( vec2 const &other );
	vec2 operator/( f64 factor );
	vec2 &operator/=( f64 factor );
	vec2 &operator*=( f64 factor );
	vec2 operator*( f64 factor );
	f64 &operator[]( u32 index );

    bool significant();
	vec2 scale(f64 factor);
	f64 to_angle();
	f64 len();
	vec2 abs();
	vec2 unit();
};

struct rect : vec2
{
	f64 w, h;
	rect();
	rect( f64 x, f64 y, f64 w, f64 h );
};

bool intersects( rect a, rect b );

#endif
