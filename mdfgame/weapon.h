#ifndef WEAPON_H
#define WEAPON_H

#ifdef _WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "animation.h"

#define MAX_WEAPONS 5

struct World;
struct Player;

enum TankWeapon
{
	WEP_TURRET,
	WEP_FLAMER,
	WEP_CLUSTER,
	WEP_ROCKET,
	WEP_JET,
};

typedef struct Weapon
{
	TankWeapon type;
	
	SDL_Surface *tankTurret;
	SDL_Surface *tankCursor;
	
	Animation *FireAnimation;
	
	float weaponDelay;
	float outgoingPower;
} Weapon;

extern Weapon weapons[MAX_WEAPONS];

void Weapon_Fire( Player *player, Weapon &weapon, World &world );
float Weapon_GetDelay(int weapon_index);
void LoadWeapons(void);

#endif
