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

void Player::selectMonster(int x, int y)
{
	for(int i=0; i<m_monsters.size(); ++i)
	{
		if(m_monsters.at(i).x==x && m_monsters.at(i).y==y)
		{ m_selectedMonster = i; break; }
	}
}

void Player::moveMonster(int x, int y)
{
	m_monsters.at(m_selectedMonster).x = x;
	m_monsters.at(m_selectedMonster).y = y;
	m_monsters.at(m_selectedMonster).moved = true;
}

void Player::levelUp()
{
	if(m_monsters.at(m_selectedMonster).level!=MLevel::GOAT)
	{
		m_monsters.at(m_selectedMonster).level = static_cast<MLevel>(static_cast<int>(m_monsters.at(m_selectedMonster).level)+1);
	}
	sortMonsters();
}

void Player::levelUp(int x, int y)
{
	for(int i=0; i<m_monsters.size(); ++i)
	{
		if(m_monsters.at(i).x==x && m_monsters.at(i).y==y)
		{
			if(m_monsters.at(m_selectedMonster).level!=MLevel::GOAT)
			{
				m_monsters.at(m_selectedMonster).level = static_cast<MLevel>(static_cast<int>(m_monsters.at(m_selectedMonster).level)+1);
			}
			sortMonsters();
			break;
		}
	}
}

void Player::demote()
{
	if(m_monsters.at(m_selectedMonster).level!=MLevel::SLIME)
	{
		m_monsters.at(m_selectedMonster).level = static_cast<MLevel>(static_cast<int>(m_monsters.at(m_selectedMonster).level)-1);
	}
	sortMonsters();
}

void Player::sortMonsters()
{
	for(int start=0; start<m_monsters.size(); ++start)
	{
		int highest = start;
		for(int current=start+1; current<m_monsters.size(); ++current)
		{
			if(m_monsters.at(current).level>m_monsters.at(highest).level)
			{ highest = current; }
		}
		std::swap(m_monsters.at(start), m_monsters.at(highest));
	}
}

Player::~Player(void)
{
}
