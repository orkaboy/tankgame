#include "projectile.h"
#include "tank.h"
#include "player.h"
#include "planet.h"
#include "physics.h"
#include "world.h"
#include "input.h"
#include <cmath>

#include "graphics.h"

#include "SDL2/SDL2_gfxPrimitives.h"

namespace MDF {

void Projectile::Draw(std::vector<Projectile*> &proj, SDL_Renderer *screen, vec2 offset)
{
	for (auto p : proj)
	{
		if (p->animation) {
			p->surface = p->animation->GetFrame(p->animFrame);
		}
		if (p->surface) {
			vec2 direction;
			vec2 unit(1,0);
			direction = p->vel;
			direction = vec2Normalize(direction);
			direction.y *= -1;
			
			float theta = acos(vec2DotProd(direction, unit));
			
			float alpha;
			if(direction.y > 0)
				alpha = theta;
			else
				alpha = -theta;
			
            Graphics::ApplySurface(p->surface, p->pos.x - offset.x,
						p->pos.y - offset.y, 1.0, alpha);
		}
		else
			filledCircleRGBA(screen, p->pos.x - offset.x, p->pos.y - offset.y, 2, 255, 0, 0, 255);
	}
}

/* creats a projectile at position for players tank with vinkel and kraft */
Projectile::Projectile(MDF::Player* player_, float kraft, vec2 pos_, float vinkel, ProjectileType type_)
{
	player = player_;
	pos = pos_;
	vel.x = cos(vinkel) * kraft;
	vel.y = -sin(vinkel) * kraft;
	/* TEMP */
	radius = 1;
	animation = NULL;
	surface = NULL;
	type = type_;
	damage = 20.0;

	switch(type)
	{
		case CLUSTER:
		{
			animation = MDF::Animation::GetAnimation("ClusterAnim");
			LifeTime = 3.0;
		}
		break;
		case FLAMER:
		{
			animation = MDF::Animation::GetAnimation("FlamerFire");
			LifeTime = 2.1;
		}
		break;
		case ROCKET:
		{
			animation = MDF::Animation::GetAnimation("RocketAnim");
		}
		break;
		case AMMO_TURRET:
		{
			surface = Graphics::getImage("TurretAmmo");
		}
		break;
		case AMMO_FLAMER:
		{
			surface = Graphics::getImage("FlamerAmmo");
		}
		break;
		case AMMO_CLUSTER:
		{
			surface = Graphics::getImage("ClusterAmmo");
		}
		break;
		default:
		break;
	}
	
	if (animation) {
		animTimer = 0;
		animFrame = 0;
	}
}

void Projectile::Hit(World* world, MDF::Tank* target, MDF::Planet* planet, bool &removal, float dt )
{
	if (target) {
		switch(type)
		{
			case TURRET:
			case FLAMER:
			case ROCKET:
			case CLUSTER:
			case CLUSTER_BIT:
			case NUKE:
			{
				planet = target->GetPlanet();
				/* Fix this */
				target->HP() -= damage;
				if(target->HP() <= 0) {
					target->Destroy();
					if(player->GetTank() == target)
						player->ModPoints(-1);
					else
						player->ModPoints(1);
				}
				else
					MDF::Audio::PlayExplosionSmall();
			}
			break;
			case AMMO_TURRET:
			{
				target->Ammo(0) += 1;
				planet = NULL;
			}
			break;
			case AMMO_FLAMER:
			{
				target->Ammo(1) += 1;
				planet = NULL;
			}
			break;
			case AMMO_CLUSTER:
			{
				target->Ammo(2) += 1;
				planet = NULL;
			}
            break;
            case AMMO_ROCKET:
            {
                target->Ammo(3) += 1;
                planet = NULL;
            }
            break;
            case AMMO_NUKE:
            {
                target->Ammo(4) += 1;
                planet = NULL;
            }
            break;
		}
	}
	
	switch(type)
	{
		case CLUSTER:
		{
			for (int i = 0; i < CLUSTER_COUNT; i++) {
				auto newproj = new Projectile(player, 150.f,
					  pos, float(i * M_PI) / 6.0, CLUSTER_BIT);
				newproj->pos.x -= vel.x*dt;
				newproj->pos.y -= vel.y*dt;
				world->projectiles.push_back(newproj);
			}
			if (planet) {
				vec2 velMod = vel;
				velMod.x = velMod.x/planet->Mass()*10000.0;
				velMod.y = velMod.y/planet->Mass()*10000.0;
				planet->Vel() = vec2Add( planet->Vel(), velMod );
			}
		}
		break;
		case ROCKET:
		{
			if (planet) {
				vec2 velMod = vel;
				velMod.x = velMod.x/planet->Mass()*10000.0;
				velMod.y = velMod.y/planet->Mass()*10000.0;
				planet->Vel() = vec2Add( planet->Vel(), velMod );
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
				vec2 distance = vec2Sub( pos, planet->Pos() );
				distance = vec2Normalize(distance);
				
				distance.x *= planet->Radius();
				distance.y *= planet->Radius();
				
				vel = distance;
				
				pos = vec2Add( planet->Pos(), distance );
				
				removal = false;
			}
		}
		break;
		default:
		break;
	}
}
void Projectile::Update(World* world, bool &removal, float dt )
{
	if (animation) {
		animation->UpdateAnimation(animFrame, animTimer, dt);
	}

	switch(type)
	{
		case CLUSTER:
		case FLAMER:
		{
			if (LifeTime <= 0.0)
				removal = true;
			else LifeTime -= dt;
		}
		break;
		case ROCKET:
		{
			vec2 cursor = MDF::Input::GetMousePos();
			
			vec2 offset = world->camera.GetCorner();
			offset.x += cursor.x;
			offset.y += cursor.y;
			
			offset = vec2Sub( offset, pos );
			offset = vec2Normalize(offset);
			
			offset.x *=10.0;
			offset.y *=10.0;
			
			vel = vec2Add( offset, vel );

			if (vec2Length(vel) > 300.0) {
				vel = vec2Normalize(vel);
				vel = vec2Multiply(vel, 300.0);
			}
		}
		break;
		case AMMO_TURRET:
		case AMMO_FLAMER:
		case AMMO_CLUSTER:
		case AMMO_ROCKET:
		{
			for(auto tank : world->tanks) {
				float dx, dy;
				float r, r2;
				
				dx = tank->Pos().x - pos.x;
				dy = tank->Pos().y - pos.y;
				
				r2 = dx*dx + dy*dy;
				r = sqrt(r2);
				
				float acc = GRAVITY * 200000;
				acc /= r2;
				
				float accy;
				float accx;
				
				accx = acc * (dx / r);
				accy = acc * (dy / r);
				
				vel.x += accx * dt;
				vel.y += accy * dt;
			}
		}
		break;
		default:
		break;
	}
}

}
