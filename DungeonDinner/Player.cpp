#include "stdafx.h"
#include "Player.h"


Player::Player(void)
	: m_totalPlayers(2), m_number(0), m_name("no_name"), m_currentPlayer(0), m_firstPlayer(0), m_winner(0)
{
	std::cout << "created new player..." << std::endl;
}

//setters
void Player::setTotalPlayers(const int totalPlayers) { m_totalPlayers = totalPlayers; }
void Player::setNumber(const int number) { m_number = number; }
void Player::setName(const std::string name) { m_name = name; }
void Player::setCurrentPlayer(int currentPlayer) { m_currentPlayer = currentPlayer; }
void Player::setFirstPlayer(int firstPlayer) { m_firstPlayer = firstPlayer; }
void Player::setWinner(const int winner) { m_winner = winner; }

//getters
int Player::getTotalPlayers() { return m_totalPlayers; }
int Player::getNumber() { return m_number; }
std::string Player::getName() { return m_name; }
int Player::getCurrentPlayer() { return m_currentPlayer; }
int Player::getFirstPlayer() { return m_firstPlayer; }
int Player::getWinner() { return m_winner; }

Player::~Player(void)
{
	std::cout << "player " << m_name << " destroyed..." << std::endl;
}
