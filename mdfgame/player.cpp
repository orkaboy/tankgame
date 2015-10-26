#include "player.h"

#include "SDL2/SDL2_gfxPrimitives.h"

Player *Player_Init()
{
	Player *newPlayer = new Player;
	
	newPlayer->points = 0;
	newPlayer->kills = 0;
	newPlayer->deaths = 0;

	return newPlayer;
}

void Player_Free(Player *player)
{
	delete player;
}

void Player_SetName(Player *player, char *name)
{
	player->name = name;
}

void Player_ModPoints(Player *player,int points)
{
	/* set the points */
	player->points += points;
	/* i do not want negative points when i play..  */
	if(player->points < 0)
		player->points = 0;
}


void Player_Kill(Player* player)
{
	player->kills++;
}

void Player_Killed(Player* player)
{
	player->deaths++;
}

void Player_DrawHud(SDL_Renderer* renderer, Player* player, int x, int y)
{
    /* life tank 1 */
    rectangleRGBA( renderer, x, y, x+154, y+24, 0, 255, 0, 100 );
    boxRGBA( renderer, x+2, y+2, x + 2 + ((150.0*player->tank->hitPoints)/MAX_HITPOINTS), y+22, 255 - (255.0*player->tank->hitPoints)/MAX_HITPOINTS, (255.0*player->tank->hitPoints)/MAX_HITPOINTS, 0, 100 );

    /* reload teleport timer*/
    rectangleRGBA( renderer, x, y+25, x+154, y+31, 0, 100, 0, 100);

    if( player->tank->teleportTimer > 0 )
        boxRGBA( renderer, x+2, y+27, x+2+ 150/30*(30-player->tank->teleportTimer),y+29,255,0,100,100);

    if( player->tank->teleportTimer < 0 )
        boxRGBA( renderer, x+2, y+27, x+152,y+29,255,0,255,100);

    rectangleRGBA( renderer, x, y+32, x+154, y+37, 0, 100, 0, 100);
    /* reload gun timer */
    float delay = Weapon_GetDelay(player->tank->weapon);

    if( player->tank->timeSinceLastFire > 0 )
        boxRGBA( renderer, x+2, y+34, x+2+ 150/delay*(delay-player->tank->timeSinceLastFire),y+35,255,0,100,100);

    if( player->tank->timeSinceLastFire < 0 )
        boxRGBA( renderer, x+2, y+34, x+152,y+35,255,0,255,100);
}
