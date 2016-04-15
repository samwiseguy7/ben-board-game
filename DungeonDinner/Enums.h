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

enum class TextSize
{
	SMALL = 12,
	MEDIUM = 20,
	BIG = 32,
};

enum class PlayerColour
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	CYAN,
	MAGENTA,
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
	BLACK,
	DIM,
	LIT,
	TOTAL
};

#endif