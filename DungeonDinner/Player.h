#ifndef PLAYER_H
#define PLAYER_H

#include "Header.h"

class Player
{
private:
	int m_number;
	PlayerColour m_colour;
	std::string m_name;
	std::vector<Monster> m_monsters;
	std::vector<int> m_resources;

public:
	Player(void);
	Player(const int, const PlayerColour, const std::string&);

	//access functions
	const int getNumber() { return m_number; }
	const PlayerColour getColour() { return m_colour; }
	const std::string& getName() { return m_name; }

	~Player(void);
};

#endif