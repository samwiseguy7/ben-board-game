#ifndef PLAYER_H
#define PLAYER_H

#include "Header.h"

class Player
{
private:
	bool m_doneSpawn;
	int m_number,
		m_selectedMonster;
	PlayerColour m_colour;
	std::string m_name;
	std::vector<Monster> m_monsters;
	std::vector<int> m_resources;

	void sortMonsters();

public:
	Player(void);
	Player(const int, const PlayerColour, const std::string&);
	void placeMonster(int, int);
	void selectMonster(int index) { m_selectedMonster = index; }
	void selectMonster(int, int);
	void moveMonster(int, int);
	void levelUp();
	void levelUp(int, int);
	void demote();
	void removeMonster() { m_monsters.at(m_selectedMonster).placed = false; }

	//access functions
	const MLevel getLevel() { return m_monsters.at(m_selectedMonster).level; }
	const bool getDoneSpawn() { return m_doneSpawn; }
	const int getNumber() { return m_number; }
	const PlayerColour getColour() { return m_colour; }
	const std::string& getName() { return m_name; }
	const std::vector<Monster>& getMonsters() { return m_monsters; }

	~Player(void);
};

#endif