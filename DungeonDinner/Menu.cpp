#include "Menu.h"

Menu::Menu(void)
	:m_state(MenuState::SELECT),
	m_option(MenuOp::NEW),
	m_option_str(static_cast<int>(MenuOp::TOTAL), ("")),
	m_font(al_load_font("C:/VS2012Projects/DungeonDinner/DungeonDinner/pirulen.ttf", static_cast<int>(TextSize::BIG), 0))
{
	m_option_str.at(0) = "New Game";
	m_option_str.at(1) = "Quit Game";
}

Screen Menu::pressKey(ALLEGRO_EVENT& keyPressed)
{
	switch(m_state)
	{
	case MenuState::SELECT:
		switch(keyPressed.keyboard.keycode)
		{
		case ALLEGRO_KEY_UP:
			if(m_option==MenuOp::NEW) { m_option = MenuOp::QUIT; }
			else { m_option = static_cast<MenuOp>(static_cast<int>(m_option) - 1); }
			return Screen::MENU;
		case ALLEGRO_KEY_DOWN:
			if(m_option==MenuOp::QUIT) { m_option = MenuOp::NEW; }
			else { m_option = static_cast<MenuOp>(static_cast<int>(m_option) + 1); }
			return Screen::MENU;
		case ALLEGRO_KEY_ENTER:
			switch(m_option)
			{
			case MenuOp::NEW:
				return Screen::BOARD;
			case MenuOp::QUIT:
				return Screen::DONE;
			}
			return Screen::MENU;
		}
		return Screen::MENU;
	}
	return Screen::MENU;
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
