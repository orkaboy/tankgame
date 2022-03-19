#pragma once

#include "SDL2/SDL.h"

#include "utility.h"
#include "animation.h"
#include <vector>

namespace MDF {

class World;
class Tank;
class Player;
class Planet;


enum ProjectileType
{
	TURRET,
	FLAMER,
	CLUSTER,
	CLUSTER_BIT,
	ROCKET,
	NUKE,
	AMMO_TURRET,
	AMMO_FLAMER,
	AMMO_CLUSTER,
	AMMO_ROCKET,
	AMMO_NUKE,
};

constexpr int CLUSTER_COUNT = 12;

class Projectile
{
public:
	/* creats a projectile at position for players tank with vinkel and kraft */
	Projectile(MDF::Player *player, float kraft, vec2 pos, float vinkel, ProjectileType type);

	void Hit(World* world, MDF::Tank* target, MDF::Planet* planet, bool &removal, float dt );

	void Update(World* world, bool &removal, float dt );

	static void Draw(std::vector<Projectile*> &projs, SDL_Renderer *screen, vec2 offset);

	vec2 pos;
	vec2 vel;
	MDF::Player* player;
	float radius;
	ProjectileType type;
	float LifeTime;
	MDF::Animation *animation;
    SDL_Texture *surface;
	unsigned int animFrame;
	float animTimer;
	float damage;
};


}
