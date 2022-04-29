#include "menu.h"
#include "graphics.h"

#include "input.h"
#include "gamestate.h"
#include <fmt/core.h>

namespace MDF {

Menu::Menu(SDL_Texture* bg, SDL_Texture* cursor)
{
	mBg = bg;
	mCursor = cursor;
}

void Menu::AddButton(int x, int y, MenuOption ret, std::string text, TTF_Font *font)
{
    mButtons.push_back(new Button(x, y, ret, text, font));
}

void Menu::Draw() {
	SDL_Point size;
    SDL_QueryTexture(mBg, NULL, NULL, &size.x, &size.y);
    Graphics::ApplySurface(mBg, size.x / 2, size.y / 2);

    auto renderer = Graphics::ReturnRenderer();
    for(auto button : mButtons)
        button->Draw(renderer);
}

MenuOption Menu::CheckButton(vec2 cursor)
{
	for(auto button : mButtons)
	{
        auto pos = button->Pos();
		auto left = pos.x;
		auto right = pos.x + pos.w;
		auto top = pos.y;
		auto bottom = pos.y + pos.h;
		
		if(cursor.x > left && cursor.x < right && cursor.y > top && cursor.y < bottom)
		{
			button->SetOver(1);
			return button->Ret();
		}
		else
			button->SetOver(0);
	}
	return MenuOption::NONE;
}


namespace Menues {

void Main::EnterState() {
    auto bg = Graphics::ReturnBg();
    auto cursor_tex = Graphics::ReturnCursor();
    mMenu = new Menu(bg, cursor_tex);
    mMenu->AddButton(60, 100, MenuOption::HostGame, "Host Game", Graphics::getFont("Text"));
    mMenu->AddButton(290, 100, MenuOption::JoinGame, "Join Game", Graphics::getFont("Text"));
    mMenu->AddButton(510, 100, MenuOption::Settings, "Settings", Graphics::getFont("Text"));
    mMenu->AddButton(700, 100, MenuOption::QuitGame, "Quit", Graphics::getFont("Text"));

}
void Main::ExitState() {
    delete mMenu;
}

std::string Main::HandleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            fmt::print("SDL_QUIT\n");
            return "QUIT";
        }

        if(event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE: // TODO Bug in SDL2 2.0.10 in WSL2
            //case SDL_SCANCODE_2:
                fmt::print("SDL_SCANCODE_ESCAPE\n");
                return "QUIT";
            }
        }

        if(event.type == SDL_MOUSEBUTTONUP) {
            for(auto button : mMenu->Buttons()) {
                if(button->Over()) {
                    switch(button->Ret()) {
                    case MenuOption::HostGame:
                        return "Game";
                    case MenuOption::JoinGame:
                    case MenuOption::Settings:
                    case MenuOption::QuitGame:
                        fmt::print("Button Quit\n");
                        return "QUIT";
                    }
                }
            }
        }
    }
    return "";
}

void Main::Draw() {
    auto cursor = Input::GetMousePos();
    auto cursor_tex = Graphics::ReturnCursor();

    mMenu->CheckButton(cursor);
    mMenu->Draw();
    Graphics::ApplySurface(cursor_tex, cursor.x, cursor.y, 1, 0);
}

}


}
