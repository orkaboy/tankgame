#include "world.h"

#include "graphics.h"
#include "resources.h"
#include "projectile.h"

#include <fmt/core.h>

#include <fstream>
#include <map>
static std::map<std::string, World> worlds;

World World_GetWorld(std::string id)
{
	ResourceMap res = Resources_GetOfType(RT_WORLD);
	std::string path = res.find(id)->second;
	
	return World_Init(path);
}

World World_Init(std::string findpath)
{
	std::ifstream pfile;
	float tempx, tempy, tempr, tempm;
	std::string tempid;
	std::string dummy;
	unsigned int num_planets;
	
	World world;
	
	pfile.open(findpath.c_str(), std::ios::in);
	if(!pfile)
	{
		fmt::print("Couldent find Worlddefine file {}\n", findpath);
	}
	
	pfile >> dummy >> num_planets;
	
	for(unsigned int i = 0; i < num_planets; i++)
	{
		pfile >> dummy >> tempid;
		pfile >> dummy >> tempx >> tempy;
		pfile >> dummy >> tempr;
		pfile >> dummy >> tempm;
		
		Planet *planet = Planet_Init(tempx, tempy, tempr, tempm);
		
		Planet_SetImage(planet, tempid);
		
        world.planets.push_back(planet);
	}
	
	/** TEMP **/
	world.size = vec2(2000, 2000);

	world.timeSinceLastAmmo = 0.0;

	return world;
}

void World_SpawnAmmo(World &world, float dt)
{
	world.timeSinceLastAmmo += dt;
	
	if (world.timeSinceLastAmmo >= 10.0 ) {
		
		vec2 pos;
		
		pos.x = rand()%(int)world.size.x;
		pos.y = rand()%(int)world.size.y;
		
		ProjectileType type;
		
		switch (rand()%3)
		{
			case 0: type = AMMO_TURRET; break;
			case 1: type = AMMO_FLAMER; break;
			case 2: type = AMMO_CLUSTER; break;
			case 3: type = AMMO_ROCKET; break;
		}
		
		Projectile* p = Projectile_Create(NULL, 0.0, pos, 0.0, type);
		
        world.projectiles.push_back(p);
		world.timeSinceLastAmmo = 0.0;
	}
}

void World_DeInit(World &world)
{
	for(unsigned int i = 0; i < world.planets.size(); i++)
		Planet_Free(world.planets[i]);
	for(unsigned int i = 0; i < world.tanks.size(); i++)
		Tank_Free(world.tanks[i]);

	for(unsigned int i = 0; i < world.projectiles.size(); i++)
		Projectile_Destroy(world.projectiles[i]);
	for(unsigned int i = 0; i < world.effects.size(); i++)
		Effect_Delete(world.effects[i]);
}
