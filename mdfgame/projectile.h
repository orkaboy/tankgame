#pragma once

#include "SDL2/SDL.h"

#include "utility.h"
#include "animation.h"
#include <vector>

namespace MDF {

struct World;
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

typedef struct Projectile
{
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
} Projectile;

/* creats a projectile at position for players tank with vinkel and kraft */
Projectile *Projectile_Create(MDF::Player *player, float kraft, vec2 pos, float vinkel, ProjectileType type);

void Projectile_Hit(Projectile* projectile, World &world, MDF::Tank* target, MDF::Planet* planet, bool &removal, float dt );

void Projectile_Update(Projectile* projectile, World &world, bool &removal, float dt );

void Projectile_Draw(std::vector<Projectile*> &proj, SDL_Renderer *screen, vec2 offset);

}
