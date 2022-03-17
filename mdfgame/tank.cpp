#include "tank.h"
#include <cmath>

#include "audio.h"
#include "world.h"
#include "graphics.h"

#include <cmath>

/* New fire function */
void Tank_FireWeapon ( Player *player, World &world, float dt )
{
	Tank *tank = player->tank;
	auto& weapon = weapons[tank->weapon];
	Weapon_Fire( player, weapon, world );
	UpdateAnimation(weapon.FireAnimation, tank->fireFrame, tank->fireTimer, dt);
}

void Tank_SetTurretRotation(Tank *tank, Sint16 x, Sint16 y)
{
	if(tank->dying)
		return;
	vec2 pos(x, y);
	vec2 direction = vec2Sub(pos, tank->pos);
	vec2 unit(1,0);

	direction = vec2Normalize(direction);
	direction.y *= -1;
	
	float theta = acos(vec2DotProd(direction, unit));
	
	float beta = tank->planet->rot + tank->angular_position;
	
	float alpha;
	if(direction.y > 0)
		alpha = theta - beta;
	else
		alpha = -beta - theta;

	while(alpha > M_PI) alpha -= 2*(float)M_PI;
	while(alpha < -M_PI) alpha += 2*(float)M_PI;
	
	if(alpha > M_PI / 2) alpha = (float)M_PI / 2;
	if(alpha < -M_PI / 2) alpha = -(float)M_PI / 2;
	tank->turret_angle = alpha;
}

#include <cassert>

void Tank_RotateTurret ( Tank* tank, float relangle )
{
	assert(false && "Fail");
	float nextang = tank->turret_angle + relangle;

	if ( nextang > -(M_PI / 2) && nextang < M_PI / 2 )
		tank->turret_angle = nextang;
}

void Tank_RotateTurret ( Tank* tank, Direction dir )
{
	assert(false && "Fail");

	float mod = dir == Direction::RIGHT ? -1.0f : 1.0f;
	float nextang = tank->turret_angle + mod * 4 * (float)(M_PI / 180);
	
	if ( nextang > -(M_PI / 2) && nextang < M_PI / 2 )
		tank->turret_angle = nextang;
}

void Tank_NextWeapon( Tank* tank )
{
	tank->weapon = (tank->weapon+1) % MAX_WEAPONS;
}

void Tank_Move ( Tank* tank, Direction dir )
{
	if(tank->dying)
		return;
	float mod = dir == Direction::RIGHT ? -1.0f : 1.0f;
	
	tank->angular_position += mod * (float)(M_PI / 180);
	while(tank->angular_position > 2 * M_PI) tank->angular_position -= 2*(float)M_PI;
	while(tank->angular_position < 0) tank->angular_position += 2*(float)M_PI;
	
	if(!Audio_IsPlayingEngineSound())
		Audio_PlayEngineSound();
}

void Tank_Spawn ( World& world, Player* player, Planet* planet )
{
	Tank* tank = new Tank;
	tank->planet = planet;
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
	
	player->tank = tank;
	tank->player = player;
	
    world.tanks.push_back(tank);
}

void Tank_Destroy ( Tank* tank )
{
	tank->hitPoints = 0;
	if(tank->dying) return;

	Audio_PlayExplosionBig();
	tank->player->deaths++;
	tank->dying = true;
	tank->dyingTimer = 1; //Animlength
}

void Tank_Free ( Tank* tank )
{
	delete tank;
}

void Tank_Teleport ( Tank* tank, World& world)
{
	if(tank->dying) return;
	if(tank->teleportTimer > 0) return;
	
	tank->teleportTimer = 30;
	int planet, size;
	/* get number of planets */
	size = world.planets.size();
	/* get random planet */
	planet = rand() % size;
	tank->planet = world.planets[planet];
}

void Tank_Draw( const Tank *tank, vec2 offset )
{
    SDL_Texture *body = tank->tankBody;
	const auto& weapon = weapons[tank->weapon];
    SDL_Texture *turret = weapon.tankTurret;
		
	float tank_rot = tank->angular_position + tank->planet->rot;
    Graphics_ApplySurface(body, (int)tank->pos.x - offset.x, (int)tank->pos.y - offset.y, 1, tank_rot - ((float)M_PI / 2));
	
	float turretX, turretY;
	turretX = (tank->turret.x + 2*tank->pos.x) / 2;
	turretY = (tank->turret.y + 2*tank->pos.y) / 2;
		
    Graphics_ApplySurface(turret, (int)turretX - offset.x, (int)turretY - offset.y, 1, tank_rot + tank->turret_angle);
	
    SDL_Texture *cursor = weapon.tankCursor;
	
	if(cursor != NULL)
	{
		int cursorX, cursorY;
        int w, h;
        SDL_QueryTexture(cursor, NULL, NULL, &w, &h);
		//cursorX = (tank->turret.x*4 + 2*tank->pos.x) / 2;
		//cursorY = (tank->turret.y*4 + 2*tank->pos.y) / 2;
		SDL_GetMouseState(&cursorX, &cursorY);
        cursorX -= w / 2;
        cursorY -= h / 2;

        Graphics_ApplySurface(cursor, cursorX, cursorY, 1, 0);
	}
	
	if(tank->firing)
	{
        SDL_Texture *fireAnim = GetFrame(weapon.FireAnimation, tank->fireFrame);
		
		if(fireAnim != NULL)
		{
			float fireX = (tank->turret.x*4 + 2*tank->pos.x) / 2;
			float fireY = (tank->turret.y*4 + 2*tank->pos.y) / 2;
			
            Graphics_ApplySurface(fireAnim, (int)fireX - offset.x, (int)fireY - offset.y, 1, tank_rot + tank->turret_angle);
		}
	}
}
