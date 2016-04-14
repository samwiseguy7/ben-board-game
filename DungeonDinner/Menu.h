#ifndef MENU_H
#define MENU_H

#include "Header.h"

class Menu
{
public:
	Menu(void);
	void update();
	Screens pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);
	~Menu(void);
};

#endif