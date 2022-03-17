#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "SDL2/SDL.h"

#include "utility.h"
#include "animation.h"
#include <vector>

struct World;
struct Tank;
struct Player;
struct Planet;

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

typedef struct Projectile
{
	vec2 pos;
	vec2 vel;
	Player* player;
	float radius;
	ProjectileType type;
	float LifeTime;
	Animation *animation;
    SDL_Texture *surface;
	unsigned int animFrame;
	float animTimer;
	float damage;
} Projectile;

/* creats a projectile at position for players tank with vinkel and kraft */
Projectile *Projectile_Create(Player *player, float kraft, vec2 pos, float vinkel, ProjectileType type);

void Projectile_Hit(Projectile* projectile, World &world, Tank* target, Planet* planet, bool &removal, float dt );

void Projectile_Update(Projectile* projectile, World &world, bool &removal, float dt );

/* removes projectile from the game */
void Projectile_Destroy(Projectile *projectile);

void Projectile_Draw(std::vector<Projectile*> &proj, SDL_Renderer *screen, vec2 offset);

#endif
