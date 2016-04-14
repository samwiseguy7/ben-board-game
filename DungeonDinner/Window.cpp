#include "Window.h"

Window::Window(int width, int height)
	:m_width(width),
	m_height(height)
{
}

void Window::redraw(Game& game)
{
	al_clear_to_color(al_map_rgb(0,0,0));
}

Window::~Window(void)
{
}
