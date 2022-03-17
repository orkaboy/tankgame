#include "physics.h"

#include <cmath>

#include "audio.h"
#include "graphics.h"

void Physics_PlanetBounce(Planet *p1, Planet *p2)
{
	// First, find the normalized vector n from the center of
	// circle1 to the center of circle2
	vec2 n = vec2Sub(p2->pos, p1->pos);
	n = vec2Normalize(n);

	// Find the length of the component of each of the movement
	// vectors along n.
	// a1 = v1 . n
	// a2 = v2 . n
	float a1 = vec2DotProd(p1->vel, n);
	float a2 = vec2DotProd(p2->vel, n);

	// Using the optimized version,
	// optimizedP =  2(a1 - a2)
	//              -----------
	//                m1 + m2
	float optimizedP = (2.0 * (a1 - a2)) / (p1->mass + p2->mass);

	// Calculate v1', the new movement vector of circle1
	// v1' = v1 - optimizedP * m2 * n
	p1->vel = vec2Sub(p1->vel, vec2Multiply(n, optimizedP * p2->mass)); 

	// Calculate v2', the new movement vector of circle2
	// v2' = v2 + optimizedP * m1 * n
	p2->vel = vec2Add(p2->vel, vec2Multiply(n, optimizedP * p1->mass));
}

void Physics_UpdatePlanets(World &w, float dt)
{
	for(unsigned int i = 0; i < w.planets.size(); i++)
	{
		Planet *planet = w.planets[i];
		
		vec2 oldPos = planet->pos;
		
		planet->rot += planet->rotvel * dt;
		while(planet->rot > 2 * M_PI) planet->rot -= 2*M_PI;
		while(planet->rot < 0) planet->rot += 2*M_PI;

		planet->pos.x += planet->vel.x * dt;
		planet->pos.y += planet->vel.y * dt;
		
		vec2 moveVec = vec2Sub(planet->pos, oldPos);
		
		planet->vel.x *= 0.99;
		planet->vel.y *= 0.99;
		
		planet->rotvel *= 0.97;
		
		if(planet->pos.x - planet->radius < 0)
		{
			planet->vel.x = -planet->vel.x;
			planet->pos.x = planet->radius;
		}
		if(planet->pos.x + planet->radius > w.size.width)
		{
			planet->vel.x = -planet->vel.x;
			planet->pos.x = w.size.width - planet->radius;
		}
		if(planet->pos.y - planet->radius < 0) 
		{
			planet->vel.y = -planet->vel.y;
			planet->pos.y = planet->radius;
		}
		if(planet->pos.y + planet->radius > w.size.height)
		{
			planet->vel.y = -planet->vel.y;
			planet->pos.y = w.size.height - planet->radius;
		}
		
		/* Check for collisions */
		for(unsigned int j = 0; j < w.planets.size(); j++)
		{
			if(i == j) continue;
			
			Planet* planet2 = w.planets[j];
			
			float dist = vec2Length(vec2Sub(planet2->pos, planet->pos));
			float r = planet2->radius + planet->radius;
			float r2 = r * r;
			dist -= r;
			if(vec2Length(moveVec) < dist) continue;

			// Normalize the movevec
			vec2 n = vec2Normalize(planet->vel);

			// Find C, the vector from the center of the moving
			// circle A to the center of B
			vec2 C = vec2Sub(planet2->pos, oldPos);

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

			planet->pos = vec2Add(oldPos, moveVec);
			
			Physics_PlanetBounce(planet, planet2);
		}
	}
}

void Physics_UpdateProjectiles(World &w, float dt)
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
		
		Projectile_Update( proj, w, removal, dt );

		for(unsigned int j = 0; j < w.planets.size(); j++)
		{
			Planet *planet = w.planets[j];
			
			float dx, dy;
			float r, r2;
			
			dx = planet->pos.x - proj->pos.x;
			dy = planet->pos.y - proj->pos.y;
			
			r2 = dx*dx + dy*dy;
			r = sqrt(r2);
			
			float acc = GRAVITY * planet->mass;
			acc /= r2;
			
			accx += acc * (dx / r);
			accy += acc * (dy / r);
		}
		
		proj->vel.x += accx * dt;
		proj->vel.y += accy * dt;
		
		proj->pos.x += proj->vel.x * dt;
		proj->pos.y += proj->vel.y * dt;
		
		/* Collision with planet */
		for(unsigned int j = 0; j < w.planets.size(); j++)
		{
			Planet *planet = w.planets[j];
			
			float distance2 = vec2Length2(vec2Sub(proj->pos, planet->pos));
			float r2 = proj->radius + planet->radius;
			r2 *= r2;

			if(r2 >= distance2)
			{
				tPlanet = planet;
				removal = true;
				break;
			}
		}
		
		/* Calculate collisions for all tanks */
		for(unsigned int j = 0; j < w.tanks.size(); j++)
		{
			Tank *tank = w.tanks[j];
			if(tank->dying) continue;
			
			float distance2 = vec2Length2(vec2Sub(proj->pos, tank->pos));
			float r2 = proj->radius + tank->bounding_radius;
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
		if (removal) Projectile_Hit(proj, w, target, tPlanet, removal, dt);
		if (removal)
		{
			Projectile_Destroy(proj);
			w.projectiles.erase(w.projectiles.begin() + i);
			i--;
		}
	}
}

void Physics(World &w, float dt)
{
	Physics_UpdatePlanets(w, dt);
	
	Physics_UpdateTanks(w, dt);

	Physics_UpdateProjectiles(w, dt);
}

void Physics_UpdateTanks(World &w, float dt)
{
	/* For all tanks */
	for(unsigned int i = 0; i < w.tanks.size(); i++)
	{
		Tank *tank = w.tanks[i];
		
		tank->timeSinceLastFire -= dt;
		tank->teleportTimer -= dt;
		
		tank->pos.x = tank->planet->pos.x + (tank->planet->radius * cos(tank->angular_position + tank->planet->rot));
		tank->pos.y = tank->planet->pos.y + (tank->planet->radius  * -sin(tank->angular_position + tank->planet->rot));
			
		tank->turret.x = 20.0f * cos(tank->turret_angle + tank->angular_position + tank->planet->rot);
		tank->turret.y = 20.0f * -sin(tank->turret_angle + tank->angular_position + tank->planet->rot);
		
		/* Check if tank is crushed! */
		for(unsigned int j = 0; j < w.planets.size(); j++)
		{
			Planet *planet = w.planets[j];
			if(planet == tank->planet) continue;
			
			float dist = vec2Length(vec2Sub(planet->pos, tank->pos));
			if(dist > planet->radius + tank->bounding_radius) continue;
			
			if(!tank->dying)
				w.effects.push_back(Effect_Create(FX_CRUSHED, tank->pos));
			
			Tank_Destroy(tank);
		}

		if(tank->firing)
			Tank_FireWeapon(tank->player, w, dt);
		
		if(tank->firing)
			Tank_FireWeapon(tank->player, w, dt);
		
		if(tank->dying)
		{
			tank->dyingTimer -= dt;
			if(tank->dyingTimer < 0)
			{
				Player *player = tank->player;
				
				Tank_Free(tank);
				w.tanks.erase(w.tanks.begin() + i);
				
				Tank_Spawn(w, player, w.planets[0]);
				Tank_SetImages(player->tank, player->col);
				Tank_Teleport(player->tank, w);
				i--;
				tank = player->tank;
			}
		}
	}
}
