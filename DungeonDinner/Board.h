#ifndef BOARD_H
#define BOARD_H

#include "Header.h"
#include "Player.h"

class Board
{
private:
	int m_currentPlayer,
		m_firstPlayer,
		m_spawnDie,
		m_moveDie,
		m_selectedX,
		m_selectedY,
		m_selectedHex,
		m_selectedEnemy,
		m_enemyPlayer,
		m_spawnCount,
		m_moveCount;
	bool m_lcapsOn,
		m_rcapsOn,
		m_flip,
		m_selected;
	BoardState m_state;
	ALLEGRO_FONT* m_font;
	std::vector<int> m_spawnPools;
	std::vector<int> m_activeMonsters;
	std::vector<int> m_adjacentEmpties;
	std::vector<int> m_adjacentEnemies;
	std::vector<ALLEGRO_BITMAP*> m_hexBMPs;
	std::vector<Player> m_players;
	std::vector<Hex> m_hexes;

	//private functions
	void createHexPlate();
	void moveHexPlateX(bool right);
	void moveHexPlateY(bool down);
	void rotateHexPlate(bool clockwise);
	bool checkValidHexPlace();
	void placeHexPlate(ALLEGRO_EVENT& keyPressed);
	
	void findSpawnPools();
	void previousHex();
	void nextHex();
	void placeMonster(ALLEGRO_EVENT& keyPressed);

	void findMonsters();
	void findAdjacentEmpties();
	void moveMonster(ALLEGRO_EVENT& keyPressed);

	void findAdjacentEnemies();
	void findAdjacentFriends();
	bool eatOrEaten();
	void resolveEating(bool);
	void eatMonster(ALLEGRO_EVENT& keyPressed);
	
	void nextPlayer();

public:
	Board(void);
	//control functions
	void update();
	ScreenMode pressKey(ALLEGRO_EVENT& keyPressed);
	void releaseKey(ALLEGRO_EVENT& keyReleased);
	void makePlayers(const PlayerDetails& players);

	//access functions
	const BoardState getState() { return m_state; }
	const int getSpawnDie() { return m_spawnDie; }
	const int getMoveDie() { return m_moveDie; }
	const int getMoveCount() { return m_moveCount; }
	const int getSelectedX() { return m_selectedX; }
	const int getSelectedY() { return m_selectedY; }
	const int getNumberPlayers() { return m_players.size(); }
	Player& getPlayer(int index) { return m_players.at(index); }
	const ALLEGRO_FONT* getFont() { return m_font; }
	ALLEGRO_BITMAP* getHexBMP(int index) { return m_hexBMPs.at(index); }
	const std::vector<Hex>& getHexes() { return m_hexes; }

	void shutdown();
	~Board(void);
};

#endif