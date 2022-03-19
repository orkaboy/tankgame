#include "player.h"

#include "SDL2/SDL2_gfxPrimitives.h"

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

void Player::DrawHud(SDL_Renderer* renderer, int x, int y)
{
    /* life tank 1 */
    rectangleRGBA( renderer, x, y, x+154, y+24, 0, 255, 0, 100 );
    boxRGBA( renderer, x+2, y+2,
        x + 2 + ((150.0*mTank->HP())/MAX_HITPOINTS), y+22,
        255 - (255.0*mTank->HP())/MAX_HITPOINTS,
        (255.0*mTank->HP())/MAX_HITPOINTS, 0, 100 );

    /* reload teleport timer*/
    rectangleRGBA( renderer, x, y+25, x+154, y+31, 0, 100, 0, 100);

    if( mTank->TeleportTimer() > 0 )
        boxRGBA( renderer, x+2, y+27, x+2+ 150/30*(30-mTank->TeleportTimer()),y+29,255,0,100,100);
    else if( mTank->TeleportTimer() < 0 )
        boxRGBA( renderer, x+2, y+27, x+152,y+29,255,0,255,100);

    rectangleRGBA( renderer, x, y+32, x+154, y+37, 0, 100, 0, 100);
    /* reload gun timer */
    float delay = Weapon_GetDelay(mTank->GetWeapon());

    auto tslf = mTank->TimeSinceLastFire();
    if( tslf > 0 )
        boxRGBA( renderer, x+2, y+34, x+2+ 150/delay*(delay-tslf),y+35,255,0,100,100);
    else if( tslf < 0 )
        boxRGBA( renderer, x+2, y+34, x+152,y+35,255,0,255,100);
}

}
