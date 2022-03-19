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
	if(tank->Dying())
		return;
	if(tank->TimeSinceLastFire() > 0)
		return;
	
	tank->TimeSinceLastFire() = weapon.weaponDelay;
	//tank->fireTimer = 0;

	auto planet = tank->GetPlanet();
	auto tankPos = tank->Pos();
	
	switch(weapon.type)
	{
		case WEP_TURRET:
		{
			float vinkel = tank->TurretAngle() + tank->AngularPos() + planet->rot;

			Projectile* p = Projectile_Create(player, weapon.outgoingPower, tankPos, vinkel, TURRET); // vel in pixels/sec

			p->pos = vec2Add(tankPos, tank->Turret());

            world.projectiles.push_back(p);

			Audio_PlayCanonFire();
		}
		break;
		case WEP_FLAMER:
		{
			float vinkel = tank->TurretAngle() + tank->AngularPos() + planet->rot + (float)(rand()%10)/20.0 - 0.25;
			
			Projectile* p = Projectile_Create(player, weapon.outgoingPower, tankPos, vinkel, FLAMER);
			
			p->pos = vec2Add(tankPos, tank->Turret());
			
            world.projectiles.push_back(p);
			
			Audio_PlayCanonFire();
		}
		break;
		case WEP_CLUSTER:
		{
			float vinkel = tank->TurretAngle() + tank->AngularPos() + planet->rot;
			
			Projectile* p = Projectile_Create(player, weapon.outgoingPower, tankPos, vinkel, CLUSTER); // vel in pixels/sec
			
			p->pos = vec2Add(tankPos, tank->Turret());
			
            world.projectiles.push_back(p);
			
			Audio_PlayCanonFire();
		}
		break;
		case WEP_ROCKET:
		{
			float vinkel = tank->TurretAngle() + tank->AngularPos() + planet->rot;
			
			Projectile* p = Projectile_Create(player, weapon.outgoingPower, tankPos, vinkel, ROCKET); // vel in pixels/sec
			
			p->pos = vec2Add(tankPos, tank->Turret());
			
            world.projectiles.push_back(p);
			
			Audio_PlayCanonFire();
		}
		break;
		
		case WEP_JET:
		{
			float tankRotation = tank->AngularPos() + planet->rot;
			
			/* x component is rotation, y comp is acc-force */
			vec2 force;
			force.x = cos(tank->TurretAngle() + (M_PI / 2)) / 5;
			force.y = sin(tank->TurretAngle() + (M_PI / 2)) * weapon.outgoingPower;
			
			/* acc-force */
			vec2 finalforce;
			finalforce.x = -cos(tankRotation) * force.y;
			finalforce.y = sin(tankRotation) * force.y;
			/* Apply vel to planet */
			planet->vel.x += finalforce.x;
			planet->vel.y += finalforce.y;
			/* Rotate planet */
			planet->rotvel += force.x;
		}
		break;
		
	}
}
