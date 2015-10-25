#include "planet.h"
#include "net.h"

Planet *Planet_Init(float x, float y, float radius, float mass)
{
	Planet* tempPlanet = new Planet;
	tempPlanet->pos.x = x;
	tempPlanet->pos.y = y;
	tempPlanet->rot = 0;
	tempPlanet->rotvel = 0;
	tempPlanet->vel.x = 0;
	tempPlanet->vel.y = 0;
	tempPlanet->radius = radius;
	tempPlanet->mass = mass;
	return tempPlanet;
}

void Planet_Free(Planet *planet)
{
	Net_UnRegisterEntity(planet);
	delete planet;
}
