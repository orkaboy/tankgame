#pragma once

#include <stdio.h>

#include "tank.h"

namespace MDF {

class Player
{
public:
	/* Init a new player */
	Player();
	/* set player name */
	void SetName(const char *name);

	/* increse/decrese player points by points */
	void ModPoints(int points);

	/* Call when player gets a kill */
	void Kill();

	/* Call when player gets killed */
	void Killed();

	void DrawHud(SDL_Renderer* renderer, int x, int y);

	void SetTank(Tank* tank) { mTank = tank; }
	auto GetTank() -> Tank* { return mTank; }

	int& Deaths() { return mDeaths; }

	void SetCol(TankColors col) { mCol = col; }
	TankColors Col() const { return mCol; }

private:
	std::string mName;
	Tank *mTank{};
	int mPoints{};
	int mKills{};
	int mDeaths{};
	int id{};

	TankColors mCol;
	/* control settings */
};

}
