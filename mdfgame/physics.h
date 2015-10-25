#ifndef PHYSICS_H
#define PHYSICS_H

#define GRAVITY 6.67259

#include "world.h"
#include "utility.h"

void Physics(World &w, float dt);

void Physics_UpdatePlanets(World &w, float dt);

void Physics_PlanetBounce(Planet *p1, Planet *p2);

void Physics_UpdateTanks(World &w, float dt);

void Physics_UpdateProjectiles(World &w, float dt);

#endif
