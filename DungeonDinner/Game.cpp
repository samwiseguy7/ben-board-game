#include "Game.h"

Game::Game(void)
	:m_mode(Screen::MENU)
{
}

void Game::update() {}

void Game::pressKey(ALLEGRO_EVENT& keyPressed)
{
	Screen newMode = m_mode;
	switch(m_mode)
	{
	case Screen::MENU:
		newMode = m_menu.pressKey(keyPressed);
		break;
	case Screen::BOARD:
		newMode = m_board.pressKey(keyPressed);
		break;
	}
	m_mode = newMode;
}

void Game::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(m_mode)
	{
	case Screen::MENU:
		m_menu.releaseKey(keyReleased);
		break;
	case Screen::BOARD:
		m_board.releaseKey(keyReleased);
		break;
	}
}

Game::~Game(void)
{
}
