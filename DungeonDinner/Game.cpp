#include "Game.h"

Game::Game(void)
	:m_done(false)
{
}

void Game::update() {}

void Game::pressKey(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_ESCAPE:
		m_done = true;
		break;
	}
}

void Game::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(keyReleased.keyboard.keycode)
	{
	}
}

Game::~Game(void)
{
}
