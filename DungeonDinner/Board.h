#ifndef BOARD_H
#define BOARD_H

#include "Header.h"

class Board
{
public:
	Board(void);
	void update();
	Screens pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);
	~Board(void);
};

#endif