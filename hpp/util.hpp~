#pragma once

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <vector>
#include <chrono>
#include <map>
#include <string>

typedef unsigned char byte;

std::string gettag(std::string s, int tagnumber, int index);
std::string gettags(std::string s, int begin, int end);//0 end signifies to end of string
std::string loadFile(char const *path);//file path to string of contents
std::map<std::string,std::string> parseConfig(char const *path);//returns map of first carot enclosed key to second carot enclosed value
bool insec( SDL_Rect a, SDL_Rect b );//returns true if the rectangles are intersecting
int randNum(int min, int max);
std::pair<std::vector<std::string>,std::string> separate(char c,std::string stream,bool exclude);

struct Vec2
{
	double x, y;
	Vec2();
	Vec2( double xy );//NOTE: USE angleToVec2 NOT A CONSTRUCTOR TO CONVER AN ANGLE TO A VECTOR
	Vec2( double x, double y );
	Vec2 operator-();
	Vec2 operator+( Vec2 const &other );
	Vec2 operator-( Vec2 const &other );
	Vec2 operator*( Vec2 const &other );
	Vec2 operator/( Vec2 const &other );
	Vec2 &operator+=( Vec2 const &other );
	Vec2 &operator-=( Vec2 const &other );
	Vec2 &operator*=( Vec2 const &other );
	Vec2 &operator/=( Vec2 const &other );
	Vec2 operator/( double factor );
	Vec2 &operator/=( double factor );
	Vec2 &operator*=( double factor );
	Vec2 operator*( double factor );

	bool operator<( double num );
	bool operator>( double num );
	/*
	Vec2 &operator=(double num)
	{
		x=num;
		y=num;
		return *this;
	}*/
	//need operators for non-vector types? does int implicitly convert to float and vice versa?
	double &operator[]( int index );
    bool significant();
	Vec2 scale(double factor);
	double toangle();
	double len();
	Vec2 abs();
	Vec2 unit();

	bool sameSign(Vec2 const &other);
};

Vec2 angleToVec2(double angle);

struct Rect : Vec2
{
	double w, h;
	Rect();
	Rect( double x, double y, double w, double h );
};

/*
This whole class has gotta be the most inefficient thing of all time.
*/
class Clock
{
	long long getmillis(std::chrono::time_point<std::chrono::high_resolution_clock> tp);

	public:

	int FPS;
	long long start;
	long long oldclock;
	long long dclock;
	long long milliSinceConstruction;
	long long deltaMilli;
	float deltaSeconds;

	Clock();
    void update();
    void reset();
	long long getNow();
};

