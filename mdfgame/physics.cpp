#include "physics.h"

#include <cmath>

#include "audio.h"
#include "graphics.h"

namespace MDF {
namespace Physics {

void UpdatePlanets(World &w, float dt);
void PlanetBounce(Planet *p1, Planet *p2);
void UpdateTanks(World &w, float dt);
void UpdateProjectiles(World &w, float dt);


void PlanetBounce(Planet *p1, Planet *p2)
{
	// First, find the normalized vector n from the center of
	// circle1 to the center of circle2
	vec2 n = vec2Sub(p2->Pos(), p1->Pos());
	n = vec2Normalize(n);

	// Find the length of the component of each of the movement
	// vectors along n.
	// a1 = v1 . n
	// a2 = v2 . n
	float a1 = vec2DotProd(p1->Vel(), n);
	float a2 = vec2DotProd(p2->Vel(), n);

	// Using the optimized version,
	// optimizedP =  2(a1 - a2)
	//              -----------
	//                m1 + m2
	float optimizedP = (2.0 * (a1 - a2)) / (p1->Mass() + p2->Mass());

	// Calculate v1', the new movement vector of circle1
	// v1' = v1 - optimizedP * m2 * n
	p1->Vel() = vec2Sub(p1->Vel(), vec2Multiply(n, optimizedP * p2->Mass())); 

	// Calculate v2', the new movement vector of circle2
	// v2' = v2 + optimizedP * m1 * n
	p2->Vel() = vec2Add(p2->Vel(), vec2Multiply(n, optimizedP * p1->Mass()));
}

void UpdatePlanets(World &w, float dt)
{
	for(auto planet : w.planets)
	{
		vec2 oldPos = planet->Pos();
		
		planet->Rot() += planet->RotVel() * dt;
		while(planet->Rot() > 2 * M_PI) planet->Rot() -= 2*M_PI;
		while(planet->Rot() < 0) planet->Rot() += 2*M_PI;

		planet->Pos().x += planet->Vel().x * dt;
		planet->Pos().y += planet->Vel().y * dt;
		
		vec2 moveVec = vec2Sub(planet->Pos(), oldPos);
		
		planet->Vel().x *= 0.99;
		planet->Vel().y *= 0.99;
		
		planet->RotVel() *= 0.97;
		
		if(planet->Pos().x - planet->Radius() < 0)
		{
			planet->Vel().x = -planet->Vel().x;
			planet->Pos().x = planet->Radius();
		}
		if(planet->Pos().x + planet->Radius() > w.size.width)
		{
			planet->Vel().x = -planet->Vel().x;
			planet->Pos().x = w.size.width - planet->Radius();
		}
		if(planet->Pos().y - planet->Radius() < 0) 
		{
			planet->Vel().y = -planet->Vel().y;
			planet->Pos().y = planet->Radius();
		}
		if(planet->Pos().y + planet->Radius() > w.size.height)
		{
			planet->Vel().y = -planet->Vel().y;
			planet->Pos().y = w.size.height - planet->Radius();
		}
		
		/* Check for collisions */
		for(auto planet2 : w.planets)
		{
			if(planet == planet2) continue;

			float dist = vec2Length(vec2Sub(planet2->Pos(), planet->Pos()));
			float r = planet2->Radius() + planet->Radius();
			float r2 = r * r;
			dist -= r;
			if(vec2Length(moveVec) < dist) continue;

			// Normalize the movevec
			vec2 n = vec2Normalize(planet->Vel());

			// Find C, the vector from the center of the moving
			// circle A to the center of B
			vec2 C = vec2Sub(planet2->Pos(), oldPos);

			// D = N . C = ||C|| * cos(angle between N and C)
			float D = vec2DotProd(n, C);

			// Another early escape: Make sure that A is moving
			// towards B! If the dot product between the movevec and
			// B.center - A.center is less that or equal to 0,
			// A isn't isn't moving towards B
			if(D <= 0) continue;

			// Find the length of the vector C
			float lengthC2 = vec2Length2(C);

			float F = lengthC2 - D*D;

			// Escape test: if the closest that A will get to B
			// is more than the sum of their radii, there's no
			// way they are going collide
			if(F >= r2) continue;

			// We now have F and sumRadii, two sides of a right triangle.
			// Use these to find the third side, sqrt(T)
			float T = r2 - F;

			// If there is no such right triangle with sides length of
			// sumRadii and sqrt(f), T will probably be less than 0.
			// Better to check now than perform a square root of a
			// negative number.
			if(T < 0) continue;

			// Therefore the distance the circle has to travel along
			// movevec is D - sqrt(T)
			float distance = D - sqrt(T);

			// Get the magnitude of the movement vector
			float mag = vec2Length(moveVec);

			// Finally, make sure that the distance A has to move
			// to touch B is not greater than the magnitude of the
			// movement vector.
			if(mag < distance) continue;

			// Set the length of the movevec so that the circles will just
			// touch
			moveVec = vec2Normalize(moveVec);
			moveVec = vec2Multiply(moveVec, distance);

			planet->Pos() = vec2Add(oldPos, moveVec);
			
			PlanetBounce(planet, planet2);
		}
	}
}

void UpdateProjectiles(World &w, float dt)
{
	/* For all bullets */
	for(unsigned int i = 0; i < w.projectiles.size(); i++)
	{
		Projectile* proj = w.projectiles[i];
		Tank* target = NULL;
		Planet* tPlanet = NULL;
		
		float accx = 0, accy = 0;
		bool removal = false;
		/* Gather acceleration from planets */
		
		proj->Update( w, removal, dt );

		for(auto planet : w.planets)
		{
			float dx, dy;
			float r, r2;
			
			dx = planet->Pos().x - proj->pos.x;
			dy = planet->Pos().y - proj->pos.y;
			
			r2 = dx*dx + dy*dy;
			r = sqrt(r2);
			
			float acc = GRAVITY * planet->Mass();
			acc /= r2;
			
			accx += acc * (dx / r);
			accy += acc * (dy / r);
		}
		
		proj->vel.x += accx * dt;
		proj->vel.y += accy * dt;
		
		proj->pos.x += proj->vel.x * dt;
		proj->pos.y += proj->vel.y * dt;
		
		/* Collision with planet */
		for(auto planet : w.planets)
		{
			float distance2 = vec2Length2(vec2Sub(proj->pos, planet->Pos()));
			float r2 = proj->radius + planet->Radius();
			r2 *= r2;

			if(r2 >= distance2)
			{
				tPlanet = planet;
				removal = true;
				break;
			}
		}
		
		/* Calculate collisions for all tanks */
		for(auto tank : w.tanks)
		{
			if(tank->Dying()) continue;
			
			float distance2 = vec2Length2(vec2Sub(proj->pos, tank->Pos()));
			float r2 = proj->radius + tank->BoundingRadius();
			r2 *= r2;
			
			if(r2 >= distance2)
			{
				removal = true;
				target = tank;
			}
		}
		
		/* Check if projectile left screen */
		if(proj->pos.x < 0) 
			proj->pos.x += w.size.width;
		if(proj->pos.x > w.size.width) 
			proj->pos.x -= w.size.width;
		if(proj->pos.y < 0) 
			proj->pos.y += w.size.height;
		if(proj->pos.y > w.size.height) 
			proj->pos.y -= w.size.height;
		
		/* Remove projectile if collided with something */
		if (removal) proj->Hit(w, target, tPlanet, removal, dt);
		if (removal)
		{
			delete proj;
			w.projectiles.erase(w.projectiles.begin() + i);
			i--;
		}
	}
}

void Process(World &w, float dt)
{
	UpdatePlanets(w, dt);
	
	UpdateTanks(w, dt);

	UpdateProjectiles(w, dt);
}

void UpdateTanks(World &w, float dt)
{
	/* For all tanks */
	for(unsigned int i = 0; i < w.tanks.size(); i++)
	{
		Tank *tank = w.tanks[i];
		auto planet = tank->GetPlanet();
		auto player = tank->GetPlayer();
		
		tank->TimeSinceLastFire() -= dt;
		tank->TeleportTimer() -= dt;
		
		tank->Pos().x = planet->Pos().x + (planet->Radius() * cos(tank->AngularPos() + planet->Rot()));
		tank->Pos().y = planet->Pos().y + (planet->Radius()  * -sin(tank->AngularPos() + planet->Rot()));
		
		tank->Turret().x = 20.0f * cos(tank->TurretAngle() + tank->AngularPos() + planet->Rot());
		tank->Turret().y = 20.0f * -sin(tank->TurretAngle() + tank->AngularPos() + planet->Rot());
		
		/* Check if tank is crushed! */
		for(auto planet : w.planets)
		{
			if(planet == tank->GetPlanet()) continue;
			
			float dist = vec2Length(vec2Sub(planet->Pos(), tank->Pos()));
			if(dist > planet->Radius() + tank->BoundingRadius()) continue;
			
			if(!tank->Dying())
				w.effects.push_back(new Effect(EffectType::CRUSHED, tank->Pos()));
			
			tank->Destroy();
		}

		if(tank->Firing())
			Tank::FireWeapon(player, w, dt);
		
		if(tank->Firing())
			Tank::FireWeapon(player, w, dt);
		
		if(tank->Dying())
		{
			tank->DyingTimer() -= dt;
			if(tank->DyingTimer() < 0)
			{
				delete tank;
				w.tanks.erase(w.tanks.begin() + i);
				
				MDF::Tank::Spawn(w, player, w.planets[0]);

				tank = player->GetTank();
				Graphics::Tank_SetImages(tank, player->Col());
				tank->Teleport(w);
				i--;
			}
		}
	}
}


} // namespace Physics
} // namespace MDF
