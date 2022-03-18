#pragma once

#include "SDL2/SDL.h"

#include "animation.h"

constexpr int MAX_WEAPONS = 5;

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
	
    SDL_Texture *tankTurret;
    SDL_Texture *tankCursor;
	
	Animation *FireAnimation;
	
	float weaponDelay;
	float outgoingPower;
} Weapon;

extern Weapon weapons[MAX_WEAPONS];

void Weapon_Fire( Player *player, Weapon &weapon, World &world );
float Weapon_GetDelay(int weapon_index);
void LoadWeapons(void);
