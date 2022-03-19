#pragma once

#include "projectile.h"
#include "utility.h"
#include "audio.h"
#include "weapon.h"

#include "SDL2/SDL.h"

constexpr int MAX_HITPOINTS = 100;


namespace MDF {

struct World;
class Player;
class Planet;

enum TankColors
{
	TANK_NEUTRAL,
	TANK_BLUE,
	TANK_GREEN,
	TANK_RED,
	TANK_YELLOW,
};

enum class Direction
{
	RIGHT,
	LEFT
};

class Tank
{
public:
	/* New fire function */
	static void FireWeapon ( MDF::Player *player, World &world, float dt );

	/* spawn tank */
	static void Spawn ( World& world, MDF::Player* player, Planet* planet );

	/* move tank in dir */
	void Move ( MDF::Direction dir );
	/* teleport tank to planet */
	void Teleport ( World& world);
	/* rotate tank turrent */
	void SetTurretRotation(vec2 target);
	void RotateTurret ( MDF::Direction dir );
	void RotateTurret ( float relangle );

	void NextWeapon();

	/* destroy tank, set tank to dead! */
	void Destroy ();

	/* Drawing function */
	void Draw( vec2 offset ) const;

	/* Getters and Setters */
	auto Pos() const -> vec2 { return pos; }
	auto Pos() -> vec2& { return pos; }

	auto BoundingRadius() const -> float { return bounding_radius; }

	auto HP() -> float& { return hitPoints; }
	auto HP() const -> float { return hitPoints; }

	auto GetPlanet() -> Planet* { return planet; }

	auto GetPlayer() -> Player* { return player; }

	auto Firing() const -> bool { return firing; }
	void SetFiring(bool f) { firing = f; }

	auto Dying() const -> bool { return dying; }
	void SetDying(bool d) { dying = d; }

	auto DyingTimer() const -> float { return dyingTimer; }
	auto DyingTimer() -> float& { return dyingTimer; }

	auto TeleportTimer() const -> float { return teleportTimer; }
	auto TeleportTimer() -> float& { return teleportTimer; }

	auto GetWeapon() const -> int { return weapon; }

	auto AngularPos() const -> float { return angular_position; }
	auto AngularPos() -> float& { return angular_position; }

	auto Turret() const -> vec2 { return turret; }
	auto Turret() -> vec2& { return turret; }

	auto TurretAngle() const -> float { return turret_angle; }
	auto TurretAngle() -> float& { return turret_angle; }

	auto TimeSinceLastFire() const -> float { return timeSinceLastFire; }
	auto TimeSinceLastFire() -> float& { return timeSinceLastFire; }

	auto Ammo(int index) const -> int { if(index < MAX_WEAPONS) return ammo[index]; else return ammo[0]; }
	auto Ammo(int index) -> int& { if(index < MAX_WEAPONS) return ammo[index]; else return ammo[0]; }

	void SetTankBody(SDL_Texture* body) { tankBody = body; }

private:
	bool dying, firing;
	float barrel_pressure;
	
	Player *player;
	
    SDL_Texture *tankBody;
	
	vec2 pos;
	float bounding_radius;
	Planet* planet;
	float angular_position;
	float turret_angle;
	vec2 turret; //position for canon tip, used only in rendering
	
	float teleportTimer;
	float timeSinceLastFire;

	float dyingTimer;
	float fireTimer;
	unsigned int fireFrame;
	
	float hitPoints;
	
	int weapon;
	int ammo[MAX_WEAPONS];
};

}
