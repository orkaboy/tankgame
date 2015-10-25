#include "input.h"

#include <map>
#include <cmath>
#include <iostream>

#include "physics.h"
#include "tank.h"
#include "graphics.h"

static void DefaultMouseHandler ( Player* player, World &world, Sint16 x, Sint16 y, Uint8 button );
static void DefaultMoveForwardHandler ( Player* player, World& world );
static void DefaultMoveBackwardHandler ( Player* player, World& world );
static void DefaultFireBeginHandler ( Player* player, World& world );
static void DefaultFireEndHandler ( Player* player, World& world );
static void DefaultTurretUpHandler ( Player* player, World& world );
static void DefaultTurretDownHandler ( Player* player, World& world );
static void DefaultTeleportHandler ( Player* player, World& world );
static void DefaultChangeWepHandler ( Player* player, World& world );

typedef std::map<PlayerAction, ActionHandler> HandlerMap;
typedef std::map<SDL_Scancode, std::pair<Player*, PlayerAction> > BindsMap;

static std::map<KeyStroke, KeyHandler> strokehandlers;

static std::pair<Player *, MouseMoveHandler> mouse_bind;
int mx, my;

static HandlerMap handlers;

static BindsMap up_binds;
static BindsMap down_binds;
static BindsMap hold_binds;

static World* world;

void Input_Grab()
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Input_Release()
{
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void Input_Init(World& w)
{
	//Input_Grab();

	mx = my = 0;
	world = &w;

	handlers[PA_MOVE_FORWARD]   	= DefaultMoveForwardHandler;
	handlers[PA_MOVE_BACKWARD]  	= DefaultMoveBackwardHandler;
	handlers[PA_FIRE_BEGIN]    	= DefaultFireBeginHandler;
	handlers[PA_FIRE_END]    	= DefaultFireEndHandler;
	handlers[PA_TURRET_UP]      	= DefaultTurretUpHandler;
	handlers[PA_TURRET_DOWN]    	= DefaultTurretDownHandler;
	handlers[PA_TELEPORT] 	    	= DefaultTeleportHandler;
	handlers[PA_CHANGE_WEP]		= DefaultChangeWepHandler;
	mouse_bind.second		= DefaultMouseHandler;

	strokehandlers[KEY_DOWN] = 0;
	strokehandlers[KEY_UP] = 0;
	strokehandlers[KEY_HOLD] = 0;
}

void Input_BindMouseHandler(Player *player)
{
	mouse_bind.first = player;
}

void Input_SetHandler(KeyStroke stroke, KeyHandler handler)
{
	strokehandlers[stroke] = handler;
}

void Input_HandleEvents()
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event))
	{
		//if ( event.button.button == SDL_BUTTON_LEFT )
		//	Input_Grab();

        if(event.type == SDL_SCANCODE_DOWN)
		{
			for ( BindsMap::iterator it = down_binds.begin(); it != down_binds.end(); it++ )
			{
				if ( event.key.keysym.sym == it->first )
					handlers[it->second.second](it->second.first, *world);
			}
			if ( strokehandlers[KEY_DOWN] )
                strokehandlers[KEY_DOWN](event.key.keysym.scancode, *world);
		}	
        else if(event.type == SDL_SCANCODE_UP)
		{
			for ( BindsMap::iterator it = up_binds.begin(); it != up_binds.end(); it++ )
			{
				if ( event.key.keysym.sym == it->first )
					handlers[it->second.second](it->second.first, *world);
			}
			if ( strokehandlers[KEY_UP] )
                strokehandlers[KEY_UP](event.key.keysym.scancode, *world);
		}
	}
// 	SDL_GetRelativeMouseState(&mx, &my);
	Uint8 button = SDL_GetMouseState(&mx, &my);

	if ( mouse_bind.second )
		mouse_bind.second(mouse_bind.first, *world, mx, my, button);
	
    const Uint8* curstate = SDL_GetKeyboardState(NULL);
	
	for ( BindsMap::iterator it = hold_binds.begin(); it != hold_binds.end(); it++ )
	{
		if ( curstate[it->first] )
			handlers[it->second.second](it->second.first, *world);
	}
	if ( strokehandlers[KEY_HOLD] )
        strokehandlers[KEY_HOLD](event.key.keysym.scancode, *world);
}

void Input_SetActionHandler(PlayerAction action, ActionHandler handler)
{
	handlers[action] = handler;
}

void Input_BindKey(SDL_Scancode key, Player* player, PlayerAction action, KeyStroke keystroke)
{
	switch(keystroke)
	{
		case KEY_UP:
			up_binds[key] = std::pair<Player*, PlayerAction>(player, action);
		break;
		case KEY_DOWN:
			down_binds[key] = std::pair<Player*, PlayerAction>(player, action);
		break;
		case KEY_HOLD:
			hold_binds[key] = std::pair<Player*, PlayerAction>(player, action);
		break;
	}
}

void DefaultMouseHandler ( Player* player, World &world, Sint16 x, Sint16 y, Uint8 button )
{
	vec2 offset = world.camera.GetCorner();
	x += offset.x;
	y += offset.y;
	
	Tank_SetTurretRotation(player->tank, x, y);
	
	static bool l_pressed = false;
	if(button & SDL_BUTTON(1))
	{
		if(!l_pressed)
		{
			DefaultFireBeginHandler(player, world);
			
			l_pressed = true;
		}
	}
	else
	{
		if(l_pressed)
		{
			DefaultFireEndHandler(player, world);
			
			l_pressed = false;
		}
	}
	static bool r_pressed = false;
	if(button & SDL_BUTTON(3))
	{
		if(!r_pressed)
		{
			DefaultChangeWepHandler(player, world);
			
			r_pressed = true;
		}
	}
	else
	{
		if(r_pressed)
		{
			r_pressed = false;
		}
	}
}

void DefaultMoveForwardHandler ( Player* player, World& world )
{
	Tank_Move(player->tank, DIR_RIGHT);
}

void DefaultMoveBackwardHandler ( Player* player, World& world )
{
	Tank_Move(player->tank, DIR_LEFT);
}

void DefaultFireBeginHandler ( Player* player, World& world )
{
	player->tank->firing = true;
}

void DefaultFireEndHandler ( Player* player, World& world )
{
	player->tank->firing = false;
}

void DefaultTurretUpHandler ( Player* player, World& world )
{
	Tank_RotateTurret(player->tank, DIR_RIGHT);
}

void DefaultTurretDownHandler ( Player* player, World& world )
{
	Tank_RotateTurret(player->tank, DIR_LEFT);
}

void DefaultTeleportHandler ( Player* player, World& world )
{
	Tank_Teleport(player->tank,world);
}

void DefaultChangeWepHandler ( Player* player, World& world )
{
	Tank_NextWeapon(player->tank);
}
