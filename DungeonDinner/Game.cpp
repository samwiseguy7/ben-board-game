#include "Game.h"

Game::Game(void)
	:m_done(false),
	m_mode(ScreenMode::MENU)
{
}

void Game::update() {}

void Game::pressKey(ALLEGRO_EVENT& keyPressed)
{
	ScreenMode newMode;
	switch(m_mode)
	{
	case ScreenMode::MENU:
		newMode = m_menu.pressKey(keyPressed);
		if(m_menu.setupDone())
		{ newMode = ScreenMode::BOARD; }
		break;
	case ScreenMode::BOARD:
		newMode = m_board.pressKey(keyPressed);
		break;
	}
	m_mode = newMode;
	if(m_mode==ScreenMode::DONE) { m_done = true; }
}

void Game::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(m_mode)
	{
	case ScreenMode::MENU:
		m_menu.releaseKey(keyReleased);
		break;
	case ScreenMode::BOARD:
		m_board.releaseKey(keyReleased);
		break;
	}
}

void Game::shutdown()
{
	m_menu.shutdown();
	m_board.shutdown();
}

Game::~Game(void)
{
}
