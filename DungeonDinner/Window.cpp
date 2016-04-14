#include "Window.h"

Window::Window(int width, int height)
	:m_width(width),
	m_height(height)
{
}

void Window::redraw(Game& game)
{
	switch(game.getMode())
	{
	case Screen::MENU:
		redrawMenu(game.getMenu());
		break;
	case Screen::BOARD:
		redrawBoard(game.getBoard());
		break;
	}
}

void Window::redrawMenu(Menu& menu)
{
	ALLEGRO_COLOR c_black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR c_dim = al_map_rgb(120, 120, 120);
	ALLEGRO_COLOR c_lit = al_map_rgb(255, 255, 255);
	al_clear_to_color(c_black);
	switch(menu.getState())
	{
	case MenuState::SELECT:
		for(int i=0; i<static_cast<int>(MenuOp::TOTAL); ++i)
		{
			ALLEGRO_COLOR colour;
			if(menu.getOption()==static_cast<MenuOp>(i)) { colour = c_lit; } else { colour = c_dim; }
			al_draw_text(menu.getFont(), colour, m_width*0.5,
				(m_height+((2*i)-static_cast<int>(MenuOp::TOTAL))*static_cast<int>(TextSize::BIG))*0.5,
				ALLEGRO_ALIGN_CENTRE, menu.getOptionStr(i).c_str());
		}
		break;
	}
}

void Window::redrawBoard(Board& board)
{
	al_clear_to_color(al_map_rgb(0, 150, 0));
}

Window::~Window(void)
{
}
