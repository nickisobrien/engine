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
	bool active;
	int type = 0; //need to change to smaller data size ie. char/short// 1==grass, 0==air, 2==sand, 3==snow 
};