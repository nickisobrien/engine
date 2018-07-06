#pragma once

enum Blocktype
{
	Air = 0,
	Grass,
	Dirt,
	Water,
	Stone,
    Wood,
    Sand
};

class Block
{
public:
	Block();
	~Block();

	bool isActive(void);
	void setActive(bool act);
	int getType(void);
private:
	bool active; // can switch to having an array of active sides
	Blocktype type = Grass;
};