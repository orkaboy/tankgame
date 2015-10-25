#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>

#ifndef M_PI
#define M_PI 3.141592653589793238
#endif
using namespace std;

struct vec2
{
	union
	{
		float x, width;
	};
	union
	{
		float y, height;
	};
	vec2() : x(0), y(0) {}
	vec2(float X, float Y) : x(X), y(Y) {}
};

vec2 vec2Normalize(vec2 v);
float vec2DotProd(vec2 v1, vec2 v2);
vec2 vec2Multiply(vec2 v, float f);
vec2 vec2Add(vec2 v1, vec2 v2);
vec2 vec2Sub(vec2 v1, vec2 v2);
float vec2CrossFail(vec2 v1, vec2 v2);
float vec2Length(vec2 v);
float vec2Length2(vec2 v);


template<class T>
string convertToStr(T value)
{
	std::ostringstream o;
	if (!(o << value))
		return "";
	return o.str();
};



#endif
