#include "Menu.h"

Menu::Menu(void)
{
}

Screens Menu::pressKey(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_ENTER:
		return Screens::BOARD;
	default:
		return Screens::MENU;
	}
}

void Menu::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(keyReleased.keyboard.keycode)
	{
	}
}

Menu::~Menu(void)
{
}
