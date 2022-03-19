#include "world.h"

#include "graphics.h"
#include "resources.h"
#include "projectile.h"

#include <fmt/core.h>

#include <fstream>
#include <map>

namespace MDF {

static std::map<std::string, World> worlds;

World World::GetWorld(std::string id)
{
	MDF::Resource::ResourceMap res = MDF::Resource::GetOfType(MDF::Resource::Type::WORLD);
	std::string path = res.find(id)->second;
	
	return World(path);
}

World::World(std::string findpath)
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

	for(unsigned int i = 0; i < num_planets; i++)
	{
		std::string tempid;
		float tempx, tempy, tempr, tempm;

		pfile >> dummy >> tempid;
		pfile >> dummy >> tempx >> tempy;
		pfile >> dummy >> tempr;
		pfile >> dummy >> tempm;

		auto planet = new MDF::Planet(vec2(tempx, tempy), tempr, tempm);

		Planet_SetImage(planet, tempid);

        planets.push_back(planet);
	}

	/** TEMP **/
	size = vec2(2000, 2000);

	timeSinceLastAmmo = 0.0;
}

void World::SpawnAmmo(float dt)
{
	timeSinceLastAmmo += dt;

	if (timeSinceLastAmmo >= 10.0 ) {
		vec2 pos;

		pos.x = rand()%(int)size.x;
		pos.y = rand()%(int)size.y;

		ProjectileType type;

		switch (rand()%3)
		{
			case 0: type = AMMO_TURRET; break;
			case 1: type = AMMO_FLAMER; break;
			case 2: type = AMMO_CLUSTER; break;
			case 3: type = AMMO_ROCKET; break;
		}

		Projectile* p = Projectile_Create(NULL, 0.0, pos, 0.0, type);

        projectiles.push_back(p);
		timeSinceLastAmmo = 0.0;
	}
}

World::~World()
{
	for(auto planet : planets)
		delete planet;
	for(auto tank : tanks)
		delete tank;

	for(auto proj : projectiles)
		delete proj;
	for(auto effect : effects)
		delete effect;
}

}
