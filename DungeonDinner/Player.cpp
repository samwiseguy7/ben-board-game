#include "Player.h"

Player::Player(void)
	:m_number(0),
	m_colour(PlayerColour::RED),
	m_name(""),
	m_monsters(),
	m_resources(static_cast<int>(Terrain::TOTAL))
{
}

Player::Player(const int number, const PlayerColour colour, const std::string& name)
	:m_number(number),
	m_colour(colour),
	m_name(name),
	m_monsters(),
	m_resources(static_cast<int>(Terrain::TOTAL))
{
}

Player::~Player(void)
{
}
