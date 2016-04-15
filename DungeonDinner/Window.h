#ifndef WINDOW_H
#define WINDOW_H

#include "Header.h"
#include "Game.h"

class Window
{
private:
	int m_width,
		m_height;
	std::vector<ALLEGRO_COLOR> m_colours;

public:
	Window(int, int);
	void redraw(Game&);
	void drawMenuSelect(Menu&);
	void drawMenuText(Menu&);
	void drawMenuColourText(Menu&);
	void redrawMenu(Menu&);
	void redrawBoard(Board&);
	void shutdown();
	~Window(void);
};

#endif