#include "Board.h"

Board::Board(void)
	:m_players(),
	m_font(al_load_font("pirulen.ttf", static_cast<int>(TextSize::MEDIUM), 0))
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

void Board::makePlayers(const PlayerDetails& players)
{
	m_players.reserve(players.number);
	for(int i=0; i<players.number; ++i)
	{
		m_players.push_back(Player(i, players.colour[i], players.name[i]));
	}
}

void Board::shutdown()
{
	al_destroy_font(m_font);
}

Board::~Board(void)
{
}
