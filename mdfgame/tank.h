#ifndef TANK_H
#define TANK_H

#include "projectile.h"
#include "utility.h"
#include "audio.h"
#include "weapon.h"

#include "SDL2/SDL.h"

#define MAX_HITPOINTS 100

struct Player;
struct Planet;
struct World;

enum TankColors
{
	TANK_NEUTRAL,
	TANK_BLUE,
	TANK_GREEN,
	TANK_RED,
	TANK_YELLOW,
};

enum Direction
{
	DIR_RIGHT,
	DIR_LEFT
};

typedef struct Tank
{
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
} Tank;

/* New fire function */
void Tank_FireWeapon ( Player *player, World &world, float dt );

/* spawn tank */
void Tank_Spawn ( World& world, Player* player, Planet* planet );

/* move tank in dir */
void Tank_Move ( Tank* tank, Direction dir );
/* teleport tank to planet */
void Tank_Teleport ( Tank* tank, World& world);
/* rotate tank turrent */
void Tank_SetTurretRotation(Tank *tank, Sint16 x, Sint16 y);
void Tank_RotateTurret ( Tank* tank, Direction dir );
void Tank_RotateTurret ( Tank* tank, float relangle );

void Tank_NextWeapon( Tank* tank );

/* destroy tank, set tank to dead! */
void Tank_Destroy ( Tank* tank );
/* free tank, after this tank is no more! */
void Tank_Free ( Tank* tank );

/* Drawing function */
void Tank_Draw( const Tank *tank, vec2 offset );

#endif
