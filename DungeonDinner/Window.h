#ifndef WINDOW_H
#define WINDOW_H

#include "Header.h"
#include "Game.h"

class Window
{
private:
	int m_width,
		m_height;
public:
	Window(int, int);
	void redraw(Game&);
	void redrawMenu(Menu&);
	void redrawBoard(Board&);
	~Window(void);
};

#endif