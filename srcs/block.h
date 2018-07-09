#pragma once

class Block
{
public:
	Block();
	~Block();

	bool isActive(void);
	void setActive(bool act);
	int getType(void);
	void setType(int t);
private:
	bool active; // can switch to having an array of active sides
	int type = 0; // 1 is grass, 0 is air
};