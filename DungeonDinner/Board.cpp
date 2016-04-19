#include "Board.h"

Board::Board(void)
	:m_lcapsOn(false),
	m_rcapsOn(false),
	m_flip(false),
	m_selected(false),
	m_currentPlayer(0),
	m_firstPlayer(0),
	m_spawnDie(0),
	m_moveDie(0),
	m_spawnCount(0),
	m_moveCount(0),
	m_selectedX(0),
	m_selectedY(0),
	m_selectedHex(0),
	m_selectedEnemy(0),
	m_enemyPlayer(0),
	m_state(BoardState::START),
	m_players(),
	m_spawnPools(),
	m_activeMonsters(),
	m_adjacentEmpties(),
	m_adjacentEnemies(),
	m_font(al_load_font("pirulen.ttf", static_cast<int>(TextSize::MEDIUM), 0)),
	m_hexBMPs(3, nullptr),
	m_hexes()
{
	m_hexBMPs.at(0) = al_load_bitmap("FullHex.png");
	m_hexBMPs.at(1) = al_load_bitmap("Target.png");
	m_hexBMPs.at(2) = al_load_bitmap("Slime.png");
	m_players.reserve(4); //max players
	m_spawnPools.reserve(4+7); //max spawnPools for four players and eight rounds
	m_activeMonsters.reserve(4); //max monsters after first spawn
	m_adjacentEmpties.reserve(6); //max empty adjacent hexes
	m_adjacentEnemies.reserve(12); //max adjacent enemies after first move
	m_hexes.reserve(7*(4+7)); //max hexes for four players and eight rounds with seven hexes per plate.
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
		moveMonster(keyPressed);
		break;
	case BoardState::EAT:
		eatMonster(keyPressed);
		break;
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
	m_selectedX = m_selectedY = m_selectedHex = m_selectedEnemy = m_currentPlayer = m_enemyPlayer = m_flip = m_selected = m_spawnDie = m_moveDie = m_spawnCount = m_moveCount = 0;
	for(int i=0; i<players.number; ++i)
	{
		m_players.push_back(Player(i, players.colour[i], players.name[i]));
	}
	m_firstPlayer = m_players.size()-1;
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

	if(abs(m_selectedX)>18 || abs(m_selectedY)>15)
	{ if(right) { moveHexPlateX(false); } else { moveHexPlateX(true); } }
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

	if(abs(m_selectedX)>18 || abs(m_selectedY)>15)
	{ if(down) { moveHexPlateY(false); } else { moveHexPlateY(true); } }
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
	else
	{
		m_selectedHex = 0;
		m_selectedX = m_hexes.at(m_spawnPools.at(m_selectedHex)).x;
		m_selectedY = m_hexes.at(m_spawnPools.at(m_selectedHex)).y;
	}
}

void Board::nextHex()
{
	++m_selectedHex;
	switch(m_state)
	{
	case BoardState::SPAWN:
		if(m_selectedHex==m_spawnPools.size())
		{ m_selectedHex = 0; }
		m_selectedX = m_hexes.at(m_spawnPools.at(m_selectedHex)).x;
		m_selectedY = m_hexes.at(m_spawnPools.at(m_selectedHex)).y;
		break;
	case BoardState::MOVE:
		if(!m_selected)
		{
			if(m_selectedHex==m_activeMonsters.size())
			{ m_selectedHex = 0; }
			m_selectedX = m_hexes.at(m_activeMonsters.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_activeMonsters.at(m_selectedHex)).y;
		}
		else
		{
			if(m_selectedHex==m_adjacentEmpties.size())
			{ m_selectedHex = 0; }
			m_selectedX = m_hexes.at(m_adjacentEmpties.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_adjacentEmpties.at(m_selectedHex)).y;
		}
		break;
	case BoardState::EAT:
		if(!m_selected)
		{
			if(m_selectedHex==m_adjacentEnemies.size())
			{ m_selectedHex = 0; }
			m_selectedX = m_hexes.at(m_adjacentEnemies.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_adjacentEnemies.at(m_selectedHex)).y;
		}
		else
		{
			if(m_selectedHex==m_activeMonsters.size())
			{ m_selectedHex = 0; }
			m_selectedX = m_hexes.at(m_activeMonsters.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_activeMonsters.at(m_selectedHex)).y;
		}
		break;
	}
}

void Board::previousHex()
{
	--m_selectedHex;
	switch(m_state)
	{
	case BoardState::SPAWN:
		if(m_selectedHex<0)
		{ m_selectedHex = m_spawnPools.size()-1; }
		m_selectedX = m_hexes.at(m_spawnPools.at(m_selectedHex)).x;
		m_selectedY = m_hexes.at(m_spawnPools.at(m_selectedHex)).y;
		break;
	case BoardState::MOVE:
		if(!m_selected)
		{
			if(m_selectedHex<0)
			{ m_selectedHex = m_activeMonsters.size()-1; }
			m_selectedX = m_hexes.at(m_activeMonsters.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_activeMonsters.at(m_selectedHex)).y;
		}
		else
		{
			if(m_selectedHex<0)
			{ m_selectedHex = m_adjacentEmpties.size()-1; }
			m_selectedX = m_hexes.at(m_adjacentEmpties.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_adjacentEmpties.at(m_selectedHex)).y;
		}
		break;
	case BoardState::EAT:
		if(!m_selected)
		{
			if(m_selectedHex<0)
			{ m_selectedHex = m_adjacentEnemies.size()-1; }
			m_selectedX = m_hexes.at(m_adjacentEnemies.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_adjacentEnemies.at(m_selectedHex)).y;
		}
		else
		{
			if(m_selectedHex<0)
			{ m_selectedHex = m_activeMonsters.size()-1; }
			m_selectedX = m_hexes.at(m_activeMonsters.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_activeMonsters.at(m_selectedHex)).y;
		}
		break;
	}
}

void Board::placeMonster(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
	case ALLEGRO_KEY_LEFT:
		previousHex();
		break;
	case ALLEGRO_KEY_DOWN:
	case ALLEGRO_KEY_RIGHT:
		nextHex();
		break;
	case ALLEGRO_KEY_ENTER:
		++m_spawnCount;
		m_hexes.at(m_spawnPools.at(m_selectedHex)).occupied = true;
		m_players.at(m_currentPlayer).placeMonster(m_selectedX, m_selectedY);
		if(m_players.at(m_currentPlayer).getDoneSpawn() || m_spawnCount==(m_hexes.size()/7)) { m_spawnCount = 0; nextPlayer(); }
		else { findSpawnPools(); }
		break;
	}
}

void Board::findMonsters()
{
	m_activeMonsters.clear();
	for(int i=0; i<m_players.at(m_currentPlayer).getMonsters().size(); ++i)
	{
		if(m_players.at(m_currentPlayer).getMonsters().at(i).placed && !m_players.at(m_currentPlayer).getMonsters().at(i).moved)
		{		
			for(int h=0; h<m_hexes.size(); ++h)
			{
				if(m_hexes.at(h).occupied)
				{
					if(m_hexes.at(h).x==m_players.at(m_currentPlayer).getMonsters().at(i).x
						&& m_hexes.at(h).y==m_players.at(m_currentPlayer).getMonsters().at(i).y)
					{ m_activeMonsters.push_back(h); }
				}
			}
		}
	}
	if(m_activeMonsters.size()<1)
	{
		m_moveCount = 0;
		int enemies = 0;
		for(int i=0; i<m_players.size(); ++i)
		{ enemies += m_players.at(i).getMonsters().size(); }
		m_adjacentEnemies.reserve(enemies);
		m_state = BoardState::EAT;
		findAdjacentEnemies();
	}
	else
	{
		m_selectedHex = 0;
		m_selectedX = m_hexes.at(m_activeMonsters.at(m_selectedHex)).x;
		m_selectedY = m_hexes.at(m_activeMonsters.at(m_selectedHex)).y;
	}
}

void Board::findAdjacentEmpties()
{
	m_adjacentEmpties.clear();
	int x[6];
	int y[6];
	x[0] = m_selectedX;
	y[0] = m_selectedY-1;
	x[1] = m_selectedX+1;
	y[1] = m_selectedY-1+(abs(m_selectedX)%2);
	x[2] = m_selectedX+1;
	y[2] = m_selectedY+(abs(m_selectedX)%2);
	x[3] = m_selectedX;
	y[3] = m_selectedY+1;
	x[4] = m_selectedX-1;
	y[4] = m_selectedY+(abs(m_selectedX)%2);
	x[5] = m_selectedX-1;
	y[5] = m_selectedY-1+(abs(m_selectedX)%2);
	int adjacent[6];
	bool hexFound[6] = {false, false, false, false, false, false};
	for(int i=0; i<m_hexes.size(); ++i)
	{
		if(!m_hexes.at(i).occupied)
		{
			if(!hexFound[0] && m_hexes.at(i).x==x[0] && m_hexes.at(i).y==y[0]) { adjacent[0] = i; hexFound[0] = true; }
			if(!hexFound[1] && m_hexes.at(i).x==x[1] && m_hexes.at(i).y==y[1]) { adjacent[1] = i; hexFound[1] = true; }
			if(!hexFound[2] && m_hexes.at(i).x==x[2] && m_hexes.at(i).y==y[2]) { adjacent[2] = i; hexFound[2] = true; }
			if(!hexFound[3] && m_hexes.at(i).x==x[3] && m_hexes.at(i).y==y[3]) { adjacent[3] = i; hexFound[3] = true; }
			if(!hexFound[4] && m_hexes.at(i).x==x[4] && m_hexes.at(i).y==y[4]) { adjacent[4] = i; hexFound[4] = true; }
			if(!hexFound[5] && m_hexes.at(i).x==x[5] && m_hexes.at(i).y==y[5]) { adjacent[5] = i; hexFound[5] = true; }
			if(hexFound[0] && hexFound[1] && hexFound[2] && hexFound[3] && hexFound[4] && hexFound[5])
			{ break; }
		}
	}
	for(int i=0; i<6; ++i)
	{
		if(hexFound[i]) { m_adjacentEmpties.push_back(adjacent[i]); }
	}
	if(m_adjacentEmpties.size()<1)
	{
		m_selected = false;
		m_hexes.at(m_activeMonsters.at(m_selectedHex)).occupied = true;
		findMonsters();
	}
	else
	{
		m_selectedHex = 0;
		m_selectedX = m_hexes.at(m_adjacentEmpties.at(m_selectedHex)).x;
		m_selectedY = m_hexes.at(m_adjacentEmpties.at(m_selectedHex)).y;
	}
}

void Board::moveMonster(ALLEGRO_EVENT& keyPressed)
{	
	int enemies = 0;
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
	case ALLEGRO_KEY_LEFT:
		previousHex();
		break;
	case ALLEGRO_KEY_DOWN:
	case ALLEGRO_KEY_RIGHT:
		nextHex();
		break;
	case ALLEGRO_KEY_SPACE:
		m_moveCount = 0;
		for(int i=0; i<m_players.size(); ++i)
		{ enemies += m_players.at(i).getMonsters().size(); }
		m_adjacentEnemies.reserve(enemies);
		m_state = BoardState::EAT;
		findAdjacentEnemies();
		break;
	case ALLEGRO_KEY_ENTER:
		if(!m_selected)
		{
			m_selected = true;
			m_hexes.at(m_activeMonsters.at(m_selectedHex)).occupied = false;
			m_players.at(m_currentPlayer).selectMonster(m_selectedX, m_selectedY);
			findAdjacentEmpties();
		}
		else
		{
			m_hexes.at(m_adjacentEmpties.at(m_selectedHex)).occupied = true;
			m_players.at(m_currentPlayer).moveMonster(m_selectedX, m_selectedY);
			m_selected = false;
			++m_moveCount;
			if(m_moveCount==m_moveDie)
			{
				m_moveCount = 0;
				for(int i=0; i<m_players.size(); ++i)
				{ enemies += m_players.at(i).getMonsters().size(); }
				m_adjacentEnemies.reserve(enemies);
				m_state = BoardState::EAT;
				findAdjacentEnemies();
			}
			else { findMonsters(); }
		}
		break;
	}
}

void Board::findAdjacentEnemies()
{
	m_adjacentEnemies.clear();
	m_activeMonsters.clear();
	for(int i=0; i<m_players.at(m_currentPlayer).getMonsters().size(); ++i) //locate each placed monster of current player
	{
		if(m_players.at(m_currentPlayer).getMonsters().at(i).placed)
		{		
			for(int h=0; h<m_hexes.size(); ++h)
			{
				if(m_hexes.at(h).occupied)
				{
					if(m_hexes.at(h).x==m_players.at(m_currentPlayer).getMonsters().at(i).x
						&& m_hexes.at(h).y==m_players.at(m_currentPlayer).getMonsters().at(i).y)
					{ m_activeMonsters.push_back(h); }
				}
			}
		}
	}
	if(m_activeMonsters.size()<1)
	{ nextPlayer(); }
	else
	{
		int x[6];
		int y[6];
		bool notFriend = true;
		for(int i=0; i<m_activeMonsters.size(); ++i) //locate adjacent hexes around each placed monster of currentPlayer
		{
			int friendlyX = m_hexes.at(m_activeMonsters.at(i)).x;
			int friendlyY = m_hexes.at(m_activeMonsters.at(i)).y;
			x[0] = friendlyX;
			y[0] = friendlyY-1;
			x[1] = friendlyX+1;
			y[1] = friendlyY-1+(abs(friendlyX)%2);
			x[2] = friendlyX+1;
			y[2] = friendlyY+(abs(friendlyX)%2);
			x[3] = friendlyX;
			y[3] = friendlyY+1;
			x[4] = friendlyX-1;
			y[4] = friendlyY+(abs(friendlyX)%2);
			x[5] = friendlyX-1;
			y[5] = friendlyY-1+(abs(friendlyX)%2);
			for(int h=0; h<m_hexes.size(); ++h) //check all hexes to locate the adjacent ones
			{
				if(m_hexes.at(h).occupied) //only occupied hexes have enemies
				{
					for(int f=0; f<m_activeMonsters.size(); ++f) //make sure not a friendly
					{ if(m_activeMonsters.at(f)==h) { notFriend = false; break; } }
					if(notFriend)
					{
						for(int a=0; a<6; ++a) //compare with each adjacent hex for this monster
						{
							if(m_hexes.at(h).x==x[a] && m_hexes.at(h).y==y[a]) //matches an adjacent hex
							{ m_adjacentEnemies.push_back(h); }
						}
					}
					notFriend = true;
				}
			}
		}
		if(m_adjacentEnemies.size()<1)
		{ nextPlayer();	}
		else
		{
			m_selectedHex = 0;
			m_selectedX = m_hexes.at(m_adjacentEnemies.at(m_selectedHex)).x;
			m_selectedY = m_hexes.at(m_adjacentEnemies.at(m_selectedHex)).y;
		}
	}
}

void Board::findAdjacentFriends()
{
	int x[6];
	int y[6];
	int enemyX = m_hexes.at(m_adjacentEnemies.at(m_selectedEnemy)).x;
	int enemyY = m_hexes.at(m_adjacentEnemies.at(m_selectedEnemy)).y;
	x[0] = enemyX;
	y[0] = enemyY-1;
	x[1] = enemyX+1;
	y[1] = enemyY-1+(abs(enemyX)%2);
	x[2] = enemyX+1;
	y[2] = enemyY+(abs(enemyX)%2);
	x[3] = enemyX;
	y[3] = enemyY+1;
	x[4] = enemyX-1;
	y[4] = enemyY+(abs(enemyX)%2);
	x[5] = enemyX-1;
	y[5] = enemyY-1+(abs(enemyX)%2);
	int adjacent[6];
	bool notEnemy[6] = {false, false, false, false, false, false};
	for(int h=0; h<m_hexes.size(); ++h) //check all hexes to locate the ones adjacent to the enemy
	{
		if(m_hexes.at(h).occupied) //only occupied hexes have friendlies
		{
			for(int a=0; a<6; ++a) //compare with each adjacent hex for this monster
			{
				if(m_hexes.at(h).x==x[a] && m_hexes.at(h).y==y[a]) //matches an adjacent hex
				{
					for(int f=0; f<m_activeMonsters.size(); ++f) //check if friendly!
					{ if(m_activeMonsters.at(f)==h) { notEnemy[a] = true; adjacent[a] = h; } }
				}
			}
		}
	}
	m_activeMonsters.clear();
	for(int i=0; i<6; ++i)
	{ if(notEnemy[i]) { m_activeMonsters.push_back(adjacent[i]); } }
	m_selectedHex = 0;
	m_selectedX = m_hexes.at(m_activeMonsters.at(m_selectedHex)).x;
	m_selectedY = m_hexes.at(m_activeMonsters.at(m_selectedHex)).y;
}

bool Board::eatOrEaten()
{
	//count adjacent enemies
	int enemyCount = 0;
	int x[6];
	int y[6];
	int friendlyX = m_hexes.at(m_activeMonsters.at(m_selectedHex)).x;
	int friendlyY = m_hexes.at(m_activeMonsters.at(m_selectedHex)).y;
	x[0] = friendlyX;
	y[0] = friendlyY-1;
	x[1] = friendlyX+1;
	y[1] = friendlyY-1+(abs(friendlyX)%2);
	x[2] = friendlyX+1;
	y[2] = friendlyY+(abs(friendlyX)%2);
	x[3] = friendlyX;
	y[3] = friendlyY+1;
	x[4] = friendlyX-1;
	y[4] = friendlyY+(abs(friendlyX)%2);
	x[5] = friendlyX-1;
	y[5] = friendlyY-1+(abs(friendlyX)%2);
	for(int h=0; h<m_hexes.size(); ++h) //check all hexes to locate the adjacent ones
	{
		if(m_hexes.at(h).occupied) //only occupied hexes have enemies
		{
			for(int a=0; a<6; ++a) //compare with each adjacent hex for this monster
			{
				if(m_hexes.at(h).x==x[a] && m_hexes.at(h).y==y[a]) //matches an adjacent hex
				{
					for(int e=0; e<m_adjacentEnemies.size(); ++e) //check if enemy!
					{ if(m_adjacentEnemies.at(e)==h) { ++enemyCount; if(enemyCount>1) { return false; } } }
				}
			}
		}
	}
	//check level of adjacent enemy
	int enemyX = m_hexes.at(m_adjacentEnemies.at(m_selectedEnemy)).x;
	int enemyY = m_hexes.at(m_adjacentEnemies.at(m_selectedEnemy)).y;
	for(int i=0; i<m_players.size(); ++i)
	{
		if(i==m_currentPlayer) { continue; } //enemy is not current player
		for(int m=0; m<m_players.at(i).getMonsters().size(); ++m) //check each placed monster location
		{
			if(m_players.at(i).getMonsters().at(m).placed
				&& m_players.at(i).getMonsters().at(m).x==enemyX && m_players.at(i).getMonsters().at(m).y==enemyY)
			{
				if(m_players.at(i).getMonsters().at(m).level<=m_players.at(m_currentPlayer).getLevel())
				{ m_enemyPlayer = i; m_players.at(i).selectMonster(m); return true; }
			}
		}
	}
	//check if enemy is surrounded
	if(m_activeMonsters.size()>1) { return true; }
	return false;
}

void Board::resolveEating(bool eat)
{
	if(eat)
	{
		m_players.at(m_currentPlayer).levelUp();
		if(m_activeMonsters.size()>1)
		{
			for(int i=0; i<m_activeMonsters.size(); ++i)
			{
				if(i!=m_selectedHex)
				{ m_players.at(m_currentPlayer).levelUp(m_hexes.at(m_activeMonsters.at(i)).x, m_hexes.at(m_activeMonsters.at(i)).y); }
			}
			m_players.at(m_enemyPlayer).demote();
		}
		m_hexes.at(m_adjacentEnemies.at(m_selectedEnemy)).occupied = false;
		m_players.at(m_enemyPlayer).removeMonster();
	}
	else
	{
		m_hexes.at(m_activeMonsters.at(m_selectedHex)).occupied = false;
		m_players.at(m_currentPlayer).removeMonster();
	}
}

void Board::eatMonster(ALLEGRO_EVENT& keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
	case ALLEGRO_KEY_LEFT:
		previousHex();
		break;
	case ALLEGRO_KEY_DOWN:
	case ALLEGRO_KEY_RIGHT:
		nextHex();
		break;
	case ALLEGRO_KEY_ENTER:
		if(!m_selected)
		{
			m_selected = true;
			m_selectedEnemy = m_selectedHex;
			findAdjacentFriends();
		}
		else
		{
			m_selected = false;
			m_players.at(m_currentPlayer).selectMonster(m_selectedX, m_selectedY);
			resolveEating(eatOrEaten());
			findAdjacentEnemies();
		}
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
		if(m_currentPlayer==m_players.size())
		{ m_currentPlayer = 0; }
		if(m_currentPlayer==m_firstPlayer)
		{
			m_activeMonsters.reserve(m_players.at(m_currentPlayer).getMonsters().size());
			findMonsters();
			newState = BoardState::MOVE;
		}
		else { m_spawnDie = rand()%6+1; findSpawnPools(); }
		break;
	case BoardState::EAT:
		++m_currentPlayer;
		if(m_currentPlayer==m_players.size())
		{ m_currentPlayer = 0; }
		if(m_currentPlayer==m_firstPlayer && (m_hexes.size()/7)<(m_players.size()+7))
		{ createHexPlate(); newState = BoardState::PLACEHEX; }
		else if(m_currentPlayer==m_firstPlayer)
		{ newState = BoardState::SCORE; }
		else
		{ m_moveCount = 0; newState = BoardState::MOVE; findMonsters(); }
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
