#ifndef ENUMS_H
#define ENUMS_H

enum class ScreenMode
{
	MENU,
	BOARD,
	DONE
};

enum class MenuOp
{
	NEW,
	QUIT,
	TOTAL
};

enum class MenuState
{
	SELECT,
	NUMBER,
	NAME,
	COLOUR
};

enum class BoardState
{
	START,
	PLACEHEX,
	SPAWN,
	MOVE,
	EAT,
	SCORE
};

enum class TextSize
{
	SMALL = 12,
	MEDIUM = 20,
	BIG = 32,
};

enum class Terrain
{
	DUNGEON,
	VEGETATION,
	OOZE,
	MINE,
	MUSHROOM,
	FRUIT,
	DIRT,
	TOTAL
};

enum class PlayerColour
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	CYAN,
	MAGENTA,
	BROWN,
	TOTAL
};

enum class Colour
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	CYAN,
	MAGENTA,
	BROWN,
	BLACK,
	DIM,
	LIT,
	TOTAL
};

#endif