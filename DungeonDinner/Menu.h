#ifndef MENU_H
#define MENU_H

#include "Header.h"

class Menu
{
private:
	bool m_setupDone,
		m_lcapsOn,
		m_rcapsOn;
	int m_currentPlayer;
	PlayerColour m_currentColour;
	ALLEGRO_FONT* m_font;
	std::vector<std::string> m_text;
	std::vector<std::string> m_colourText;
	MenuOp m_option;
	MenuState m_state;
	PlayerDetails m_players;

	//private functions
	void changeStateSelect();
	void previousOption();
	void nextOption();
	ScreenMode selectOption(ALLEGRO_EVENT& keyPressed);

	void changeStateNumber();
	void numberPlayers(ALLEGRO_EVENT& keyPressed);

	void changeStateName();
	bool checkValidName();
	void namePlayers(ALLEGRO_EVENT& keyPressed);

	void changeStateColour();
	void previousColour();
	void nextColour();
	void colourPlayers(ALLEGRO_EVENT& keyPressed);

public:
	Menu(void);

	//control functions
	void update();
	ScreenMode pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);

	//access functions
	PlayerDetails& getPlayerDetails() { return m_players; }
	const bool setupDone() { if(m_setupDone) { m_setupDone = false; return true; } else return false; }
	const MenuState getState() { return m_state; }
	const MenuOp getOption() { return m_option; }
	const std::vector<std::string>& getText() { return m_text; }
	const ALLEGRO_FONT* getFont() { return m_font; }
	
	void shutdown();
	~Menu(void);
};

#endif