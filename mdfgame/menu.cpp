#include "menu.h"
#include "graphics.h"

#include "input.h"
#include "gamestate.h"

namespace MDF {

Menu::Menu(SDL_Texture* bg, SDL_Texture* cursor)
{
	mBg = bg;
	mCursor = cursor;
}

void Menu::AddButton(int x, int y, MenuOption ret, std::string text, TTF_Font *font)
{
    mButtons.push_back(new MDF::Button(x, y, ret, text, font));
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


MenuOption TheMenu ( bool *quit )
{
    SDL_Event event;

    auto bg = Graphics::ReturnBg();
    auto cursor_tex = Graphics::ReturnCursor();
    Menu menu(bg, cursor_tex);
    menu.AddButton(60, 100, MenuOption::HostGame, "Host Game", Graphics::getFont("Text"));
    menu.AddButton(290, 100, MenuOption::JoinGame, "Join Game", Graphics::getFont("Text"));
    menu.AddButton(510, 100, MenuOption::Settings, "Settings", Graphics::getFont("Text"));
    menu.AddButton(700, 100, MenuOption::QuitGame, "Quit", Graphics::getFont("Text"));


    bool mquit = false;
    unsigned int i;
    float timestamp = 0.0f;
    const float dt = 1.0 / FPS;
    float currentTime = SDL_GetTicks();
    float accumulator = 0.0f;

    while(!mquit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                mquit = true;
                *quit = true;
            }

            if(event.type == SDL_KEYDOWN)
            {
                switch ( event.key.keysym.scancode )
                {
                case SDL_SCANCODE_E:
                    mquit = true;
                break;
                case SDL_SCANCODE_ESCAPE:
                    mquit = true;
                    *quit = true;
                break;
                default:
                break;
                }
            }
            if( event.type == SDL_MOUSEBUTTONUP )
            {
                for(auto button : menu.Buttons())
                {
                    if(button->Over())
                        return button->Ret();
                }
            }
        }

        float newTime = (float)SDL_GetTicks();
        float deltaTime = (newTime - currentTime) / 1000.f;
        currentTime = newTime;

        accumulator += deltaTime;

        while (accumulator>=dt)
        {
            timestamp += dt;
            accumulator -= dt;
        }


        Graphics::BeginScene();
        auto cursor = Input::GetMousePos();
        menu.CheckButton(cursor);
        Graphics::Menu_Draw(&menu);
        Graphics::ApplySurface(cursor_tex, cursor.x, cursor.y, 1, 0);

        Graphics::EndScene();
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
            return "QUIT";
        }

        if(event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                return "QUIT";
            }
        }
        if(event.type == SDL_MOUSEBUTTONUP) {
            for(auto button : mMenu->Buttons()) {
                if(button->Over()) {
                    switch(button->Ret()) {
                    case MenuOption::HostGame:
                    case MenuOption::JoinGame:
                    case MenuOption::Settings:
                    case MenuOption::QuitGame:
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
    Graphics::Menu_Draw(mMenu);
    Graphics::ApplySurface(cursor_tex, cursor.x, cursor.y, 1, 0);
}

}


}
