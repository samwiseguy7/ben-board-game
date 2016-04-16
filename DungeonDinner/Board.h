#ifndef BOARD_H
#define BOARD_H

#include "Header.h"
#include "Player.h"

class Board
{
private:
	ALLEGRO_FONT* m_font;
	std::vector<ALLEGRO_BITMAP*> m_hexes;
	std::vector<Player> m_players;

public:
	Board(void);
	//control functions
	void update();
	ScreenMode pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);
	void makePlayers(const PlayerDetails& players);

	//access functions
	const int getNumberPlayers() { return m_players.size(); }
	Player& getPlayer(int index) { return m_players.at(index); }
	const ALLEGRO_FONT* getFont() { return m_font; }
	ALLEGRO_BITMAP* getHexBMP(int index) { return m_hexes.at(index); }

	void shutdown();
	~Board(void);
};

#endif