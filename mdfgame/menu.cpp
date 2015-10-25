#include "menu.h"

Menu* Menu_Init(SDL_Surface* bg, SDL_Surface* cursor)
{
	Menu* menu = new Menu;
	menu->bg = bg;
	menu->cursor = cursor;
	
	return menu;
}

void Menu_AddButton(Menu* menu,int x, int y, int ret, string text, TTF_Font *font)
{
    menu->list.push_back(Button_Init(x,y,ret,text, font));
}

int Menu_CheckButton(Menu* menu, int mx, int my)
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
	return -1;
}
