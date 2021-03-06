#include "input.h"

#include <map>
#include <cmath>

#include "physics.h"
#include "tank.h"
#include "graphics.h"

namespace MDF {
namespace Input {

static void DefaultMouseHandler ( Player* player, World *world, vec2 pos, Uint8 button );
static void DefaultMoveForwardHandler ( Player* player, World* world );
static void DefaultMoveBackwardHandler ( Player* player, World* world );
static void DefaultFireBeginHandler ( Player* player, World* world );
static void DefaultFireEndHandler ( Player* player, World* world );
static void DefaultTurretUpHandler ( Player* player, World* world );
static void DefaultTurretDownHandler ( Player* player, World* world );
static void DefaultTeleportHandler ( Player* player, World* world );
static void DefaultChangeWepHandler ( Player* player, World* world );

typedef std::map<PlayerAction, ActionHandler> HandlerMap;
typedef std::map<SDL_Scancode, std::pair<Player*, PlayerAction> > BindsMap;

static std::map<KeyStroke, KeyHandler> strokehandlers;

static std::pair<Player *, MouseMoveHandler> mouse_bind;

static HandlerMap handlers;

static BindsMap up_binds;
static BindsMap down_binds;
static BindsMap hold_binds;

static World* world;

void Grab()
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Release()
{
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void Init(World* w)
{
	//Grab();

	world = w;

	handlers[PA_MOVE_FORWARD]   = DefaultMoveForwardHandler;
	handlers[PA_MOVE_BACKWARD]  = DefaultMoveBackwardHandler;
	handlers[PA_FIRE_BEGIN]    	= DefaultFireBeginHandler;
	handlers[PA_FIRE_END]    	= DefaultFireEndHandler;
	handlers[PA_TURRET_UP]      = DefaultTurretUpHandler;
	handlers[PA_TURRET_DOWN]    = DefaultTurretDownHandler;
	handlers[PA_TELEPORT] 	    = DefaultTeleportHandler;
	handlers[PA_CHANGE_WEP]		= DefaultChangeWepHandler;
	mouse_bind.second			= DefaultMouseHandler;

	strokehandlers[KEY_DOWN] = 0;
	strokehandlers[KEY_UP] = 0;
	strokehandlers[KEY_HOLD] = 0;
}

void BindMouseHandler(Player *player)
{
	mouse_bind.first = player;
}

void SetHandler(KeyStroke stroke, KeyHandler handler)
{
	strokehandlers[stroke] = handler;
}

/* Code to transform raw mouse position to the logical rendering scale */
vec2 GetMousePos() {
	auto renderer = Graphics::ReturnRenderer();

	int windowX, windowY;
	SDL_GetMouseState(&windowX, &windowY);

	int logical_w, logical_h;
	SDL_RenderGetLogicalSize(renderer, &logical_w, &logical_h);

	int window_w, window_h;
    SDL_GetWindowSize(Graphics::ReturnScreen(), &window_w, &window_h);

    int window_midpoint_x = window_w>>1;
    int window_midpoint_y = window_h>>1;

	float scale_x, scale_y;
	SDL_RenderGetScale(renderer, &scale_x, &scale_y);
    int renderer_real_w = (float)logical_w*scale_x;
    int renderer_real_h = (float)logical_h*scale_y;

    int renderer_midpoint_x = renderer_real_w>>1;
    int renderer_midpoint_y = renderer_real_h>>1;

    /* Compare with midpoints because a renderer that is logically sized will be centered and have a border
     if the window aspect ratio differs from the renderer's.
    */
    int window_renderer_midpoint_delta_x = window_midpoint_x - renderer_midpoint_x;
    int window_renderer_midpoint_delta_y = window_midpoint_y - renderer_midpoint_y;

    int renderer_real_x = windowX - window_renderer_midpoint_delta_x; 
    int renderer_real_y = windowY - window_renderer_midpoint_delta_y;

	vec2 ret;
    ret.x = (float)renderer_real_x / (float)renderer_real_w * (float)logical_w;
    ret.y = (float)renderer_real_y / (float)renderer_real_h * (float)logical_h;
	return ret;
}

void HandleEvents()
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event))
	{
		//if ( event.button.button == SDL_BUTTON_LEFT )
		//	Grab();

        if(event.type == SDL_KEYDOWN)
		{
			for ( BindsMap::iterator it = down_binds.begin(); it != down_binds.end(); it++ )
			{
				if ( event.key.keysym.sym == it->first )
					handlers[it->second.second](it->second.first, world);
			}
			if ( strokehandlers[KEY_DOWN] )
                strokehandlers[KEY_DOWN](event.key.keysym.scancode, world);
		}	
        else if(event.type == SDL_KEYUP)
		{
			for ( BindsMap::iterator it = up_binds.begin(); it != up_binds.end(); it++ )
			{
				if ( event.key.keysym.sym == it->first )
					handlers[it->second.second](it->second.first, world);
			}
			if ( strokehandlers[KEY_UP] )
                strokehandlers[KEY_UP](event.key.keysym.scancode, world);
		}
	}

	Uint8 button = SDL_GetMouseState(NULL, NULL);
	auto cursor = GetMousePos();

	if ( mouse_bind.second )
		mouse_bind.second(mouse_bind.first, world, cursor, button);
	
    const Uint8* curstate = SDL_GetKeyboardState(NULL);
	
	for ( BindsMap::iterator it = hold_binds.begin(); it != hold_binds.end(); it++ )
	{
		if ( curstate[it->first] )
			handlers[it->second.second](it->second.first, world);
	}
	if ( strokehandlers[KEY_HOLD] )
        strokehandlers[KEY_HOLD](event.key.keysym.scancode, world);
}

void SetActionHandler(PlayerAction action, ActionHandler handler)
{
	handlers[action] = handler;
}

void BindKey(SDL_Scancode key, Player* player, PlayerAction action, KeyStroke keystroke)
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

void DefaultMouseHandler ( Player* player, World* world, vec2 cursor, Uint8 button )
{
	cursor = vec2Add(cursor, world->camera.GetCorner());

	player->GetTank()->SetTurretRotation(cursor);
	
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

void DefaultMoveForwardHandler ( Player* player, World* world )
{
	player->GetTank()->Move(Direction::RIGHT);
}

void DefaultMoveBackwardHandler ( Player* player, World* world )
{
	player->GetTank()->Move(Direction::LEFT);
}

void DefaultFireBeginHandler ( Player* player, World* world )
{
	player->GetTank()->SetFiring(true);
}

void DefaultFireEndHandler ( Player* player, World* world )
{
	player->GetTank()->SetFiring(false);
}

void DefaultTurretUpHandler ( Player* player, World* world )
{
	player->GetTank()->RotateTurret(Direction::RIGHT);
}

void DefaultTurretDownHandler ( Player* player, World* world )
{
	player->GetTank()->RotateTurret(Direction::LEFT);
}

void DefaultTeleportHandler ( Player* player, World* world )
{
	player->GetTank()->Teleport(world);
}

void DefaultChangeWepHandler ( Player* player, World* world )
{
	player->GetTank()->NextWeapon();
}


}
}
