#include "Game.h"

Game::Game(void)
	:m_done(false),
	m_mode(Screens::MENU)
{
}

void Game::update() {}

void Game::pressKey(ALLEGRO_EVENT& keyPressed)
{
	if(keyPressed.keyboard.keycode==ALLEGRO_KEY_ESCAPE) { m_done = true; }
	else
	{
		Screens newMode = m_mode;
		switch(m_mode)
		{
		case Screens::MENU:
			newMode = m_menu.pressKey(keyPressed);
			break;
		case Screens::BOARD:
			newMode = m_board.pressKey(keyPressed);
			break;
		}
		m_mode = newMode;
	}
}

void Game::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(m_mode)
	{
	case Screens::MENU:
		m_menu.releaseKey(keyReleased);
		break;
	case Screens::BOARD:
		m_board.releaseKey(keyReleased);
		break;
	}
}

Game::~Game(void)
{
}
