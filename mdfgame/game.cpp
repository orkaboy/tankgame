#include "game.h"

#include "graphics.h"
#include "input.h"
#include "physics.h"

namespace MDF {

bool gquit = false;

static void KeyDown ( SDL_Scancode key, World* world )
{
    switch ( key )
    {
    case SDL_SCANCODE_ESCAPE: //TODO Bug in SDL2 as of 2.0.10 in WSL2
    //case SDL_SCANCODE_1:
        gquit = true;
        break;
    default: break;
    }
}

void InitializeInput(World* world, Player* player) {
    Input::Init(world);

    Input::BindKey(SDL_SCANCODE_A, player, Input::PA_MOVE_BACKWARD, Input::KEY_HOLD);
    Input::BindKey(SDL_SCANCODE_D, player, Input::PA_MOVE_FORWARD, Input::KEY_HOLD);
    //Input::BindKey(SDL_SCANCODE_W, player, Input::PA_TURRET_UP, Input::KEY_HOLD);
    //Input::BindKey(SDL_SCANCODE_S, player, Input::PA_TURRET_DOWN, Input::KEY_HOLD);
    Input::BindKey(SDL_SCANCODE_SPACE, player, Input::PA_FIRE_BEGIN, Input::KEY_DOWN);
    Input::BindKey(SDL_SCANCODE_SPACE, player, Input::PA_FIRE_END, Input::KEY_UP);
    Input::BindKey(SDL_SCANCODE_Q, player, Input::PA_TELEPORT, Input::KEY_HOLD);
    Input::BindKey(SDL_SCANCODE_E, player, Input::PA_CHANGE_WEP, Input::KEY_DOWN);

    Input::BindMouseHandler(player);

    /*Input::BindKey(SDL_SCANCODE_LEFT, player2, Input::PA_MOVE_BACKWARD, Input::KEY_HOLD);
    Input::BindKey(SDL_SCANCODE_RIGHT, player2, Input::PA_MOVE_FORWARD, Input::KEY_HOLD);
    Input::BindKey(SDL_SCANCODE_UP, player2, Input::PA_TURRET_UP, Input::KEY_HOLD);
    Input::BindKey(SDL_SCANCODE_DOWN, player2, Input::PA_TURRET_DOWN, Input::KEY_HOLD);
    Input::BindKey(SDL_SCANCODE_RCTRL, player2, Input::PA_FIRE_BEGIN, Input::KEY_DOWN);
    Input::BindKey(SDL_SCANCODE_RCTRL, player2, Input::PA_FIRE_END, Input::KEY_UP);
    Input::BindKey(SDL_SCANCODE_RSHIFT, player2, Input::PA_TELEPORT, Input::KEY_HOLD);
    Input::BindKey(SDL_SCANCODE_RETURN, player2, Input::PA_CHANGE_WEP, Input::KEY_DOWN);*/

    Input::SetHandler(Input::KEY_DOWN, KeyDown);
}

void Game::EnterState() {
    gquit = false;

    mWorld = World::GetWorld("WorldOne");
    auto player = new MDF::Player;

    InitializeInput(mWorld, player);

    //world.players.push_back(player); // DEPRECATED! THERE SHALL BE NO HARDCODING OF INDICES!
    mWorld->player = player;

    player->SetCol(MDF::TANK_NEUTRAL);
    //player2->SetCol(MDF::TANK_YELLOW);

    // TODO random planet
    MDF::Tank::Spawn(mWorld, player, mWorld->planets[0]);
    Graphics::Tank_SetImages(player->GetTank(), player->Col());
    //Tank_Spawn(world, player2, world.planets[4]);
    //Tank_SetImages(player2->GetTank(), player2->Col());
    
    Audio::PlayMusic(0, -1);
}

void Game::ExitState() {
}

std::string Game::HandleEvents() {
    Input::HandleEvents();
    if(gquit) {
        return "MainMenu";
    }
    return "";
}

std::string Game::Update(float dt) {
    mWorld->SpawnAmmo(dt);
    /* Physics */
    Physics::Process(mWorld, dt);
    /* Animtions */
    Effect::Update(mWorld->effects, dt);

    return "";
}

void Game::Draw() {
    Graphics::DrawScene(mWorld);
    //TODO Draw HUD
}

}
