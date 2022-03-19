#pragma once

#include "SDL2/SDL.h"

#include <vector>
#include "button.h"
#include "menuoptions.h"
#include "gamestate.h"
#include "utility.h"

namespace MDF {

class Menu
{
public:
    Menu(SDL_Texture* bg, SDL_Texture* cursor);

    void AddButton(int x, int y, MenuOption ret, std::string text, TTF_Font *font);
    MenuOption CheckButton(vec2 cursor);

    auto Buttons() const -> const std::vector<MDF::Button*>& { return mButtons; }
    auto BG() const -> SDL_Texture* { return mBg; }

private:
    SDL_Texture *mBg;
    SDL_Texture *mCursor;
	std::vector<MDF::Button*> mButtons;
};

namespace Menues {

class Main : public GameState {
public:
    void EnterState() override;
    void ExitState() override;

    std::string HandleEvents() override;
    void Draw() override;
private:
    Menu *mMenu{};
};

}

}
