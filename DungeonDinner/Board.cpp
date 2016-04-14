#include "Board.h"

Board::Board(void)
{
}

Screen Board::pressKey(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_ENTER:
		return Screen::MENU;
	default:
		return Screen::BOARD;
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
