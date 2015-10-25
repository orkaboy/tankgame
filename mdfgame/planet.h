#ifndef PLANET_H
#define PLANET_H

#include "utility.h"

#ifdef _WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

typedef struct Planet
{
	vec2 pos;
	float radius;
	float rot;
	float rotvel;
	vec2 vel;
	float mass;
	SDL_Surface *image;
} Planet;

Planet *Planet_Init(float x, float y, float radius, float mass);
void Planet_Free(Planet *planet);

#endif
