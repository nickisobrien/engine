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
	void render(void);
	int getType(void);
private:
	bool active;
	Blocktype type = Grass;
};