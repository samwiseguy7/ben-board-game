#include "Board.h"

Board::Board(void)
	:m_lcapsOn(false),
	m_rcapsOn(false),
	m_flip(false),
	m_currentPlayer(0),
	m_firstPlayer(0),
	m_spawnDie(0),
	m_moveDie(0),
	m_spawnCount(0),
	m_selectedX(0),
	m_selectedY(0),
	m_selectedPool(0),
	m_state(BoardState::START),
	m_players(),
	m_spawnPools(),
	m_font(al_load_font("pirulen.ttf", static_cast<int>(TextSize::MEDIUM), 0)),
	m_hexBMPs(2, nullptr),
	m_hexes()
{
	m_hexBMPs.at(0) = al_load_bitmap("FullHex.png");
	m_hexBMPs.at(1) = al_load_bitmap("Target.png");
}

ScreenMode Board::pressKey(ALLEGRO_EVENT& keyPressed)
{
	if(keyPressed.keyboard.keycode==ALLEGRO_KEY_ESCAPE) { return ScreenMode::MENU; }
	switch(m_state)
	{
	case BoardState::START:
	case BoardState::PLACEHEX:
		placeHexPlate(keyPressed);
		break;
	case BoardState::SPAWN:
		placeMonster(keyPressed);
		break;
	case BoardState::MOVE:
//		moveMonster(keyPressed);
		break;
//	case BoardState::EAT:
//		eatMonster(keyPressed);
//		break;
//	case BoardState::SCORE:
//		return ScreenMode::DONE;
	}
	return ScreenMode::BOARD;
}

void Board::releaseKey(ALLEGRO_EVENT& keyReleased)
{
	switch(keyReleased.keyboard.keycode)
	{
	case ALLEGRO_KEY_LSHIFT:
		m_lcapsOn = false;
		break;
	case ALLEGRO_KEY_RSHIFT:
		m_rcapsOn = false;
		break;
	}
}

void Board::makePlayers(const PlayerDetails& players)
{
	m_players.clear();
	m_hexes.clear();
	m_selectedX = m_selectedY = m_selectedPool = m_currentPlayer = m_flip = m_spawnDie = m_moveDie = m_spawnCount = 0;
	m_players.reserve(players.number);
	for(int i=0; i<players.number; ++i)
	{
		m_players.push_back(Player(i, players.colour[i], players.name[i]));
	}
	m_firstPlayer = m_players.size()-1;
	m_hexes.reserve(7*(m_players.size()+7));
	m_state = BoardState::START;
	createHexPlate();
	nextPlayer();
}

void Board::createHexPlate()
{
	int x[7] = {m_selectedX, m_selectedX, m_selectedX+1, m_selectedX+1, m_selectedX, m_selectedX-1, m_selectedX-1};
	int y[7] = {m_selectedY, m_selectedY-1, m_selectedY-1, m_selectedY, m_selectedY+1, m_selectedY, m_selectedY-1};	
	if(m_selectedX%2)
	{
		++y[2];
		++y[3];
		++y[5];
		++y[6];
	}
	int numbers[7] = {0, 1, 2, 3, 4, 5, 6};
	Terrain terrains[7];
	terrains[0] = static_cast<Terrain>(rand()%7);
	for(int i=1; i<7; ++i)
	{
		bool unique = false;
		while(!unique)
		{
			terrains[i] = static_cast<Terrain>(rand()%7);
			for(int p=0; p<i; ++p)
			{
				unique = true;
				if(terrains[p]==terrains[i]) { unique = false; break; }
			}
		}
	}
	for(int i=0; i<7; ++i)
	{
		Hex hex = {x[i], y[i], numbers[i], terrains[i], false};
		m_hexes.push_back(hex);
	}
}

void Board::moveHexPlateX(bool right)
{
	for(int i=m_hexes.size()-7; i<m_hexes.size(); ++i)
	{
		if(right) { ++m_hexes.at(i).x; }
		else { --m_hexes.at(i).x; }
		if(m_hexes.at(i).x%2) { --m_hexes.at(i).y; }
		if(m_flip) { ++m_hexes.at(i).y; }
	}
	if(!m_flip) { m_flip = true; }
	else { m_flip = false; }

	m_selectedX = m_hexes.at(m_hexes.size()-7).x;
	m_selectedY = m_hexes.at(m_hexes.size()-7).y;
}

void Board::moveHexPlateY(bool down)
{
	for(int i=m_hexes.size()-7; i<m_hexes.size(); ++i)
	{
		if(down) { ++m_hexes.at(i).y; }
		else { --m_hexes.at(i).y; }
	}

	m_selectedX = m_hexes.at(m_hexes.size()-7).x;
	m_selectedY = m_hexes.at(m_hexes.size()-7).y;
}

void Board::rotateHexPlate(bool clockwise)
{
	if(clockwise)
	{
		for(int i=m_hexes.size()-2; i>m_hexes.size()-7; --i)
		{
			std::swap(m_hexes.at(i).x, m_hexes.at(m_hexes.size()-1).x);
			std::swap(m_hexes.at(i).y, m_hexes.at(m_hexes.size()-1).y);
		}		
	}
	else
	{
		for(int i=m_hexes.size()-5; i<m_hexes.size(); ++i)
		{
			std::swap(m_hexes.at(i).x, m_hexes.at(m_hexes.size()-6).x);
			std::swap(m_hexes.at(i).y, m_hexes.at(m_hexes.size()-6).y);
		}
	}
}

bool Board::checkValidHexPlace()
{	
	int adjacent = 0;
	for(int i=0; i<m_hexes.size()-7; i+=7)
	{
		if(m_selectedX==m_hexes.at(i).x && m_selectedY==m_hexes.at(i).y) { return false; }
		int oldX = m_hexes.at(i).x;
		int oldY = m_hexes.at(i).y;
		int sign = 1;
		if(oldX%2) { sign = -1; }
		if((m_selectedY==oldY 
			&& (m_selectedX+3==oldX || m_selectedX-3==oldX))
			|| (m_selectedY-(2*sign)==oldY
			&& (m_selectedX+1==oldX || m_selectedX-1==oldX || m_selectedX-2==oldX || m_selectedX+2==oldX))
			|| (m_selectedY+(1*sign)==oldY
			&& (m_selectedX-3==oldX || m_selectedX+3==oldX))
			|| (m_selectedY+(3*sign)==oldY
			&& (m_selectedX-1==oldX || m_selectedX+1==oldX))
			|| (m_selectedY+(2*sign)==oldY
			&& (m_selectedX+2==oldX || m_selectedX-2==oldX)))
		{
			++adjacent;
			if(adjacent>1 || m_hexes.size()==14) { return true; }
		}
	}
	return false;
}

void Board::placeHexPlate(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
		moveHexPlateY(false);
		break;
	case ALLEGRO_KEY_DOWN:
		moveHexPlateY(true);
		break;
	case ALLEGRO_KEY_LEFT:
		if(m_lcapsOn || m_rcapsOn) { rotateHexPlate(false); }
		else { moveHexPlateX(false); }
		break;
	case ALLEGRO_KEY_RIGHT:
		if(m_lcapsOn || m_rcapsOn) { rotateHexPlate(true); }
		else { moveHexPlateX(true); }
		break;
	case ALLEGRO_KEY_LSHIFT:
		m_lcapsOn = true;
		break;
	case ALLEGRO_KEY_RSHIFT:
		m_rcapsOn = true;
		break;
	case ALLEGRO_KEY_ENTER:
		if(checkValidHexPlace())
		{ nextPlayer(); }
		break;
	}
}

void Board::findSpawnPools()
{
	m_spawnPools.clear();
	m_spawnPools.reserve(m_hexes.size()/7);
	for(int i=0; i<m_hexes.size(); ++i)
	{
		if(!(m_hexes.at(i).occupied) && m_hexes.at(i).number==m_spawnDie)
		{ m_spawnPools.push_back(i); }
	}
	if(m_spawnPools.size()<1)
	{ m_spawnDie = rand()%6+1; findSpawnPools(); }
	m_selectedPool = 0;
	m_selectedX = m_hexes.at(m_spawnPools.at(m_selectedPool)).x;
	m_selectedY = m_hexes.at(m_spawnPools.at(m_selectedPool)).y;
}

void Board::nextSpawnPool()
{
	++m_selectedPool;
	if(m_selectedPool==m_spawnPools.size())
	{ m_selectedPool = 0; }
	m_selectedX = m_hexes.at(m_spawnPools.at(m_selectedPool)).x;
	m_selectedY = m_hexes.at(m_spawnPools.at(m_selectedPool)).y;
}

void Board::previousSpawnPool()
{
	--m_selectedPool;
	if(m_selectedPool<0)
	{ m_selectedPool = m_spawnPools.size()-1; }
	m_selectedX = m_hexes.at(m_spawnPools.at(m_selectedPool)).x;
	m_selectedY = m_hexes.at(m_spawnPools.at(m_selectedPool)).y;
}

void Board::placeMonster(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
	case ALLEGRO_KEY_LEFT:
		previousSpawnPool();
		break;
	case ALLEGRO_KEY_DOWN:
	case ALLEGRO_KEY_RIGHT:
		nextSpawnPool();
		break;
	case ALLEGRO_KEY_ENTER:
		++m_spawnCount;
		m_hexes.at(m_spawnPools.at(m_selectedPool)).occupied = true;
		m_players.at(m_currentPlayer).placeMonster(m_hexes.at(m_spawnPools.at(m_selectedPool)).x, m_hexes.at(m_spawnPools.at(m_selectedPool)).y);
		if(m_players.at(m_currentPlayer).getDoneSpawn() || m_spawnCount==(m_hexes.size()/7)) { m_spawnCount = 0; nextPlayer(); }
		else { findSpawnPools(); }
		break;
	}
}

void Board::nextPlayer()
{
	BoardState newState = m_state;
	switch(m_state)
	{
	case BoardState::START:
		++m_currentPlayer;
		if(m_currentPlayer==(m_players.size()-1))
		{ createHexPlate(); newState = BoardState::PLACEHEX; }
		else { createHexPlate(); }
		break;
	case BoardState::PLACEHEX:
		m_currentPlayer = ++m_firstPlayer;
		if(m_firstPlayer==m_players.size())
		{ m_firstPlayer = m_currentPlayer = 0; }
		m_moveDie = rand()%6+1;
		m_spawnDie = rand()%6+1;
		findSpawnPools();
		newState = BoardState::SPAWN;
		break;
	case BoardState::SPAWN:
		++m_currentPlayer;
		m_spawnDie = rand()%6+1;
		findSpawnPools();
		if(m_currentPlayer==m_players.size())
		{ m_currentPlayer = 0; }
		if(m_currentPlayer==m_firstPlayer)
		{ newState = BoardState::MOVE; }
		break;
	case BoardState::EAT:
		++m_currentPlayer;
		if(m_currentPlayer==m_players.size())
		{ m_currentPlayer = 0; }
		if(m_currentPlayer==m_firstPlayer && (m_hexes.size()/7)<(m_players.size()+7))
		{ createHexPlate(); newState = BoardState::PLACEHEX; }
		else if(m_currentPlayer==m_firstPlayer)
		{ newState = BoardState::SCORE; }
		break;
	}
	m_state = newState;
}

void Board::shutdown()
{
	al_destroy_font(m_font);
	for(int i=0; i<m_hexBMPs.size(); ++i)
	{
		al_destroy_bitmap(m_hexBMPs.at(i));
	}
}

Board::~Board(void)
{
}
