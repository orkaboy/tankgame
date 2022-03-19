#include "planet.h"

namespace MDF {

Planet::Planet(float x, float y, float radius, float mass)
{
	mPos.x = x;
	mPos.y = y;
	mRot = 0;
	mRotvel = 0;
	mVel.x = 0;
	mVel.y = 0;
	mRadius = radius;
	mMass = mass;
}

}
