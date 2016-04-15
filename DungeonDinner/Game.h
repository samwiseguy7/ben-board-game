#ifndef GAME_H
#define GAME_H

#include "Header.h"
#include "Menu.h"
#include "Board.h"

class Game
{
private:
	bool m_done;
	ScreenMode m_mode;
	Menu m_menu;
	Board m_board;

public:
	Game(void);
	void update();
	void pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);

	const bool done() { return m_done; }
	const ScreenMode getMode() { return m_mode; }
	Menu& getMenu() { return m_menu; }
	Board& getBoard() { return m_board; }
	
	void shutdown();
	~Game(void);
};

#endif