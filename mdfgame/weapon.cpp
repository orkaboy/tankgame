#include "weapon.h"
#include "world.h"
#include "graphics.h"

#include <cmath>

namespace MDF {

Weapon weapons[MAX_WEAPONS];

float Weapon::GetDelay(int weapon_index)
{
	return weapons[weapon_index].weaponDelay;
}

void Weapon::LoadWeapons(void)
{
	/* WEP_TURRET */
	weapons[WEP_TURRET].type = WEP_TURRET;
	weapons[WEP_TURRET].tankTurret = Graphics::getImage("TankCannon");
	weapons[WEP_TURRET].tankCursor = Graphics::getImage("TankCursor");
	
	weapons[WEP_TURRET].fireAnimation = NULL;

	weapons[WEP_TURRET].weaponDelay = 0.2;
	weapons[WEP_TURRET].outgoingPower = 200.f;

	/* WEP_FLAMER */
	weapons[WEP_FLAMER].type = WEP_FLAMER;
	weapons[WEP_FLAMER].tankTurret = Graphics::getImage("FlamerCannon");
	weapons[WEP_FLAMER].tankCursor = Graphics::getImage("TankCursor");
	
	weapons[WEP_FLAMER].fireAnimation = NULL;

	weapons[WEP_FLAMER].weaponDelay = 0.05;
	weapons[WEP_FLAMER].outgoingPower = 180.f;
	
	/* WEP_CLUSTER */
	weapons[WEP_CLUSTER].type = WEP_CLUSTER;
	weapons[WEP_CLUSTER].tankTurret = Graphics::getImage("ClusterCannon");
	weapons[WEP_CLUSTER].tankCursor = Graphics::getImage("TankCursor");
	
	weapons[WEP_CLUSTER].fireAnimation = NULL;

	weapons[WEP_CLUSTER].weaponDelay = 1.0;
	weapons[WEP_CLUSTER].outgoingPower = 200.f;

	/* WEP_ROCKET */
	weapons[WEP_ROCKET].type = WEP_ROCKET;
	weapons[WEP_ROCKET].tankTurret = Graphics::getImage("ClusterCannon");
	weapons[WEP_ROCKET].tankCursor = Graphics::getImage("TankCursor");
	
	weapons[WEP_ROCKET].fireAnimation = NULL;

	weapons[WEP_ROCKET].weaponDelay = 1.0;
	weapons[WEP_ROCKET].outgoingPower = 100.f;

	/* WEP_JET */
	weapons[WEP_JET].type = WEP_JET;
	weapons[WEP_JET].tankTurret = Graphics::getImage("JetCannon");
	weapons[WEP_JET].tankCursor = Graphics::getImage("JetCursor");

	weapons[WEP_JET].fireAnimation = MDF::Animation::GetAnimation("JetFire");
	
	weapons[WEP_JET].weaponDelay = 0.05;
	weapons[WEP_JET].outgoingPower = 20.f;
}

void Weapon::Fire( MDF::Player *player, World &world )
{
	auto tank = player->GetTank();
	if(tank->Dying())
		return;
	if(tank->TimeSinceLastFire() > 0)
		return;
	
	tank->TimeSinceLastFire() = weaponDelay;
	//tank->fireTimer = 0;

	auto planet = tank->GetPlanet();
	auto tankPos = tank->Pos();
	
	switch(type)
	{
		case WEP_TURRET:
		{
			float vinkel = tank->TurretAngle() + tank->AngularPos() + planet->Rot();

			auto p = new Projectile(player, outgoingPower, tankPos, vinkel, TURRET); // vel in pixels/sec

			p->pos = vec2Add(tankPos, tank->Turret());

            world.projectiles.push_back(p);

			MDF::Audio::PlayCanonFire();
		}
		break;
		case WEP_FLAMER:
		{
			float vinkel = tank->TurretAngle() + tank->AngularPos() + planet->Rot() + (float)(rand()%10)/20.0 - 0.25;
			
			auto p = new Projectile(player, outgoingPower, tankPos, vinkel, FLAMER);
			
			p->pos = vec2Add(tankPos, tank->Turret());
			
            world.projectiles.push_back(p);
			
			MDF::Audio::PlayCanonFire();
		}
		break;
		case WEP_CLUSTER:
		{
			float vinkel = tank->TurretAngle() + tank->AngularPos() + planet->Rot();
			
			auto p = new Projectile(player, outgoingPower, tankPos, vinkel, CLUSTER); // vel in pixels/sec
			
			p->pos = vec2Add(tankPos, tank->Turret());
			
            world.projectiles.push_back(p);
			
			MDF::Audio::PlayCanonFire();
		}
		break;
		case WEP_ROCKET:
		{
			float vinkel = tank->TurretAngle() + tank->AngularPos() + planet->Rot();

			auto p = new Projectile(player, outgoingPower, tankPos, vinkel, ROCKET); // vel in pixels/sec

			p->pos = vec2Add(tankPos, tank->Turret());

            world.projectiles.push_back(p);

			MDF::Audio::PlayCanonFire();
		}
		break;
		
		case WEP_JET:
		{
			float tankRotation = tank->AngularPos() + planet->Rot();

			/* x component is rotation, y comp is acc-force */
			vec2 force;
			force.x = cos(tank->TurretAngle() + (M_PI / 2)) / 5;
			force.y = sin(tank->TurretAngle() + (M_PI / 2)) * outgoingPower;
			
			/* acc-force */
			vec2 finalforce;
			finalforce.x = -cos(tankRotation) * force.y;
			finalforce.y = sin(tankRotation) * force.y;
			/* Apply vel to planet */
			planet->Vel() = vec2Add(planet->Vel(), finalforce);
			/* Rotate planet */
			planet->RotVel() += force.x;
		}
		break;
		
	}
}

}
