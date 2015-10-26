#include "projectile.h"
#include "tank.h"
#include "player.h"
#include "planet.h"
#include "physics.h"
#include "world.h"
#include "net.h"
#include <cmath>

#include "graphics.h"

#include "SDL2/SDL2_gfxPrimitives.h"


void Projectile_Draw(vector<Projectile*> &proj, SDL_Renderer *screen, vec2 offset)
{
	for (unsigned int l = 0; l < proj.size(); l++)
	{
		if (proj[l]->animation) {
			
			proj[l]->surface = GetFrame(proj[l]->animation, proj[l]->animFrame);
			
		}
		if (proj[l]->surface) {
			vec2 direction;
			vec2 unit(1,0);
			direction = proj[l]->vel;
			direction = vec2Normalize(direction);
			direction.y *= -1;
			
			float theta = acos(vec2DotProd(direction, unit));
			
			float alpha;
			if(direction.y > 0)
				alpha = theta;
			else
				alpha = -theta;
			
            Graphics_ApplySurface(proj[l]->surface, proj[l]->pos.x - offset.x,
						proj[l]->pos.y - offset.y, 1.0, alpha);
		}
		else
			filledCircleRGBA(screen, proj[l]->pos.x - offset.x, proj[l]->pos.y - offset.y, 2, 255, 0, 0, 255);
	}
}

/* creats a projectile at position for players tank with vinkel and kraft */
Projectile* Projectile_Create(Player* player, float kraft, vec2 pos, float vinkel, ProjectileType type)
{
	Projectile* p = new Projectile;

	p->player = player;
	p->pos.x = pos.x;
	p->pos.y = pos.y;
	p->vel.x = cos(vinkel) * kraft;
	p->vel.y = -sin(vinkel) * kraft;
	/* TEMP */
	p->radius = 1;
	p->animation = NULL;
	p->surface = NULL;
	p->type = type;
	p->damage = 20.0;

	switch(p->type)
	{
		case CLUSTER:
		{
			p->animation = GetAnimation("ClusterAnim");
			p->LifeTime = 3.0;
		}
		break;
		case FLAMER:
		{
			p->animation = GetAnimation("FlamerFire");
			p->LifeTime = 2.1;
		}
		break;
		case ROCKET:
		{
			p->animation = GetAnimation("RocketAnim");
		}
		break;
		case AMMO_TURRET:
		{
			p->surface = getImage("TurretAmmo");
		}
		break;
		case AMMO_FLAMER:
		{
			p->surface = getImage("FlamerAmmo");
		}
		break;
		case AMMO_CLUSTER:
		{
			p->surface = getImage("ClusterAmmo");
		}
		break;
		default:
		break;
	}
	
	if (p->animation) {
		p->animTimer = 0;
		p->animFrame = 0;
	}

	return p;
}

void Projectile_Hit(Projectile* projectile, World &world, Tank* target, Planet* planet, bool &removal, float dt )
{
	if (target) {
		switch(projectile->type)
		{
			case TURRET:
			case FLAMER:
			case ROCKET:
			case CLUSTER:
			case CLUSTER_BIT:
			case NUKE:
			{
				planet = target->planet;
				/* Fix this */
				target->hitPoints -= projectile->damage;
				if(target->hitPoints <= 0) {
					Tank_Destroy(target);
					if(projectile->player->tank == target)
						Player_ModPoints(projectile->player,-1);
					else
						Player_ModPoints(projectile->player,1);
				}
				else
					Audio_PlayExplosionSmall();
			}
			break;
			case AMMO_TURRET:
			{
				target->ammo[0] += 1;
				planet = NULL;
			}
			break;
			case AMMO_FLAMER:
			{
				target->ammo[1] += 1;
				planet = NULL;
			}
			break;
			case AMMO_CLUSTER:
			{
				target->ammo[2] += 1;
				planet = NULL;
			}
            break;
            case AMMO_ROCKET:
            {
                target->ammo[3] += 1;
                planet = NULL;
            }
            break;
            case AMMO_NUKE:
            {
                target->ammo[4] += 1;
                planet = NULL;
            }
            break;
		}
	}
	
	switch(projectile->type)
	{
		case CLUSTER:
		{
			Projectile* newproj;
			for (int i = 0; i < 12; i++) {
				newproj = Projectile_Create(projectile->player, 150.f,
					  projectile->pos, float(i * M_PI) / 6.0, CLUSTER_BIT);
				newproj->pos.x -= projectile->vel.x*dt;
				newproj->pos.y -= projectile->vel.y*dt;
				world.projectiles.push_back(newproj);
			}
			if (planet) {
				vec2 velMod = projectile->vel;
				velMod.x = velMod.x/planet->mass*10000.0;
				velMod.y = velMod.y/planet->mass*10000.0;
				planet->vel = vec2Add( planet->vel, velMod );
			}
		}
		break;
		case ROCKET:
		{
			if (planet) {
				vec2 velMod = projectile->vel;
				velMod.x = velMod.x/planet->mass*10000.0;
				velMod.y = velMod.y/planet->mass*10000.0;
				planet->vel = vec2Add( planet->vel, velMod );
			}
		}
		break;
		case AMMO_TURRET:
		case AMMO_FLAMER:
        case AMMO_CLUSTER:
        case AMMO_ROCKET:
        case AMMO_NUKE:
		{
			if (planet) {
				vec2 distance = vec2Sub( projectile->pos, planet->pos );
				distance = vec2Normalize(distance);
				
				distance.x *= planet->radius;
				distance.y *= planet->radius;
				
				projectile->vel = distance;
				
				projectile->pos = vec2Add( planet->pos, distance );
				
				removal = false;
			}
		}
		break;
		default:
		break;
	}
}
void Projectile_Update(Projectile* projectile, World &world, bool &removal, float dt )
{
	if (projectile->animation) UpdateAnimation(projectile->animation, projectile->animFrame,
						projectile->animTimer, dt);
	switch(projectile->type)
	{
		case CLUSTER:
		case FLAMER:
		{
			if (projectile->LifeTime <= 0.0)
				removal = true;
			else projectile->LifeTime -= dt;
		}
		break;
		case ROCKET:
		{
			int cursorX, cursorY;
			SDL_GetMouseState(&cursorX, &cursorY);
			
			vec2 offset = world.camera.GetCorner();
			offset.x += cursorX;
			offset.y += cursorY;
			
			offset = vec2Sub( offset, projectile->pos );
			offset = vec2Normalize(offset);
			
			offset.x *=10.0;
			offset.y *=10.0;
			
			projectile->vel = vec2Add( offset, projectile->vel );

			if (vec2Length(projectile->vel) > 300.0) {
				projectile->vel = vec2Normalize(projectile->vel);
				projectile->vel = vec2Multiply(projectile->vel, 300.0);
			}
		}
		break;
		case AMMO_TURRET:
		case AMMO_FLAMER:
		case AMMO_CLUSTER:
		case AMMO_ROCKET:
		{
			for(unsigned int j = 0; j < world.tanks.size(); j++) {
				Tank* tank = world.tanks[j];
				
				float dx, dy;
				float r, r2;
				
				dx = tank->pos.x - projectile->pos.x;
				dy = tank->pos.y - projectile->pos.y;
				
				r2 = dx*dx + dy*dy;
				r = sqrt(r2);
				
				float acc = GRAVITY * 200000;
				acc /= r2;
				
				float accy;
				float accx;
				
				accx = acc * (dx / r);
				accy = acc * (dy / r);
				
				projectile->vel.x += accx * dt;
				projectile->vel.y += accy * dt;
			}
		}
		break;
		default:
		break;
	}
}

/* removes projectile from the game */
void Projectile_Destroy(Projectile* projectile)
{
	Net_UnRegisterEntity(projectile);
  	delete projectile;
}
