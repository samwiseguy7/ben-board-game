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
	case Screens::MENU:
		redrawMenu(game);
		break;
	case Screens::BOARD:
		redrawBoard(game);
		break;
	}
}

void Window::redrawMenu(Game& game)
{
	al_clear_to_color(al_map_rgb(150, 0, 0));
}

void Window::redrawBoard(Game& game)
{
	al_clear_to_color(al_map_rgb(0, 150, 0));
}

Window::~Window(void)
{
}
