#include "player.h"

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
