#ifndef STRUCTS_H
#define STRUCTS_H

struct PlayerDetails
{
	int number;
	std::string name[4];
	PlayerColour colour[4];
};

struct Hex
{
	int x,
		y,
		number;
	Terrain terrain;
};

struct Monster
{
	int x,
		y,
		level;
	bool moved,
		placed;
};

#endif