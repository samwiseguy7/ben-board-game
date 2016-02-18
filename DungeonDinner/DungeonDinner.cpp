// DungeonDinner.cpp : Defines the entry point for the console application.

#include "stdafx.h"

//classes
#include "Player.h"
#include "Dungeon.h"
#include "Monsters.h"


//global variables (leave for now, remove with great prejudice later)


//forward declarations
int askHowMany();
std::string askPlayerName();
Player* getPlayers();
void printPlayers(Player* player);
Dungeon* buildDungeon(Player* player);
Monsters* prepareMonsters(Player* player);

Monsters* spawnMonsters(Player* player, Dungeon* dungeon, Monsters* monsters);
Monsters* dinnerTime(Player* player, Dungeon* dungeon, Monsters* monsters);
Dungeon* expandDungeon(Player* player, Dungeon* dungeon);
void playGame(Player* player, Dungeon* dungeon, Monsters* monsters);

void printScore(Player* player, Monsters* monsters);
void score(Player* player, Monsters* monsters);
void shutDown();


//setup game functions
int askHowMany()
{
	std::cout << "asking how many players..." << std::endl;
	const int maxPlayers = 6;
	const int minPlayers = 2;
	int totalPlayers = minPlayers; //default to minimum players
	bool totalPlayersValid = false; //default to false until proven true
	while (!totalPlayersValid)
	{
		std::cin >> totalPlayers;
		if (std::cin.fail() || totalPlayers>maxPlayers || totalPlayers<minPlayers) //in case of bad input
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove newline from buffer
			std::cout << "invalid input, trying again..." << std::endl;
		}
		else //otherwise flush buffer and proceed
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			totalPlayersValid = true;
		}
	}
	return totalPlayers;
}

std::string askPlayerName()
{
	std::string playerName = "no_name";
	std::cout << "asking player name..." << std::endl;
    std::getline(std::cin, playerName);
	return playerName;
}

Player* getPlayers()
{
	int totalPlayers = askHowMany();
	std::string playerName = "no_name";
	Player *player = new Player[totalPlayers]; //make pointer to declared array of Player objects (can only use default constructor)
	std::cout << "naming players..." << std::endl;
	for (int i=0; i < totalPlayers; ++i) //loop until all the Player objects have details initialized
	{
		playerName = askPlayerName();
		player[i].setTotalPlayers(totalPlayers);
		player[i].setNumber(i+1); //i+1 because the index number of an array starts at 0 but we want player no.1, 2, etc.
		player[i].setName(playerName);
		std::cout << "added player details..." << std::endl;
	}
	return player;
}

void printPlayers(Player* player)
{
	int totalPlayers = player[0].getTotalPlayers();
	for (int i=0; i < totalPlayers; ++i) //loop until all the Player objects are printed
	{
		int playerNumber = player[i].getNumber();
		std::string playerName = player[i].getName();
		std::cout << playerNumber << ": " << playerName << std::endl;
	}
}

Dungeon* buildDungeon(Player* player)
{
	std::cout << "building dungeon..." << std::endl;
	return new Dungeon;
}

Monsters* prepareMonsters(Player* player)
{
	std::cout << "preparing monsters..." << std::endl;
	return new Monsters;
}


//play game functions
Monsters* spawnMonsters(Player* player, Dungeon* dungeon, Monsters* monsters)
{
	int currentPlayer = player[0].getCurrentPlayer();
	std::string name = player[currentPlayer].getName();
	std::cout << "spawning monsters for " << name << "..." << std::endl;
	return monsters;
}

Monsters* dinnerTime(Player* player, Dungeon* dungeon, Monsters* monsters)
{
	int currentPlayer = player[0].getCurrentPlayer();
	std::string name = player[currentPlayer].getName();
	std::cout << name << "\'s monsters are having dinner..." << std::endl;
	return monsters;
}

Dungeon* expandDungeon(Player* player, Dungeon* dungeon)
{
	int currentPlayer = player[0].getCurrentPlayer();
	std::string name = player[currentPlayer].getName();
	std::cout << name << " is expanding dungeon..." << std::endl;
	return dungeon;
}

void playGame(Player* player, Dungeon* dungeon, Monsters* monsters)
{
	std::cout << "playing game..." << std::endl;
	int totalPlayers = player[0].getTotalPlayers();
	int currentPlayer = player[0].getFirstPlayer();
	int totalRounds = 12;
	for (int i=0; i < totalRounds; ++i) //loop rounds until game ends
	{
		currentPlayer = player[0].getFirstPlayer();
		player[0].setCurrentPlayer(currentPlayer);
		for (int i=0; i < totalPlayers; ++i)
		{
			spawnMonsters(player, dungeon, monsters);
			dinnerTime(player, dungeon, monsters);
			currentPlayer ++;
			if (currentPlayer==totalPlayers) {currentPlayer = 0; player[0].setCurrentPlayer(0);}
			player[0].setCurrentPlayer(currentPlayer);
		}
		for (int i=0; i < totalPlayers; ++i)
		{
			player[0].setCurrentPlayer(currentPlayer);
			expandDungeon(player, dungeon);
			currentPlayer ++;
			if (currentPlayer==totalPlayers) {currentPlayer = 0; player[0].setCurrentPlayer(0);}
		}
		currentPlayer ++;
		if (currentPlayer==totalPlayers) {currentPlayer = 0; player[0].setCurrentPlayer(0);}
		player[0].setFirstPlayer(currentPlayer);
	}
	std::cout << "ending game..." << std::endl;
}

//end game functions
void printScore(Player* player, Monsters* monsters)
{
	int winner = player[0].getWinner();
	std::string name = player[winner].getName();
	std::cout << name << " is the winner!" << std::endl;
}

void score(Player* player, Monsters* monsters)
{
	std::cout << "starting scoring..." << std::endl;
	printScore(player, monsters);
}

void shutDown()
{
	std::cout << "shutting down..." << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//setup
	Player *player = getPlayers();
	printPlayers(player);
	Dungeon *dungeon = buildDungeon(player);
	Monsters *monsters = prepareMonsters(player);

	playGame(player, dungeon, monsters);

	score(player, monsters);

	//destroy everything
	shutDown();
	delete[] player;
	delete dungeon;
	delete monsters;

	return 0;
}

