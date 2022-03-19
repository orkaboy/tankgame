#include "tank.h"
#include <cmath>

#include "audio.h"
#include "world.h"
#include "graphics.h"
#include "input.h"

#include <cmath>

namespace MDF {

/* New fire function */
void Tank::FireWeapon ( MDF::Player *player, World* world, float dt )
{
	Tank *tank = player->GetTank();
	auto& weapon = weapons[tank->weapon];
	weapon.Fire( player, world );
	auto anim = weapon.FireAnimation();
	if(anim)
		anim->UpdateAnimation(tank->fireFrame, tank->fireTimer, dt);
}

void Tank::SetTurretRotation(vec2 target)
{
	if(dying)
		return;
	vec2 direction = vec2Sub(target, pos);
	vec2 unit(1,0);

	direction = vec2Normalize(direction);
	direction.y *= -1;
	
	float theta = acos(vec2DotProd(direction, unit));
	
	float beta = planet->Rot() + angular_position;
	
	float alpha;
	if(direction.y > 0)
		alpha = theta - beta;
	else
		alpha = -beta - theta;

	while(alpha > M_PI) alpha -= 2*(float)M_PI;
	while(alpha < -M_PI) alpha += 2*(float)M_PI;
	
	if(alpha > M_PI / 2) alpha = (float)M_PI / 2;
	if(alpha < -M_PI / 2) alpha = -(float)M_PI / 2;
	turret_angle = alpha;
}

#include <cassert>

void Tank::RotateTurret ( float relangle )
{
	assert(false && "Fail");
	float nextang = turret_angle + relangle;

	if ( nextang > -(M_PI / 2) && nextang < M_PI / 2 )
		turret_angle = nextang;
}

void Tank::RotateTurret ( MDF::Direction dir )
{
	assert(false && "Fail");

	float mod = dir == MDF::Direction::RIGHT ? -1.0f : 1.0f;
	float nextang = turret_angle + mod * 4 * (float)(M_PI / 180);
	
	if ( nextang > -(M_PI / 2) && nextang < M_PI / 2 )
		turret_angle = nextang;
}

void Tank::NextWeapon()
{
	weapon = (weapon+1) % MAX_WEAPONS;
}

void Tank::Move ( MDF::Direction dir )
{
	if(dying)
		return;
	float mod = dir == MDF::Direction::RIGHT ? -1.0f : 1.0f;
	
	angular_position += mod * (float)(M_PI / 180);
	while(angular_position > 2 * M_PI) angular_position -= 2*(float)M_PI;
	while(angular_position < 0) angular_position += 2*(float)M_PI;
	
	if(!Audio::IsPlayingEngineSound())
		Audio::PlayEngineSound();
}

void Tank::Spawn ( World* world, MDF::Player* player_, Planet* planet_ )
{
	Tank* tank = new Tank;
	tank->planet = planet_;
	tank->dying = false;
	tank->firing = false;
	tank->turret_angle = 0.0f;
	tank->bounding_radius = 10.0f;
	
	tank->tankBody = NULL;
	
	tank->weapon = 0;
	
	tank->angular_position = 0;
	tank->timeSinceLastFire = 0;
	
	tank->teleportTimer = 0;
	
	tank->hitPoints = MAX_HITPOINTS;
	tank->fireTimer = 0;
	tank->fireFrame = 0;
	
	player_->SetTank(tank);
	tank->player = player_;
	
    world->tanks.push_back(tank);
}

void Tank::Destroy ()
{
	hitPoints = 0;
	if(dying) return;

	MDF::Audio::PlayExplosionBig();
	player->Deaths()++;
	dying = true;
	dyingTimer = 1; //Animlength
}

void Tank::Teleport ( World* world)
{
	if(dying) return;
	if(teleportTimer > 0) return;
	
	teleportTimer = 30;
	int planet_, size;
	/* get number of planets */
	size = world->planets.size();
	/* get random planet */
	planet_ = rand() % size;
	planet = world->planets[planet_];
}

void Tank::Draw( vec2 offset ) const
{
    SDL_Texture *body = tankBody;
	const auto& weapon_ = weapons[weapon];
    SDL_Texture *turret_ = weapon_.TankTurret();
	
	float tank_rot = angular_position + planet->Rot();
    Graphics::ApplySurface(body, (int)pos.x - offset.x, (int)pos.y - offset.y, 1, tank_rot - ((float)M_PI / 2));

	float turretX, turretY;
	turretX = (turret.x + 2*pos.x) / 2;
	turretY = (turret.y + 2*pos.y) / 2;

    Graphics::ApplySurface(turret_, (int)turretX - offset.x, (int)turretY - offset.y, 1, tank_rot + turret_angle);

    SDL_Texture *cursor_tex = weapon_.TankCursor();

	if(cursor_tex != NULL)
	{
        int w, h;
        SDL_QueryTexture(cursor_tex, NULL, NULL, &w, &h);
		//cursorX = (turret.x*4 + 2*pos.x) / 2;
		//cursorY = (turret.y*4 + 2*pos.y) / 2;
		auto cursor = Input::GetMousePos();
        cursor.x -= w / 2;
        cursor.y -= h / 2;

        Graphics::ApplySurface(cursor_tex, cursor.x, cursor.y, 1, 0);
	}

	auto anim = weapon_.FireAnimation();
	if(firing && anim)
	{
        SDL_Texture *fireAnim = anim->GetFrame(fireFrame);

		if(fireAnim != NULL)
		{
			float fireX = (turret.x*4 + 2*pos.x) / 2;
			float fireY = (turret.y*4 + 2*pos.y) / 2;

            Graphics::ApplySurface(fireAnim, (int)fireX - offset.x, (int)fireY - offset.y, 1, tank_rot + turret_angle);
		}
	}
}

}
