#include "player.h"

#include "SDL2/SDL2_gfxPrimitives.h"
#include <fmt/core.h>
#include "graphics.h"

namespace MDF {

Player::Player()
{
	mPoints = 0;
	mKills = 0;
	mDeaths = 0;
}

void Player::SetName(const char *name)
{
	mName = name;
}

void Player::ModPoints(int points)
{
	/* set the points */
	mPoints += points;
	/* i do not want negative points when i play..  */
	if(mPoints < 0)
		mPoints = 0;
}


void Player::Kill()
{
	mKills++;
}

void Player::Killed()
{
	mDeaths++;
}

void Player::DrawHud(SDL_Renderer* renderer, vec2 pos)
{
    /* life tank */
    rectangleRGBA( renderer, pos.x, pos.y, pos.x+154, pos.y+24, 0, 255, 0, 100 );
    boxRGBA( renderer, pos.x+2, pos.y+2,
        pos.x + 2 + ((150.0*mTank->HP())/MAX_HITPOINTS), pos.y+22,
        255 - (255.0*mTank->HP())/MAX_HITPOINTS,
        (255.0*mTank->HP())/MAX_HITPOINTS, 0, 100 );

    /* reload teleport timer*/
    rectangleRGBA( renderer, pos.x, pos.y+25, pos.x+154, pos.y+31, 0, 100, 0, 100);

    if( mTank->TeleportTimer() > 0 )
        boxRGBA( renderer, pos.x+2, pos.y+27, pos.x+2+ 150/30*(30-mTank->TeleportTimer()),pos.y+29,255,0,100,100);
    else if( mTank->TeleportTimer() < 0 )
        boxRGBA( renderer, pos.x+2, pos.y+27, pos.x+152,pos.y+29,255,0,255,100);

    rectangleRGBA( renderer, pos.x, pos.y+32, pos.x+154, pos.y+37, 0, 100, 0, 100);
    /* reload gun timer */
    float delay = Weapon::GetDelay(mTank->GetWeapon());

    auto tslf = mTank->TimeSinceLastFire();
    if( tslf > 0 )
        boxRGBA( renderer, pos.x+2, pos.y+34, pos.x+2+ 150/delay*(delay-tslf),pos.y+35,255,0,100,100);
    else if( tslf < 0 )
        boxRGBA( renderer, pos.x+2, pos.y+34, pos.x+152,pos.y+35,255,0,255,100);
    
    /* Draw Kill/Deaths */
    std::string scoreStr = fmt::format("K/D: {}/{}", mKills, mDeaths);
    Graphics::DrawString(scoreStr, pos.x + 10, pos.y + 45);
}

}
