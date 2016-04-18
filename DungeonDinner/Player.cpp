#include "Player.h"

Player::Player(void)
	:m_doneSpawn(false),
	m_number(0),
	m_colour(PlayerColour::RED),
	m_name(""),
	m_monsters(static_cast<int>(MLevel::TOTAL), Monster()),
	m_resources(static_cast<int>(Terrain::TOTAL), 0)
{
}

Player::Player(const int number, const PlayerColour colour, const std::string& name)
	:m_doneSpawn(false),
	m_number(number),
	m_colour(colour),
	m_name(name),
	m_monsters(static_cast<int>(MLevel::TOTAL), Monster()),
	m_resources(static_cast<int>(Terrain::TOTAL), 0)
{
}

void Player::placeMonster(int x, int y)
{
	if(m_doneSpawn) { m_doneSpawn = false; }
	for(int i=0; i<m_monsters.size(); ++i)
	{
		if(!m_monsters.at(i).placed)
		{
			m_monsters.at(i).x = x;
			m_monsters.at(i).y = y;
			m_monsters.at(i).placed = true;
			m_monsters.at(i).moved = false;
			if(i==(m_monsters.size()-1)) { m_doneSpawn = true; }
			break;
		}
	}
}

Player::~Player(void)
{
}
