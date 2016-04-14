#ifndef GAME_H
#define GAME_H

#include "Header.h"

class Game
{
private:
	bool m_done;

public:
	Game(void);
	void update();
	void pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);
	bool done() { return m_done; }
	~Game(void);
};

#endif