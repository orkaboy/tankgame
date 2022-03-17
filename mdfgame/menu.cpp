#include "menu.h"
#include "graphics.h"

Menu* Menu_Init(SDL_Texture* bg, SDL_Texture* cursor)
{
	Menu* menu = new Menu;
	menu->bg = bg;
	menu->cursor = cursor;
	
	return menu;
}

void Menu_AddButton(Menu* menu,int x, int y, MenuOption ret, std::string text, TTF_Font *font)
{
    menu->list.push_back(Button_Init(x,y,ret,text, font));
}

MenuOption Menu_CheckButton(Menu* menu, int mx, int my)
{
	unsigned int i;
	int left,right,top,bottom;
	
	for(i=0; i<menu->list.size(); i++ )
	{
		left = menu->list[i]->pos.x;
		right = menu->list[i]->pos.x + menu->list[i]->pos.w;
		top = menu->list[i]->pos.y;
		bottom = menu->list[i]->pos.y + menu->list[i]->pos.h;
		
		if(mx > left && mx < right && my > top && my < bottom)
		{
			menu->list[i]->over = 1;
			return menu->list[i]->ret;
		}
		else
			menu->list[i]->over = 0;
	}
	return MenuOption::NONE;
}


MenuOption TheMenu ( bool *quit )
{
    SDL_Event event;

    SDL_Texture* bg = ReturnBg();
    SDL_Texture* cursor = ReturnCursor();
    Menu* menu = Menu_Init(bg,cursor);
    Menu_AddButton(menu,60,100,MenuOption::HostGame,"Host Game", getFont("Text"));
    Menu_AddButton(menu,290,100,MenuOption::JoinGame,"Join Game", getFont("Text"));
    Menu_AddButton(menu,510,100,MenuOption::Settings,"Settings", getFont("Text"));
    Menu_AddButton(menu,700,100,MenuOption::QuitGame,"Quit", getFont("Text"));


    bool mquit = false;
    int mx,my;
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
                for(i=0; i<menu->list.size(); i++)
                {
                    if(menu->list[i]->over)
                        return menu->list[i]->ret;
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

        SDL_GetMouseState(&mx, &my);

        Graphics_BeginScene();

        Menu_CheckButton(menu,mx,my);
        Menu_Draw(menu);
        Graphics_ApplySurface(cursor, mx, my,1,0);

        Graphics_EndScene();
    }
    return MenuOption::NONE;
}
