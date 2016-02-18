#pragma once
class Player
{
private:
	int m_totalPlayers;
	int m_number;
	std::string m_name;
	int m_currentPlayer;
	int m_firstPlayer;
	int m_winner;

public:
	Player(void);
	
	//setters
	void setTotalPlayers(const int totalPlayers);
	void setNumber(const int number);
	void setName(const std::string name);
	void setCurrentPlayer(int currentPlayer);
	void setFirstPlayer(int firstPlayer);
	void setWinner(const int winner);

	//getters
	int getNumber();
	std::string getName();
	int getTotalPlayers();
	int getCurrentPlayer();
	int getFirstPlayer();
	int getWinner();

	~Player(void);
};

