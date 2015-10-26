#include "world.h"

#include "graphics.h"
#include "resources.h"
#include "projectile.h"

#include <fstream>
#include <map>
static map<string, World> worlds;

World World_GetWorld(string id)
{
	ResourceMap res = Resources_GetOfType(RT_WORLD);
	string path = res.find(id)->second;
	
	return World_Init(path);
}

World World_Init(string findpath)
{
	ifstream pfile;
	float tempx, tempy, tempr, tempm;
	string tempid;
	string dummy;
	unsigned int num_planets;
	
	World world;
	
	pfile.open(findpath.c_str(), ios::in);
	if(!pfile)
	{
		printf("Couldent find Worlddefine file %s\n", findpath.c_str());
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
