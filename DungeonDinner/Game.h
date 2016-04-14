#ifndef GAME_H
#define GAME_H

#include "Header.h"
#include "Menu.h"
#include "Board.h"

class Game
{
private:
	Screen m_mode;
	Menu m_menu;
	Board m_board;

public:
	Game(void);
	void update();
	void pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);

	const bool done() { if(m_mode==Screen::DONE) { return true; } else { return false; } }
	const Screen getMode() { return m_mode; }
	Menu& getMenu() { return m_menu; }
	Board& getBoard() { return m_board; }

	~Game(void);
};

#endif