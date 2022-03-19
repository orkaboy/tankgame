#pragma once

#include "utility.h"

#include "SDL2/SDL.h"

typedef struct Planet
{
	vec2 pos;
	float radius;
	float rot;
	float rotvel;
	vec2 vel;
	float mass;
    SDL_Texture *image;
} Planet;

Planet *Planet_Init(float x, float y, float radius, float mass);
