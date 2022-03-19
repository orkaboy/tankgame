#include "utility.h"
#include <cmath>

namespace MDF {

float vec2Length2(vec2 v)
{
	return v.x*v.x + v.y*v.y;
}

float vec2Length(vec2 v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

vec2 vec2Normalize(vec2 v)
{
	vec2 n;
	float length = vec2Length(v);
	if(length == 0)
	{
		n.x = 0;
		n.y = 0;
	}
	else
	{
		n.x = v.x / length;
		n.y = v.y / length;
	}
	return n;
}

float vec2DotProd(vec2 v1, vec2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float vec2CrossFail(vec2 v1, vec2 v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

vec2 vec2Multiply(vec2 v, float f)
{
	v.x *= f;
	v.y *= f;
	return v;
}

vec2 vec2Add(vec2 v1, vec2 v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
}

vec2 vec2Sub(vec2 v1, vec2 v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	return v1;
}

}
