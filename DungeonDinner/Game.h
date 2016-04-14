#ifndef GAME_H
#define GAME_H

#include "Header.h"
#include "Menu.h"
#include "Board.h"

class Game
{
private:
	bool m_done;
	Screens m_mode;
	Menu m_menu;
	Board m_board;

public:
	Game(void);
	void update();
	void pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);
	bool done() { return m_done; }
	Screens getMode() { return m_mode; }
	~Game(void);
};

#endif