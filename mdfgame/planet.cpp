#include "planet.h"

namespace MDF {

Planet::Planet(vec2 pos, float radius, float mass)
{
	mPos = pos;
	mRot = 0;
	mRotvel = 0;
	mRadius = radius;
	mMass = mass;
}

}
