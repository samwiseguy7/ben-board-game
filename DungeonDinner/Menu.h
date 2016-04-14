#ifndef MENU_H
#define MENU_H

#include "Header.h"

class Menu
{
private:
	std::vector<std::string> m_option_str;
	ALLEGRO_FONT* m_font;
	MenuOp m_option;
	MenuState m_state;

public:
	Menu(void);
	void update();
	Screen pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);

	const MenuState getState() { return m_state; }
	const MenuOp getOption() { return m_option; }
	const std::string& getOptionStr(int index) { return m_option_str.at(index); }
	const ALLEGRO_FONT* getFont() { return m_font; }

	~Menu(void);
};

#endif