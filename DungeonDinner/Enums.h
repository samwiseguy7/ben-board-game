#ifndef ENUMS_H
#define ENUMS_H

enum class Screen
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
	SMALL = 10,
	MEDIUM = 20,
	BIG = 32,
};

#endif