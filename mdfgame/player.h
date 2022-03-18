#pragma once

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

/* Call when player gets a kill */
void Player_Kill(Player *Player);

/* Call when player gets killed */
void Player_Killed(Player * Player);

void Player_DrawHud(SDL_Renderer* renderer, Player* player, int x, int y);
