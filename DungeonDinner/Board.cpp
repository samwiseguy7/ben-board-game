#include "Board.h"

Board::Board(void)
{
}

ScreenMode Board::pressKey(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_ESCAPE:
		return ScreenMode::MENU;
	}
	return ScreenMode::BOARD;
}

void Board::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(keyReleased.keyboard.keycode)
	{
	}
}

void Board::shutdown()
{}

Board::~Board(void)
{
}
