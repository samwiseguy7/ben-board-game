#include "Menu.h"

Menu::Menu(void)
	:m_setupDone(false),
	m_lcapsOn(false),
	m_rcapsOn(false),
	m_currentPlayer(0),
	m_currentColour(PlayerColour::RED),
	m_players(),
	m_state(MenuState::SELECT),
	m_option(MenuOp::NEW),
	m_text(static_cast<int>(MenuOp::TOTAL)),
	m_colourText(static_cast<int>(PlayerColour::TOTAL)),
	m_font(al_load_font("pirulen.ttf", static_cast<int>(TextSize::BIG), 0))
{
	changeStateSelect();
	m_colourText.at(static_cast<int>(PlayerColour::RED)) = "Red";
	m_colourText.at(static_cast<int>(PlayerColour::GREEN)) = "Green";
	m_colourText.at(static_cast<int>(PlayerColour::BLUE)) = "Blue";
	m_colourText.at(static_cast<int>(PlayerColour::YELLOW)) = "Yellow";
	m_colourText.at(static_cast<int>(PlayerColour::CYAN)) = "Cyan";
	m_colourText.at(static_cast<int>(PlayerColour::MAGENTA)) = "Magenta";
}

ScreenMode Menu::pressKey(ALLEGRO_EVENT& keyPressed)
{
	switch(m_state)
	{
	case MenuState::SELECT:
		return selectOption(keyPressed);
	case MenuState::NUMBER:
		numberPlayers(keyPressed);
		return ScreenMode::MENU;
	case MenuState::NAME:
		namePlayers(keyPressed);
		return ScreenMode::MENU;
	case MenuState::COLOUR:
		colourPlayers(keyPressed);
		return ScreenMode::MENU;
	}
	return ScreenMode::MENU;
}

void Menu::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(keyReleased.keyboard.keycode)
	{
	case ALLEGRO_KEY_LSHIFT:
		m_lcapsOn = false;
		break;
	case ALLEGRO_KEY_RSHIFT:
		m_rcapsOn = false;
		break;
	}
}

void Menu::changeStateSelect()
{
	m_text.at(0) = "New Game";
	m_text.at(1) = "Quit Game";
	m_state = MenuState::SELECT;
}

void Menu::previousOption()
{
	if(m_option==MenuOp::NEW) { m_option = MenuOp::QUIT; }
	else { m_option = static_cast<MenuOp>(static_cast<int>(m_option) - 1); }
}

void Menu::nextOption()
{
	if(m_option==MenuOp::QUIT) { m_option = MenuOp::NEW; }
	else { m_option = static_cast<MenuOp>(static_cast<int>(m_option) + 1); }
}

ScreenMode Menu::selectOption(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
		previousOption();
		return ScreenMode::MENU;
	case ALLEGRO_KEY_DOWN:
		nextOption();
		return ScreenMode::MENU;
	case ALLEGRO_KEY_ENTER:
		switch(m_option)
		{
		case MenuOp::NEW:
			changeStateNumber();
			return ScreenMode::MENU;
		case MenuOp::QUIT:
			return ScreenMode::DONE;
		}
		return ScreenMode::MENU;
	case ALLEGRO_KEY_ESCAPE:
		return ScreenMode::DONE;
	}
	return ScreenMode::MENU;
}

void Menu::changeStateNumber()
{
	m_state = MenuState::NUMBER;
	m_text.at(0) = "How many Players\?";
	m_text.at(1) = "";
}

void Menu::numberPlayers(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_2:
	case ALLEGRO_KEY_3:
	case ALLEGRO_KEY_4:
		m_players.number = keyPressed.keyboard.keycode - 27;
		m_text.at(1) = std::to_string(m_players.number);
		break;
	case ALLEGRO_KEY_ENTER:
		if(m_text.at(1) != "")
		{ changeStateName(); }
		break;
	case ALLEGRO_KEY_ESCAPE:
		changeStateSelect();
		break;
	}
}

void Menu::changeStateName()
{
	if(m_currentPlayer==m_players.number) { m_currentPlayer = 0; changeStateColour(); }
	else
	{
		m_text.at(0) = std::string("Name of Player " + std::to_string(m_currentPlayer+1) + "\?");
		m_text.at(1) = "";
		m_state = MenuState::NAME;
	}
}

bool Menu::checkValidName()
{
	if(m_text.at(1)=="") { return false; }
	for(int i=0; i<m_currentPlayer; ++i)
	{
		if(m_text.at(1)==m_players.name[i]) { return false; }
	}
	return true;
}

void Menu::namePlayers(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_LSHIFT:
		m_lcapsOn = true;
		break;
	case ALLEGRO_KEY_RSHIFT:
		m_rcapsOn = true;
		break;
	case ALLEGRO_KEY_A:
	case ALLEGRO_KEY_B:
	case ALLEGRO_KEY_C:
	case ALLEGRO_KEY_D:
	case ALLEGRO_KEY_E:
	case ALLEGRO_KEY_F:
	case ALLEGRO_KEY_G:
	case ALLEGRO_KEY_H:
	case ALLEGRO_KEY_I:
	case ALLEGRO_KEY_J:
	case ALLEGRO_KEY_K:
	case ALLEGRO_KEY_L:
	case ALLEGRO_KEY_M:
	case ALLEGRO_KEY_N:
	case ALLEGRO_KEY_O:
	case ALLEGRO_KEY_P:
	case ALLEGRO_KEY_Q:
	case ALLEGRO_KEY_R:
	case ALLEGRO_KEY_S:
	case ALLEGRO_KEY_T:
	case ALLEGRO_KEY_U:
	case ALLEGRO_KEY_V:
	case ALLEGRO_KEY_W:
	case ALLEGRO_KEY_X:
	case ALLEGRO_KEY_Y:
	case ALLEGRO_KEY_Z:
		if(m_rcapsOn || m_lcapsOn) { m_text.at(1) += int(keyPressed.keyboard.keycode + 64); } //convert allegro keycode to ascii char
		else { m_text.at(1) += int(keyPressed.keyboard.keycode + 96); }
		break;
	case ALLEGRO_KEY_SPACE:
		m_text.at(1) += " ";
		break;
	case ALLEGRO_KEY_BACKSPACE:
		if(m_text.at(1).size()>0) { m_text.at(1).pop_back(); }
		break;
	case ALLEGRO_KEY_ENTER:
		if(checkValidName())
		{
			m_players.name[m_currentPlayer] = m_text.at(1);
			++m_currentPlayer;
			changeStateName();
		}
		break;
	case ALLEGRO_KEY_ESCAPE:
		m_currentPlayer = 0;
		changeStateNumber();
		break;
	}
}

void Menu::changeStateColour()
{
	if(m_currentPlayer==m_players.number)
	{
		m_currentPlayer = 0;
		m_currentColour = PlayerColour::RED;
		changeStateSelect();
		m_setupDone = true;
	}
	else
	{
		m_text.at(0) = std::string("Colour of Player " + std::to_string(m_currentPlayer+1) + "\?");
		m_text.at(1) = m_colourText.at(static_cast<int>(m_currentColour));
		m_state = MenuState::COLOUR;
	}
}

void Menu::previousColour()
{
	if(m_currentColour==PlayerColour::RED) { m_currentColour = PlayerColour::MAGENTA; }
	else { m_currentColour = static_cast<PlayerColour>(static_cast<int>(m_currentColour) - 1); }
	for(int i=0; i<m_currentPlayer; ++i)
	{ if(m_currentColour==m_players.colour[i]) { previousColour(); } }
	m_text.at(1) = m_colourText.at(static_cast<int>(m_currentColour));
}

void Menu::nextColour()
{
	if(m_currentColour==PlayerColour::MAGENTA) { m_currentColour = PlayerColour::RED; }
	else { m_currentColour = static_cast<PlayerColour>(static_cast<int>(m_currentColour) + 1); }
	for(int i=0; i<m_currentPlayer; ++i)
	{ if(m_currentColour==m_players.colour[i]) { nextColour(); } }
	m_text.at(1) = m_colourText.at(static_cast<int>(m_currentColour));
}

void Menu::colourPlayers(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
		previousColour();
		break;
	case ALLEGRO_KEY_DOWN:
		nextColour();
		break;
	case ALLEGRO_KEY_ENTER:
		m_players.colour[m_currentPlayer] = m_currentColour;
		++m_currentPlayer;
		nextColour();
		changeStateColour();
		break;
	case ALLEGRO_KEY_ESCAPE:
		m_currentPlayer = 0;
		changeStateName();
		break;
	}
}

void Menu::shutdown()
{
	al_destroy_font(m_font);
}

Menu::~Menu(void)
{
}
