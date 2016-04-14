#include "Board.h"

Board::Board(void)
{
}

Screens Board::pressKey(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_ENTER:
		return Screens::MENU;
	default:
		return Screens::BOARD;
	}
}

void Board::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(keyReleased.keyboard.keycode)
	{
	}
}

Board::~Board(void)
{
}
