#pragma once

#include "SDL2/SDL.h"

#include "animation.h"

constexpr int MAX_WEAPONS = 5;


namespace MDF {

class World;
class Player;

enum TankWeapon
{
	WEP_TURRET,
	WEP_FLAMER,
	WEP_CLUSTER,
	WEP_ROCKET,
	WEP_JET,
};

class Weapon
{
public:
	static void LoadWeapons();
	void Fire( MDF::Player *player, World* world );
	static float GetDelay(int weapon_index);

	auto FireAnimation() const -> Animation* { return fireAnimation; }
	auto TankTurret() const -> SDL_Texture* { return tankTurret; }
	auto TankCursor() const -> SDL_Texture* { return tankCursor; }

private:
	TankWeapon type;
	
    SDL_Texture *tankTurret;
    SDL_Texture *tankCursor;
	
	Animation *fireAnimation;
	
	float weaponDelay;
	float outgoingPower;
};

extern Weapon weapons[MAX_WEAPONS];

}
