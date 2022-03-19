#pragma once

#include "utility.h"

#include "SDL2/SDL.h"

namespace MDF {

class Planet
{
public:
	Planet(vec2 pos, float radius, float mass);

	auto Pos() const -> vec2 { return mPos; }
	auto Pos() -> vec2& { return mPos; }
	auto Radius() const -> float { return mRadius; }
	auto Rot() const -> float { return mRot; }
	auto Rot() -> float& { return mRot; }
	auto RotVel() const -> float { return mRotvel; }
	auto RotVel() -> float& { return mRotvel; }
	auto Vel() const -> vec2 { return mVel; }
	auto Vel() -> vec2& { return mVel; }
	auto Mass() const -> float { return mMass; }
	auto SetImage(SDL_Texture* image) -> SDL_Texture* { mImage = image; return mImage; }
	auto Image() const -> SDL_Texture* { return mImage; }
private:
	vec2 mPos;
	float mRadius;
	float mRot;
	float mRotvel;
	vec2 mVel;
	float mMass;
    SDL_Texture *mImage;
};

}
