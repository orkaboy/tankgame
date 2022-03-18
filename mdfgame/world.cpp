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
	MDF::Resource::ResourceMap res = MDF::Resource::GetOfType(MDF::Resource::Type::WORLD);
	std::string path = res.find(id)->second;
	
	return World_Init(path);
}

World World_Init(std::string findpath)
{
	std::ifstream pfile;

	pfile.open(findpath.c_str(), std::ios::in);
	if(!pfile)
	{
		fmt::print("Couldent find Worlddefine file {}\n", findpath);
	}

	std::string dummy;
	unsigned int num_planets;
	pfile >> dummy >> num_planets;

	World world;
	for(unsigned int i = 0; i < num_planets; i++)
	{
		std::string tempid;
		float tempx, tempy, tempr, tempm;

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
	for(auto planet : world.planets)
		Planet_Free(planet);
	for(auto tank : world.tanks)
		Tank_Free(tank);

	for(auto proj : world.projectiles)
		Projectile_Destroy(proj);
	for(auto effect : world.effects)
		delete effect;
}
