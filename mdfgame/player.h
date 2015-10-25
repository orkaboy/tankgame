#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>

#include "tank.h"

typedef struct Player
{
	char *name;
	Tank *tank;
	int points;
	int kills;
	int deaths;
	int id;

	TankColors col;
	/* control settings */
} Player;

/* Init a new player */
Player *Player_Init();
void Player_Free(Player *player);
/* set player name */
void Player_SetName(Player *player, char *name);

/* increse/decrese player points by points */
void Player_ModPoints(Player *player,int points);

/* kills a player */
void Player_Kill(Player *Player);

/* get killed */
void Player_Killed(Player * Player);

#endif
