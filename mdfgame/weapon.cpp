#include "weapon.h"
#include "world.h"
#include "graphics.h"

#include <cmath>

Weapon weapons[MAX_WEAPONS];

float Weapon_GetDelay(int weapon_index)
{
	return weapons[weapon_index].weaponDelay;
}

void LoadWeapons(void)
{
	weapons[WEP_TURRET].type = WEP_TURRET;
	weapons[WEP_TURRET].tankTurret = getImage("TankCannon");
	weapons[WEP_TURRET].tankCursor = getImage("TankCursor");
	
	weapons[WEP_TURRET].FireAnimation = NULL;

	weapons[WEP_TURRET].weaponDelay = 0.2;
	weapons[WEP_TURRET].outgoingPower = 200.f;

	weapons[WEP_FLAMER].type = WEP_FLAMER;
	weapons[WEP_FLAMER].tankTurret = getImage("FlamerCannon");
	weapons[WEP_FLAMER].tankCursor = getImage("TankCursor");
	
	weapons[WEP_FLAMER].FireAnimation = NULL;

	weapons[WEP_FLAMER].weaponDelay = 0.05;
	weapons[WEP_FLAMER].outgoingPower = 180.f;
	
	weapons[WEP_CLUSTER].type = WEP_CLUSTER;
	weapons[WEP_CLUSTER].tankTurret = getImage("ClusterCannon");
	weapons[WEP_CLUSTER].tankCursor = getImage("TankCursor");
	
	weapons[WEP_CLUSTER].FireAnimation = NULL;

	weapons[WEP_CLUSTER].weaponDelay = 1.0;
	weapons[WEP_CLUSTER].outgoingPower = 200.f;

	weapons[WEP_ROCKET].type = WEP_ROCKET;
	weapons[WEP_ROCKET].tankTurret = getImage("ClusterCannon");
	weapons[WEP_ROCKET].tankCursor = getImage("TankCursor");
	
	weapons[WEP_ROCKET].FireAnimation = NULL;

	weapons[WEP_ROCKET].weaponDelay = 1.0;
	weapons[WEP_ROCKET].outgoingPower = 100.f;

	/* JET gun */
	weapons[WEP_JET].type = WEP_JET;
	weapons[WEP_JET].tankTurret = getImage("JetCannon");
	weapons[WEP_JET].tankCursor = getImage("JetCursor");
	
	/* Fire animations */
	weapons[WEP_JET].FireAnimation = MDF::Animation::GetAnimation("JetFire");
	
	weapons[WEP_JET].weaponDelay = 0.05;
	weapons[WEP_JET].outgoingPower = 20.f;
}

void Weapon_Fire( MDF::Player *player, Weapon &weapon, World &world )
{
	auto tank = player->GetTank();
	if(tank->dying)
		return;
	if(tank->timeSinceLastFire > 0)
		return;
	
	tank->timeSinceLastFire = weapon.weaponDelay;
	//tank->fireTimer = 0;
	
	switch(weapon.type)
	{
		case WEP_TURRET:
		{
			float vinkel = tank->turret_angle + tank->angular_position + tank->planet->rot;
			
			Projectile* p = Projectile_Create(player, weapon.outgoingPower, tank->pos, vinkel, TURRET); // vel in pixels/sec
			
			p->pos = vec2Add(tank->pos, tank->turret);
			
            world.projectiles.push_back(p);
			
			Audio_PlayCanonFire();
		}
		break;
		case WEP_FLAMER:
		{
			float vinkel = tank->turret_angle + tank->angular_position + tank->planet->rot + (float)(rand()%10)/20.0 - 0.25;
			
			Projectile* p = Projectile_Create(player, weapon.outgoingPower, tank->pos, vinkel, FLAMER);
			
			p->pos = vec2Add(tank->pos, tank->turret);
			
            world.projectiles.push_back(p);
			
			Audio_PlayCanonFire();
		}
		break;
		case WEP_CLUSTER:
		{
			float vinkel = tank->turret_angle + tank->angular_position + tank->planet->rot;
			
			Projectile* p = Projectile_Create(player, weapon.outgoingPower, tank->pos, vinkel, CLUSTER); // vel in pixels/sec
			
			p->pos = vec2Add(tank->pos, tank->turret);
			
            world.projectiles.push_back(p);
			
			Audio_PlayCanonFire();
		}
		break;
		case WEP_ROCKET:
		{
			float vinkel = tank->turret_angle + tank->angular_position + tank->planet->rot;
			
			Projectile* p = Projectile_Create(player, weapon.outgoingPower, tank->pos, vinkel, ROCKET); // vel in pixels/sec
			
			p->pos = vec2Add(tank->pos, tank->turret);
			
            world.projectiles.push_back(p);
			
			Audio_PlayCanonFire();
		}
		break;
		
		case WEP_JET:
		{
			float tankRotation = tank->angular_position + tank->planet->rot;
			
			/* x component is rotation, y comp is acc-force */
			vec2 force;
			force.x = cos(tank->turret_angle + (M_PI / 2)) / 5;
			force.y = sin(tank->turret_angle + (M_PI / 2)) * weapon.outgoingPower;
			
			/* acc-force */
			vec2 finalforce;
			finalforce.x = -cos(tankRotation) * force.y;
			finalforce.y = sin(tankRotation) * force.y;
			/* Apply vel to planet */
			tank->planet->vel.x += finalforce.x;
			tank->planet->vel.y += finalforce.y;
			/* Rotate planet */
			tank->planet->rotvel += force.x;
		}
		break;
		
	}
}
