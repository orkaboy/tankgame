#ifndef INPUT_H
#define INPUT_H

#ifdef _WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "player.h"
#include "world.h"

enum KeyStroke
{
	KEY_UP,
	KEY_DOWN,
	KEY_HOLD
};

enum PlayerAction
{
	PA_MOVE_FORWARD,
	PA_MOVE_BACKWARD,
	PA_FIRE_BEGIN,
	PA_FIRE_END,
	PA_TURRET_UP,
	PA_TURRET_DOWN,
	PA_TELEPORT,
	PA_CHANGE_WEP
};

typedef void(*ActionHandler)(Player* player, World& world);
typedef void(*KeyHandler)(SDLKey key, World& world);
typedef void(*MouseMoveHandler)(Player* player, World &world, Sint16 x, Sint16 y, Uint8 button);

void Input_Init(World& w);
void Input_HandleEvents();
void Input_SetActionHandler(PlayerAction action, ActionHandler handler);
void Input_BindKey(SDLKey key, Player* player, PlayerAction ation, KeyStroke keystroke);
void Input_SetHandler(KeyStroke stroke, KeyHandler handler);
void Input_BindMouseHandler(Player* player);
void Input_Grab();
void Input_Release();
bool Input_HasFocus();
bool Input_KeyIsDown(SDLKey key);

#endif
