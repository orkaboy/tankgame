#pragma once

#include "SDL.h"

#include "player.h"
#include "world.h"

namespace MDF {
namespace Input {

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

typedef void(*ActionHandler)(MDF::Player* player, World* world);
typedef void(*KeyHandler)(SDL_Scancode key, World* world);
typedef void(*MouseMoveHandler)(MDF::Player* player, World* world, vec2 cursor, Uint8 button);

void Init(World* w);
void HandleEvents();
void SetActionHandler(PlayerAction action, ActionHandler handler);
void BindKey(SDL_Scancode key, MDF::Player* player, PlayerAction ation, KeyStroke keystroke);
void SetHandler(KeyStroke stroke, KeyHandler handler);
void BindMouseHandler(MDF::Player* player);
void Grab();
void Release();

vec2 GetMousePos();

}
}
